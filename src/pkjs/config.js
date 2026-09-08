// Clay settings schema. See docs/vendor/pebble-app-configuration.md.
//
// UNIT_SYSTEM's messageKey matches a real watch-facing key (package.json /
// message_keys.json) - src/pkjs/index.js forwards it to the watch on every
// refresh (via pack.js). manualLocationEnabled/manualLat/manualLon are
// JS-only: they never go over AppMessage, only into Clay's own localStorage,
// per PLAN.md's "Clay manual lat/lon override" risk mitigation. index.js
// handles showConfiguration/webviewclosed manually (autoHandleEvents: false)
// so it can choose which settings actually reach the watch.
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
      }
    ]
  },
  {
    "type": "text",
    "defaultValue": "Manual location only changes where weather is fetched from - it is never sent anywhere except Open-Meteo."
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];
