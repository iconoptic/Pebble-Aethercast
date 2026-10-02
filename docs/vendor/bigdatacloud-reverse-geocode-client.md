<!--
source: https://www.bigdatacloud.com/docs/article/fair-use-policy-for-free-client-side-reverse-geocoding-api
also: https://www.bigdatacloud.com/free-api/free-reverse-geocode-to-city-api
also: https://www.bigdatacloud.com/docs/article/why-is-reverse-geocoding-api-free
fetched: 2026-10-02T21:53:00Z
-->

# Free Client-side Reverse Geocoding API (BigDataCloud)

## Fair use policy

Source: [Fair Use Policy](https://www.bigdatacloud.com/docs/article/fair-use-policy-for-free-client-side-reverse-geocoding-api)

The free client-side Reverse Geocoding API is designed for client-side use only. Calls that violate this policy may result in your IP address being banned, preventing access to all of BigDataCloud's free APIs.

If your IP is banned you will receive a 402 error. To request a review, contact us via our contact page.

### Fair use policy

1. The API must only be used to convert current geo-coordinates obtained with permission from the calling client device.
2. Pre-stored coordinates, or coordinates belonging to another client or user, must not be passed to this endpoint.
3. Coordinates must reflect the calling device's current, best-known location, retrieved via client-side geolocation services such as GPS or Wi-Fi positioning.
4. For web projects, use the HTML5 Geolocation API. For mobile and IoT devices, use the platform's native location SDK.
5. The API call must originate from the client where the coordinates are retrieved — not from a separate server.
6. You may not retrieve coordinates on the client and transmit them to a server for processing before calling this API. The call must happen on the same client where the coordinates are available.

### If you need server-side access

Use the server-side Reverse Geocoding API instead. It returns the same data, includes 50,000 free queries per month, and requires an API key.

## API overview

Source: [FREE Client Side Reverse Geocoding to City API](https://www.bigdatacloud.com/free-api/free-reverse-geocode-to-city-api)

Endpoint (no API key):

```
GET https://api.bigdatacloud.net/data/reverse-geocode-client
  ?latitude={lat}
  &longitude={lon}
  &localityLanguage=en
```

- Client-side only; free; no API key.
- Must use the device's current location, not pre-stored or third-party coordinates.
- Server-side or batch use must use the authenticated Reverse Geocoding API instead.
- Breach of fair use (e.g. server-side calls) may trigger a temporary IP-level ban with HTTP 402.

### Parameters (relevant)

| Parameter | Required | Description |
|---|---|---|
| `latitude` | Optional | WGS 84, [-90, 90] |
| `longitude` | Optional | WGS 84, [-180, 180] |
| `localityLanguage` | Optional | ISO 639-1 (e.g. `en`); falls back to English / native names |

### Response fields used by AetherCast

- `city` — most significant populated place
- `locality` — finer named locality (suburb / village / town)
- `principalSubdivision` — state / province style name

### Attribution / rate notes

- No attribution requirement is stated on the fair-use or free-API pages fetched above.
- No numeric free-tier rate limit is published for the client endpoint; abuse / non-client use is enforced via IP ban (HTTP 402).
- Why free: anonymous pairing of consented GPS with the requesting IP improves BigDataCloud's IP-geolocation models ([Why is Reverse Geocoding API free?](https://www.bigdatacloud.com/docs/article/why-is-reverse-geocoding-api-free)).

## AetherCast reading (2026-10-02)

PebbleKit JS runs on the phone and calls this endpoint with coordinates from the phone's own geolocation API. That matches rules 1, 3, 4, and 5 (client-side, current device location, native/HTML5 geolocation, call from the same client).

**Allowed:** live GPS / Wi-Fi fixes from the companion phone.

**Not allowed under these terms:** typed / stored / manual coordinates. AetherCast must not call this endpoint in manual-location mode.
