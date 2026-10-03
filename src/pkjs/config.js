// Clay settings schema. See docs/vendor/pebble-app-configuration.md.
//
// UNIT_SYSTEM's messageKey matches a real watch-facing key (package.json /
// message_keys.json) - src/pkjs/index.js forwards it to the watch on every
// refresh (via pack.js). manualLocationEnabled/manualLat/manualLon/manualLocName,
// pressureHistory, sampleHours, and the clear-history button are JS-only
// keys in Clay's localStorage (the button has no messageKey at all —
// index.js's clayCustom closes the page with a one-shot flag). index.js
// handles showConfiguration/webviewclosed manually (autoHandleEvents:
// false) so it can choose which settings actually reach the watch. It
// forwards sampleHours itself, as SAMPLE_HRS on the weather dict, because
// the watch is what schedules the wakeup.
module.exports = [
  {
    "type": "heading",
    "defaultValue": "AetherCast Settings"
  },
  {
    "type": "section",
    "items": [
      { "type": "heading", "defaultValue": "Units" },
      {
        "type": "radiogroup",
        "messageKey": "UNIT_SYSTEM",
        "label": "Temperature & wind",
        "defaultValue": 0,
        "serializeValueAs": "integer",
        "options": [
          { "label": "Imperial (\u00b0F, mph)", "value": 0 },
          { "label": "Metric (\u00b0C, km/h)", "value": 1 }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      { "type": "heading", "defaultValue": "Location" },
      {
        "type": "toggle",
        "messageKey": "manualLocationEnabled",
        "label": "Use manual location",
        "defaultValue": false
      },
      {
        "type": "input",
        "messageKey": "manualLat",
        "label": "Latitude",
        "attributes": { "type": "number", "step": "0.0001", "placeholder": "e.g. 39.7392" }
      },
      {
        "type": "input",
        "messageKey": "manualLon",
        "label": "Longitude",
        "attributes": { "type": "number", "step": "0.0001", "placeholder": "e.g. -104.9903" }
      },
      {
        "type": "input",
        "messageKey": "manualLocName",
        "label": "Location name (manual mode only)",
        "defaultValue": "",
        "attributes": { "type": "text", "placeholder": "e.g. North Platte" }
      },
      {
        "type": "radiogroup",
        "messageKey": "pressureHistory",
        "label": "Pressure history",
        "defaultValue": 0,
        "serializeValueAs": "integer",
        "options": [
          { "label": "Follow me", "value": 0 },
          { "label": "This location only", "value": 1 }
        ]
      },
      {
        "type": "button",
        "id": "clearLocationHistory",
        "defaultValue": "Clear location history",
        "description": "Deletes the location trail stored on this phone. The next refresh uses only where you are now."
      },
      {
        "type": "radiogroup",
        "messageKey": "sampleHours",
        "label": "Background location",
        "defaultValue": 0,
        "serializeValueAs": "integer",
        "options": [
          { "label": "Off", "value": 0 },
          { "label": "Every hour", "value": 1 },
          { "label": "Every 2 hours", "value": 2 },
          { "label": "Every 4 hours", "value": 4 }
        ],
        "description": "Off by default. When on, the watch briefly opens the app every N hours, records where you are (only if Follow me is on), updates the cache, and closes. It skips Quiet Time and times when the phone is disconnected. Opening the app also resets the timer. On some watches it may interrupt another open app."
      }
    ]
  },
  {
    "type": "text",
    "defaultValue": "Follow me reconstructs the past 24 h from places this phone has been. That trail stays on the phone, and the watch receives weather — not the trail. Open-Meteo receives coordinates for the forecast. BigDataCloud receives coordinates rounded to about 1 km only on a GPS fix more than 2 km from the last named place, never in manual mode. In manual mode the optional location name (or the timezone city) labels the watch. A local place-name cache stays on the phone. Manual location and \"This location only\" use one fixed place and do not record a trail. Background location is the same refresh on a timer: a trail point is stored only when Follow me is on."
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];
