// Personal location trail and pressure-history stitching.
//
// The watch has no barometer, so the past 24 h of the barograph is Open-Meteo
// pressure_msl. Recording where the phone was — not the pressure — lets us
// fill each past hour from the place the user was actually in. pressure_msl
// is sea-level reduced, so values from different places are comparable.
// See docs/design/01-data-protocol.md.

var pack = require('./pack');

var HOURLY_SAMPLES = pack.HOURLY_SAMPLES;
var NOW_IDX = pack.NOW_IDX;
var HOUR_S = 3600;

// ~0.01°: model grids are kilometre-scale, and finer coordinates are a
// privacy cost with no benefit to the graph.
var COORD_STEP = 100;
// Consecutive fixes closer than this stay one "place".
var CLUSTER_KM = 5;
// Keep a little more than the 24 h graph so the oldest hour still has a place.
var RETENTION_S = 30 * HOUR_S;
var MAX_STORED_PLACES = 48;
// Each extra coordinate is another free-tier API call. Includes the current fix.
var MAX_PLACES_PER_REQUEST = 6;
// Open-Meteo snaps requests onto its grid; this is how far a response may sit
// from the coordinate we asked for and still count as that place.
var MATCH_KM = 50;
var MAX_CACHE_HOURS = 48;

var TRAIL_KEY = 'aethercast-trail';
var CACHE_KEY = 'aethercast-pressure-cache';

function roundCoord(v) {
  return Math.round(Number(v) * COORD_STEP) / COORD_STEP;
}

function distanceKm(lat1, lon1, lat2, lon2) {
  var r = 6371;
  var dLat = (lat2 - lat1) * Math.PI / 180;
  var dLon = (lon2 - lon1) * Math.PI / 180;
  var a = Math.sin(dLat / 2) * Math.sin(dLat / 2) +
    Math.cos(lat1 * Math.PI / 180) * Math.cos(lat2 * Math.PI / 180) *
    Math.sin(dLon / 2) * Math.sin(dLon / 2);
  return 2 * r * Math.asin(Math.min(1, Math.sqrt(a)));
}

function samePlace(a, b) {
  if (!a || !b) return false;
  return distanceKm(a.lat, a.lon, b.lat, b.lon) <= CLUSTER_KM;
}

function placeKey(p) {
  return roundCoord(p.lat).toFixed(2) + ',' + roundCoord(p.lon).toFixed(2);
}

function hourKey(t) {
  return String(Math.round(t));
}

// Follow me is the default. Manual location and "this location only" both
// leave the trail unused and do not record new fixes.
function trailActive(settings) {
  if (!settings) return true;
  if (settings.manualLocationEnabled) return false;
  if (settings.pressureHistory === 1) return false;
  return true;
}

function configRequestsClear(settings) {
  if (!settings) return false;
  var clear = settings.clearLocationHistory;
  return clear === true || (!!clear && clear.value === true);
}

function hourTimesFromNow(now) {
  var currentHour = Math.floor(now / HOUR_S) * HOUR_S;
  var t0 = currentHour - NOW_IDX * HOUR_S;
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) out.push(t0 + i * HOUR_S);
  return out;
}

// Drop stays that had already ended before the retention cutoff, but keep
// the place that was current at the cutoff so the start of the graph still
// has somewhere to point. Then merge the shortest finished stays until the
// stored list fits MAX_STORED_PLACES.
function prunePlaces(places, now) {
  var ordered = places.slice().sort(function (a, b) { return a.t - b.t; });
  if (!ordered.length) return [];
  var cutoff = now - RETENTION_S;
  var start = 0;
  for (var i = 0; i < ordered.length; i++) {
    if (ordered[i].t <= cutoff) start = i;
  }
  var out = ordered.slice(start);
  while (out.length > MAX_STORED_PLACES) {
    var shortest = 0;
    var shortestDur = Infinity;
    var last = out.length - 1;
    for (var j = 0; j < last; j++) {
      var dur = out[j + 1].t - out[j].t;
      if (dur < shortestDur) {
        shortestDur = dur;
        shortest = j;
      }
    }
    out.splice(shortest, 1);
  }
  return out;
}

// Append a fix. A fix within CLUSTER_KM of the newest place does not move
// that place's entry time or coordinates — the entry time is when the user
// arrived, which is what hour assignment needs.
function appendFix(places, lat, lon, t) {
  var fix = { t: t, lat: roundCoord(lat), lon: roundCoord(lon) };
  var next = places.slice();
  if (!next.length || !samePlace(next[next.length - 1], fix)) {
    next.push(fix);
  }
  return prunePlaces(next, t);
}

// Most recent place with t <= the hour. Hours before the oldest fix use the
// oldest place. The current hour and the forecast use `current`.
function assignHours(places, hourTimes, current, nowIdx) {
  var ordered = places.slice().sort(function (a, b) { return a.t - b.t; });
  var currentR = { lat: roundCoord(current.lat), lon: roundCoord(current.lon) };
  var out = [];
  for (var i = 0; i < hourTimes.length; i++) {
    if (i >= nowIdx) {
      out.push({ lat: currentR.lat, lon: currentR.lon });
      continue;
    }
    var chosen = null;
    for (var p = 0; p < ordered.length; p++) {
      if (ordered[p].t <= hourTimes[i]) chosen = ordered[p];
      else break;
    }
    if (!chosen) chosen = ordered.length ? ordered[0] : currentR;
    out.push({ lat: roundCoord(chosen.lat), lon: roundCoord(chosen.lon) });
  }
  return out;
}

function distinctInOrder(assigned) {
  var seen = {};
  var list = [];
  for (var i = 0; i < assigned.length; i++) {
    var key = placeKey(assigned[i]);
    if (seen[key]) continue;
    seen[key] = true;
    list.push({ key: key, lat: assigned[i].lat, lon: assigned[i].lon });
  }
  return list;
}

// Merge the shortest non-current place into a neighbour until only `cap`
// distinct places remain. The current fix is never dropped: the forecast
// half of the graph is always that place.
function downsampleAssignments(assignments, current, cap) {
  var assigned = [];
  for (var i = 0; i < assignments.length; i++) {
    assigned.push({ lat: roundCoord(assignments[i].lat), lon: roundCoord(assignments[i].lon) });
  }
  var guard = 0;
  while (guard++ < 64) {
    var groups = distinctInOrder(assigned);
    if (groups.length <= cap) break;
    var counts = {};
    for (var g = 0; g < groups.length; g++) counts[groups[g].key] = 0;
    for (var n = 0; n < assigned.length; n++) counts[placeKey(assigned[n])]++;

    var victim = null;
    var victimCount = Infinity;
    for (var g = 0; g < groups.length; g++) {
      if (samePlace(groups[g], current)) continue;
      if (counts[groups[g].key] < victimCount) {
        victimCount = counts[groups[g].key];
        victim = groups[g];
      }
    }
    if (!victim) break;

    var best = null;
    var bestDist = Infinity;
    for (var s = 0; s < assigned.length; s++) {
      if (placeKey(assigned[s]) !== victim.key) continue;
      var cands = [];
      if (s > 0 && placeKey(assigned[s - 1]) !== victim.key) cands.push(assigned[s - 1]);
      if (s + 1 < assigned.length && placeKey(assigned[s + 1]) !== victim.key) cands.push(assigned[s + 1]);
      for (var c = 0; c < cands.length; c++) {
        var dist = distanceKm(victim.lat, victim.lon, cands[c].lat, cands[c].lon);
        if (dist < bestDist) {
          bestDist = dist;
          best = cands[c];
        }
      }
    }
    if (!best) break;
    for (var w = 0; w < assigned.length; w++) {
      if (placeKey(assigned[w]) === victim.key) {
        assigned[w] = { lat: best.lat, lon: best.lon };
      }
    }
  }
  return assigned;
}

function readCache(cache, hourT, place) {
  if (!cache || !cache.hours) return null;
  var entry = cache.hours[hourKey(hourT)];
  if (!entry || !isFinite(entry.p)) return null;
  if (!samePlace(entry, place)) return null;
  return entry.p;
}

// Current location is always fetched (conditions + forecast). Another place
// is fetched only when one of its past hours is not already cached.
function coordsToFetch(assignments, current, hourTimes, cache) {
  var currentR = { lat: roundCoord(current.lat), lon: roundCoord(current.lon) };
  var coords = [currentR];
  var seen = {};
  seen[placeKey(currentR)] = true;
  for (var i = 0; i < assignments.length && i < NOW_IDX; i++) {
    var p = assignments[i];
    if (samePlace(p, currentR)) continue;
    var key = placeKey(p);
    if (seen[key]) continue;
    var need = false;
    for (var h = 0; h < assignments.length && h < NOW_IDX; h++) {
      if (placeKey(assignments[h]) !== key) continue;
      if (readCache(cache, hourTimes[h], p) === null) {
        need = true;
        break;
      }
    }
    if (!need) {
      seen[key] = true;
      continue;
    }
    seen[key] = true;
    coords.push({ lat: roundCoord(p.lat), lon: roundCoord(p.lon) });
  }
  return coords;
}

function planFetch(places, current, now, cache) {
  var currentR = { lat: roundCoord(current.lat), lon: roundCoord(current.lon) };
  var hourTimes = hourTimesFromNow(now);
  var assignments = downsampleAssignments(
    assignHours(places, hourTimes, currentR, NOW_IDX),
    currentR,
    MAX_PLACES_PER_REQUEST);
  var coords = coordsToFetch(assignments, currentR, hourTimes, cache || { hours: {} });
  return {
    places: places,
    current: currentR,
    hourTimes: hourTimes,
    assignments: assignments,
    coords: coords,
    apiCalls: coords.length,
  };
}

function alignResults(coords, results) {
  if (!Array.isArray(results)) return null;
  var aligned = [];
  var used = {};
  for (var c = 0; c < coords.length; c++) {
    var best = -1;
    var bestDist = Infinity;
    for (var i = 0; i < results.length; i++) {
      if (used[i] || !results[i] || !isFinite(results[i].latitude) || !isFinite(results[i].longitude)) {
        continue;
      }
      var dist = distanceKm(coords[c].lat, coords[c].lon, results[i].latitude, results[i].longitude);
      if (dist < bestDist) {
        bestDist = dist;
        best = i;
      }
    }
    if (best < 0 || bestDist > MATCH_KM) return null;
    used[best] = true;
    aligned.push(results[best]);
  }
  return aligned;
}

function takeHours(times) {
  var out = [];
  var n = Math.min(times && times.length ? times.length : 0, HOURLY_SAMPLES);
  for (var i = 0; i < n; i++) out.push(Math.round(times[i]));
  while (out.length < HOURLY_SAMPLES) {
    var prev = out.length ? out[out.length - 1] : 0;
    out.push(prev + HOUR_S);
  }
  return out;
}

function buildMask(used) {
  var mask = [0, 0, 0, 0, 0];
  for (var i = 1; i < used.length && i < HOURLY_SAMPLES; i++) {
    if (!samePlace(used[i], used[i - 1])) {
      mask[i >> 3] |= (1 << (i & 7));
    }
  }
  return mask;
}

function maskHasAny(mask) {
  for (var i = 0; i < mask.length; i++) {
    if (mask[i]) return true;
  }
  return false;
}

// The 3 h trend is series[now] - series[now-3]. A place change on any of
// those three steps (indices now-2, now-1, now) would make a drive look
// like a front.
function maskHitsTrendWindow(mask, nowIdx) {
  if (nowIdx < 3) return false;
  for (var i = nowIdx - 2; i <= nowIdx; i++) {
    if (mask[i >> 3] & (1 << (i & 7))) return true;
  }
  return false;
}

function finiteOrNull(value) {
  if (value === null || value === undefined) return null;
  var n = Number(value);
  return isFinite(n) ? n : null;
}

function pruneCache(cache, nowHour) {
  var cutoff = nowHour - RETENTION_S;
  var keys = [];
  var hours = cache && cache.hours ? cache.hours : {};
  for (var k in hours) {
    if (!Object.prototype.hasOwnProperty.call(hours, k)) continue;
    if (Number(k) >= cutoff) keys.push(k);
  }
  keys.sort(function (a, b) { return Number(a) - Number(b); });
  while (keys.length > MAX_CACHE_HOURS) keys.shift();
  var kept = {};
  for (var i = 0; i < keys.length; i++) kept[keys[i]] = hours[keys[i]];
  return { hours: kept };
}

function copyHours(cache) {
  var out = {};
  var hours = cache && cache.hours ? cache.hours : {};
  for (var k in hours) {
    if (Object.prototype.hasOwnProperty.call(hours, k)) out[k] = hours[k];
  }
  return out;
}

// trail + per-place hourly arrays → 36 pressures, a place-change mask, and
// (only when a move falls inside the 3 h window) the current place's own
// 3 h delta in tenths of hPa. Nulls are filled with pack.fillNulls, the
// same forward-fill / back-fill the single-location encoder uses.
function integrate(plan, results, cache) {
  var aligned = alignResults(plan.coords, results);
  if (!aligned) return { error: 4 };
  for (var i = 0; i < aligned.length; i++) {
    var hourly = aligned[i].hourly && aligned[i].hourly.pressure_msl;
    if (!Array.isArray(hourly)) return { error: 4 };
  }

  var currentJson = aligned[0];
  var times = takeHours(currentJson.hourly && currentJson.hourly.time);
  var current = { lat: roundCoord(plan.current.lat), lon: roundCoord(plan.current.lon) };
  var assignments = downsampleAssignments(
    assignHours(plan.places, times, current, NOW_IDX),
    current,
    MAX_PLACES_PER_REQUEST);

  var seriesByKey = {};
  for (var c = 0; c < plan.coords.length; c++) {
    seriesByKey[placeKey(plan.coords[c])] = aligned[c].hourly.pressure_msl;
  }
  var currentSeries = seriesByKey[placeKey(current)];
  if (!currentSeries) return { error: 4 };

  var hours = copyHours(cache);
  var raw = [];
  var used = [];
  for (var h = 0; h < HOURLY_SAMPLES; h++) {
    var place = assignments[h]
      ? { lat: roundCoord(assignments[h].lat), lon: roundCoord(assignments[h].lon) }
      : current;
    if (h < NOW_IDX) {
      var cached = readCache({ hours: hours }, times[h], place);
      if (cached !== null) {
        raw.push(cached);
        used.push(place);
        continue;
      }
    }
    // A null sample stays null so fillNulls can back-fill inside the
    // stitched series. Falling back to the current place is only for a
    // place we never fetched — substituting one hour at a time would both
    // invent a place change and pull in the wrong city's pressure.
    var series = seriesByKey[placeKey(place)];
    var own = series ? finiteOrNull(series[h]) : null;
    var plotted = place;
    var value = own;
    if (!series && !samePlace(place, current)) {
      value = finiteOrNull(currentSeries[h]);
      plotted = current;
    }
    raw.push(value);
    used.push(plotted);
    // Cache the source sample, not a forward-filled guess, and not the
    // forecast. A later revision of the same place then cannot move a
    // hour the graph has already shown.
    if (h < NOW_IDX && own !== null && samePlace(plotted, place)) {
      hours[hourKey(times[h])] = { lat: place.lat, lon: place.lon, p: own };
    }
  }

  var filled = pack.fillNulls(raw);
  if (!filled) return { error: 4 };
  filled = pack.normalizeLength(filled);

  var mask = buildMask(used);
  var delta = null;
  if (maskHitsTrendWindow(mask, NOW_IDX)) {
    var currentFilled = pack.fillNulls(currentSeries.slice(0, HOURLY_SAMPLES));
    if (currentFilled) {
      currentFilled = pack.normalizeLength(currentFilled);
      delta = Math.round((currentFilled[NOW_IDX] - currentFilled[NOW_IDX - 3]) * 10);
    }
  }

  return {
    pressure: filled,
    placeChange: maskHasAny(mask) ? mask : null,
    delta3Tenths: delta,
    cache: pruneCache({ hours: hours }, times[NOW_IDX]),
    currentJson: currentJson,
    apiCalls: plan.apiCalls,
  };
}

function createStore(storage) {
  function read(key) {
    try {
      var raw = storage.getItem(key);
      if (!raw) return null;
      return JSON.parse(raw);
    } catch (e) {
      return null;
    }
  }
  function write(key, value) {
    storage.setItem(key, JSON.stringify(value));
  }
  return {
    loadPlaces: function () {
      var data = read(TRAIL_KEY);
      if (!data || !Array.isArray(data.places)) return [];
      return data.places;
    },
    loadCache: function () {
      var data = read(CACHE_KEY);
      if (!data || !data.hours || typeof data.hours !== 'object') return { hours: {} };
      return { hours: data.hours };
    },
    recordFix: function (lat, lon, t) {
      var places = appendFix(this.loadPlaces(), lat, lon, t);
      write(TRAIL_KEY, { v: 1, places: places });
      return places;
    },
    saveCache: function (cache) {
      write(CACHE_KEY, { v: 1, hours: (cache && cache.hours) || {} });
    },
    clear: function () {
      storage.removeItem(TRAIL_KEY);
      storage.removeItem(CACHE_KEY);
    },
  };
}

module.exports = {
  CLUSTER_KM: CLUSTER_KM,
  RETENTION_S: RETENTION_S,
  MAX_STORED_PLACES: MAX_STORED_PLACES,
  MAX_PLACES_PER_REQUEST: MAX_PLACES_PER_REQUEST,
  NOW_IDX: NOW_IDX,
  HOURLY_SAMPLES: HOURLY_SAMPLES,
  roundCoord: roundCoord,
  distanceKm: distanceKm,
  samePlace: samePlace,
  placeKey: placeKey,
  trailActive: trailActive,
  configRequestsClear: configRequestsClear,
  hourTimesFromNow: hourTimesFromNow,
  appendFix: appendFix,
  prunePlaces: prunePlaces,
  assignHours: assignHours,
  downsampleAssignments: downsampleAssignments,
  planFetch: planFetch,
  integrate: integrate,
  alignResults: alignResults,
  buildMask: buildMask,
  createStore: createStore,
};
