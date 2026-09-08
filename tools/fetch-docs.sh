#!/usr/bin/env bash
# Mirrors the upstream reference docs this project depends on into docs/vendor/
# as offline Markdown. Re-run to refresh; output is git-ignored-by-choice.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="$ROOT/docs/vendor"
mkdir -p "$OUT"

# name|url
PAGES=(
  "pebble-sdk-install|https://developer.repebble.com/sdk/"
  "pebble-hardware-information|https://developer.repebble.com/guides/tools-and-resources/hardware-information/"
  "pebble-building-for-every-pebble|https://developer.repebble.com/guides/best-practices/building-for-every-pebble/"
  "pebble-app-metadata|https://developer.repebble.com/guides/tools-and-resources/app-metadata/"
  "pebble-cli-tool|https://developer.repebble.com/guides/tools-and-resources/pebble-tool/"
  "pebble-developer-connection|https://developer.repebble.com/guides/tools-and-resources/developer-connection/"
  "pebble-comm-advanced|https://developer.repebble.com/guides/communication/advanced-communication/"
  "pebble-comm-pebblekit-js|https://developer.repebble.com/guides/communication/using-pebblekit-js/"
  "pebble-comm-sending-receiving|https://developer.repebble.com/guides/communication/sending-and-receiving-data/"
  "pebble-persistent-storage|https://developer.repebble.com/guides/events-and-services/persistent-storage/"
  "pebble-touch|https://developer.repebble.com/guides/events-and-services/touch/"
  "pebble-buttons|https://developer.repebble.com/guides/events-and-services/buttons/"
  "pebble-wakeups|https://developer.repebble.com/guides/events-and-services/wakeups/"
  "pebble-graphics-drawing-primitives|https://developer.repebble.com/guides/graphics-and-animations/drawing-primitives-images-and-text/"
  "pebble-graphics-framebuffer|https://developer.repebble.com/guides/graphics-and-animations/framebuffer-graphics/"
  "pebble-graphics-vector|https://developer.repebble.com/guides/graphics-and-animations/vector-graphics/"
  "pebble-graphics-animations|https://developer.repebble.com/guides/graphics-and-animations/animations/"
  "pebble-ui-layers|https://developer.repebble.com/guides/user-interfaces/layers/"
  "pebble-ui-content-size|https://developer.repebble.com/guides/user-interfaces/content-size/"
  "pebble-ui-appglance-c|https://developer.repebble.com/guides/user-interfaces/appglance-c/"
  "pebble-ui-unobstructed-area|https://developer.repebble.com/guides/user-interfaces/unobstructed-area/"
  "pebble-app-configuration|https://developer.repebble.com/guides/user-interfaces/app-configuration/"
  "pebble-app-resources|https://developer.repebble.com/guides/app-resources/"
  "pebble-app-resources-platform-specific|https://developer.repebble.com/guides/app-resources/platform-specific/"
  "pebble-conserving-battery|https://developer.repebble.com/guides/best-practices/conserving-battery-life/"
  "pebble-modular-app-architecture|https://developer.repebble.com/guides/best-practices/modular-app-architecture/"
  "pebble-debugging|https://developer.repebble.com/guides/debugging/"
  "pebble-design-and-interaction|https://developer.repebble.com/guides/design-and-interaction/"
  "pebble-tutorial-web-content|https://developer.repebble.com/tutorials/watchface-tutorial/part3/"
  "capi-appmessage|https://developer.repebble.com/docs/c/Foundation/AppMessage/"
  "capi-storage|https://developer.repebble.com/docs/c/Foundation/Storage/"
  "capi-graphics-context|https://developer.repebble.com/docs/c/Graphics/Graphics_Context/"
  "capi-graphics-types|https://developer.repebble.com/docs/c/Graphics/Graphics_Types/"
  "capi-graphics-path|https://developer.repebble.com/docs/c/Graphics/Draw_Commands/"
  "capi-layers|https://developer.repebble.com/docs/c/User_Interface/Layers/"
  "capi-window|https://developer.repebble.com/docs/c/User_Interface/Window/"
  "capi-window-stack|https://developer.repebble.com/docs/c/User_Interface/Window_Stack/"
  "capi-ticktimer|https://developer.repebble.com/docs/c/Foundation/Event_Service/TickTimerService/"
  "capi-connection-service|https://developer.repebble.com/docs/c/Foundation/Event_Service/ConnectionService/"
  "openmeteo-forecast-api|https://open-meteo.com/en/docs"
  "openmeteo-geocoding-api|https://open-meteo.com/en/docs/geocoding-api"
  "openmeteo-terms|https://open-meteo.com/en/terms"
)

for entry in "${PAGES[@]}"; do
  name="${entry%%|*}"
  url="${entry#*|}"
  printf '  -> %-42s %s\n' "$name" "$url"
  if curl -fsSL --retry 2 --max-time 45 -A 'aethercast-docs-fetch/1.0' "$url" -o "$OUT/.tmp.html"; then
    {
      printf '<!--\nsource: %s\nfetched: %s\n-->\n\n' "$url" "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
      pandoc -f html -t gfm-raw_html --wrap=none "$OUT/.tmp.html" 2>/dev/null || echo '(pandoc conversion failed)'
    } > "$OUT/$name.md"
  else
    echo "     !! fetch failed: $url" >&2
  fi
done
rm -f "$OUT/.tmp.html"

echo
echo "Wrote $(ls -1 "$OUT"/*.md 2>/dev/null | wc -l) documents to $OUT"
