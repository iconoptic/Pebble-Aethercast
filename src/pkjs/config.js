// Clay settings schema. See docs/vendor/pebble-app-configuration.md.
//
// UNIT_SYSTEM's messageKey matches a real watch-facing key (package.json /
// message_keys.json) - src/pkjs/index.js forwards it to the watch on every
// refresh (via pack.js). manualLocationEnabled/manualLat/manualLon,
// pressureHistory, sampleHours, and the clear-history button are JS-only
// keys in Clay's localStorage (the button has no messageKey at all —
// index.js's clayCustom closes the page with a one-shot flag). index.js
// handles showConfiguration/webviewclosed manually (autoHandleEvents:
// false). It forwards sampleHours itself, as SAMPLE_HRS on the weather
// dict, because the watch is what schedules the wakeup.
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
        "description": "Off by default. When on, the watch opens by itself on that interval, records where you are, updates the cache, and closes. It takes the screen while it does this."
      }
    ]
  },
  {
    "type": "text",
    "defaultValue": "Follow me reconstructs the past 24 h from places this phone has been. That trail stays on the phone. Open-Meteo receives coordinates only, the same as a normal refresh, and the watch receives weather — not the trail. Manual location and \"This location only\" use one fixed place and do not record a trail. Background location is the same refresh on a timer: a trail point is stored only when Follow me is on."
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];
