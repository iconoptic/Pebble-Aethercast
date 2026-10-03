# Vendored documentation

Offline Markdown mirrors of the upstream docs this project depends on, so the plan
can be checked against a fixed snapshot rather than a moving website.

Fetched **2026-09-05**. Regenerate with:

```bash
./tools/fetch-docs.sh
```

Each file carries its source URL and fetch timestamp in an HTML comment at the top.
Converted from HTML with `pandoc -f html -t gfm-raw_html`, so code blocks are
flattened to single lines — good enough to read and grep, not to copy verbatim.

| File | Source |
|---|---|
| [capi-appmessage](capi-appmessage.md) | <https://developer.repebble.com/docs/c/Foundation/AppMessage/> |
| [capi-connection-service](capi-connection-service.md) | <https://developer.repebble.com/docs/c/Foundation/Event_Service/ConnectionService/> |
| [capi-graphics-context](capi-graphics-context.md) | <https://developer.repebble.com/docs/c/Graphics/Graphics_Context/> |
| [capi-graphics-path](capi-graphics-path.md) | <https://developer.repebble.com/docs/c/Graphics/Draw_Commands/> |
| [capi-graphics-types](capi-graphics-types.md) | <https://developer.repebble.com/docs/c/Graphics/Graphics_Types/> |
| [capi-layers](capi-layers.md) | <https://developer.repebble.com/docs/c/User_Interface/Layers/> |
| [capi-storage](capi-storage.md) | <https://developer.repebble.com/docs/c/Foundation/Storage/> |
| [capi-ticktimer](capi-ticktimer.md) | <https://developer.repebble.com/docs/c/Foundation/Event_Service/TickTimerService/> |
| [capi-window](capi-window.md) | <https://developer.repebble.com/docs/c/User_Interface/Window/> |
| [openmeteo-forecast-api](openmeteo-forecast-api.md) | <https://open-meteo.com/en/docs> |
| [openmeteo-geocoding-api](openmeteo-geocoding-api.md) | <https://open-meteo.com/en/docs/geocoding-api> |
| [openmeteo-terms](openmeteo-terms.md) | <https://open-meteo.com/en/terms> |
| [bigdatacloud-reverse-geocode-client](bigdatacloud-reverse-geocode-client.md) | <https://www.bigdatacloud.com/docs/article/fair-use-policy-for-free-client-side-reverse-geocoding-api> |
| [pebble-app-configuration](pebble-app-configuration.md) | <https://developer.repebble.com/guides/user-interfaces/app-configuration/> |
| [pebble-app-metadata](pebble-app-metadata.md) | <https://developer.repebble.com/guides/tools-and-resources/app-metadata/> |
| [pebble-app-resources](pebble-app-resources.md) | <https://developer.repebble.com/guides/app-resources/> |
| [pebble-app-resources-platform-specific](pebble-app-resources-platform-specific.md) | <https://developer.repebble.com/guides/app-resources/platform-specific/> |
| [pebble-building-for-every-pebble](pebble-building-for-every-pebble.md) | <https://developer.repebble.com/guides/best-practices/building-for-every-pebble/> |
| [pebble-buttons](pebble-buttons.md) | <https://developer.repebble.com/guides/events-and-services/buttons/> |
| [pebble-cli-tool](pebble-cli-tool.md) | <https://developer.repebble.com/guides/tools-and-resources/pebble-tool/> |
| [pebble-comm-advanced](pebble-comm-advanced.md) | <https://developer.repebble.com/guides/communication/advanced-communication/> |
| [pebble-comm-pebblekit-js](pebble-comm-pebblekit-js.md) | <https://developer.repebble.com/guides/communication/using-pebblekit-js/> |
| [pebble-comm-sending-receiving](pebble-comm-sending-receiving.md) | <https://developer.repebble.com/guides/communication/sending-and-receiving-data/> |
| [pebble-conserving-battery](pebble-conserving-battery.md) | <https://developer.repebble.com/guides/best-practices/conserving-battery-life/> |
| [pebble-debugging](pebble-debugging.md) | <https://developer.repebble.com/guides/debugging/> |
| [pebble-design-and-interaction](pebble-design-and-interaction.md) | <https://developer.repebble.com/guides/design-and-interaction/> |
| [pebble-developer-connection](pebble-developer-connection.md) | <https://developer.repebble.com/guides/tools-and-resources/developer-connection/> |
| [pebble-graphics-animations](pebble-graphics-animations.md) | <https://developer.repebble.com/guides/graphics-and-animations/animations/> |
| [pebble-graphics-drawing-primitives](pebble-graphics-drawing-primitives.md) | <https://developer.repebble.com/guides/graphics-and-animations/drawing-primitives-images-and-text/> |
| [pebble-graphics-framebuffer](pebble-graphics-framebuffer.md) | <https://developer.repebble.com/guides/graphics-and-animations/framebuffer-graphics/> |
| [pebble-graphics-vector](pebble-graphics-vector.md) | <https://developer.repebble.com/guides/graphics-and-animations/vector-graphics/> |
| [pebble-hardware-information](pebble-hardware-information.md) | <https://developer.repebble.com/guides/tools-and-resources/hardware-information/> |
| [pebble-modular-app-architecture](pebble-modular-app-architecture.md) | <https://developer.repebble.com/guides/best-practices/modular-app-architecture/> |
| [pebble-persistent-storage](pebble-persistent-storage.md) | <https://developer.repebble.com/guides/events-and-services/persistent-storage/> |
| [pebble-sdk-install](pebble-sdk-install.md) | <https://developer.repebble.com/sdk/> |
| [pebble-touch](pebble-touch.md) | <https://developer.repebble.com/guides/events-and-services/touch/> |
| [pebble-tutorial-web-content](pebble-tutorial-web-content.md) | <https://developer.repebble.com/tutorials/watchface-tutorial/part3/> |
| [pebble-ui-appglance-c](pebble-ui-appglance-c.md) | <https://developer.repebble.com/guides/user-interfaces/appglance-c/> |
| [pebble-ui-content-size](pebble-ui-content-size.md) | <https://developer.repebble.com/guides/user-interfaces/content-size/> |
| [pebble-ui-layers](pebble-ui-layers.md) | <https://developer.repebble.com/guides/user-interfaces/layers/> |
| [pebble-ui-unobstructed-area](pebble-ui-unobstructed-area.md) | <https://developer.repebble.com/guides/user-interfaces/unobstructed-area/> |
| [pebble-wakeups](pebble-wakeups.md) | <https://developer.repebble.com/guides/events-and-services/wakeups/> |

## Not mirrored here

- `https://ndocs.repebble.com/` — the PebbleOS and mobile-app changelogs live in a
  Notion-backed site that does not convert cleanly. Check it manually for
  firmware-level changes.
- `https://developer.repebble.com/docs/c/` full API reference — only the nine pages
  this project touches are mirrored. Add more to `tools/fetch-docs.sh` as needed.

## Reference code

`../reference/` holds shallow clones, not mirrors:

- `coredevices-example-apps/` — official examples including `touch-thing`, which is
  the most current example of a PT2-era `package.json` and `wscript`
- `pebblekit-js-weather/` — the canonical PebbleKit JS weather example
