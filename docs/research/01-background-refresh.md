# Background location sampling

Checked **2026-10-02**. The question is how a closed AetherCast can record a
geolocation fix often enough for the Part A trail to follow a real day, without
a server and without the user opening the app.

The trail lives in PebbleKit JS `localStorage`. A fix can only be recorded by
code that can call `navigator.geolocation` and then Open-Meteo. That code runs
on the phone, and only while this watchapp's JS session is alive.

## What was read

| Source | Fetched | What it settles |
|---|---|---|
| [pebble-comm-pebblekit-js.md](../vendor/pebble-comm-pebblekit-js.md) | 2026-09-07 | When PKJS runs and stops |
| [pebble-wakeups.md](../vendor/pebble-wakeups.md) | 2026-09-07 | How to schedule a future launch |
| [capi-wakeup.md](../vendor/capi-wakeup.md) | 2026-10-02 | Caps, the 1-minute window, `notify_if_missed` |
| [capi-launch-reason.md](../vendor/capi-launch-reason.md) | 2026-10-02 | `APP_LAUNCH_WAKEUP` / `APP_LAUNCH_WORKER` / `APP_LAUNCH_PHONE` |
| [pebble-background-worker.md](../vendor/pebble-background-worker.md) | 2026-10-02 | What a worker cannot do |
| [capi-app-focus.md](../vendor/capi-app-focus.md) | 2026-10-02 | A notification covers the app |
| [pebble-conserving-battery.md](../vendor/pebble-conserving-battery.md) | 2026-09-07 | What actually costs battery |
| [Core Devices mobile app README](https://github.com/coredevices/mobileapp) | live 2026-10-02 | Where PKJS runs |
| [CrimsonBear forum thread](https://forum.repebble.com/t/crimsonbear-glucose-data-on-pebble-pebblekit-js-timers-suspended-on-ios/1557) | 4 Sep 2026 | iOS suspends PKJS timers |
| [Notifications forum thread](https://forum.repebble.com/t/can-you-pop-app-up-above-notifications-without-dismissing-notifications/730) | 24 May 2026 | An app launch sits under a notification |
| [Pebble JS tips, 2013](https://developer.rebble.io/blog/2013/12/20/Pebble-Javascript-Tips-and-Tricks/) | republished | Do not poll from JS timers |

`tools/fetch-docs.sh` lists the vendor pages so a later refresh keeps them.

## PebbleKit JS does not run in the background

The PKJS guide:

> Code in this file will be executed when the associated watchapp is launched,
> and will stop once that app exits.

The `ready` handler must return within a few seconds or the phone kills it.
Geolocation and `XMLHttpRequest` exist only in that session.

The Core Devices phone app is the process that hosts it. Its README
(fetched 2026-10-02 from `master`):

> **PebbleKit JS** — watchapps can include a JavaScript component that runs
> on the *phone*, inside this app (`js/`), giving watchapps network access
> and configuration UIs.

Nothing there says JS keeps running for a watchapp that is not in the
foreground. The official lifecycle is the opposite: launch starts it, exit
stops it.

A long-lived JS timer is also the wrong tool when the session does exist.
On 4 Sep 2026 @dabear reported CrimsonBear (a CGM watchface) on iOS with the
Pebble app backgrounded: a `setTimeout` fired 259 s late and a `setInterval`
fired 477 s late, both resuming together, with no fetch or companion-restart
error. Developer Connection (phone app effectively awake) fired the same
timers within 3–8 ms. That is iOS suspending the JS runtime, not a bug in
one timer. Pebble's own 2013 guidance already said not to use `setInterval`,
and to schedule on the watch and message the phone so a dead JS environment
gets re-initialized to handle the message.

`APP_LAUNCH_PHONE` is a launch performed by the mobile app. We do not control
the Core Devices app, so we cannot ask it to wake us on a timer.

## The background worker cannot take the sample

A worker runs while the foreground app is closed, in 10.5 kB, and there is
only one worker on the watch at a time. Starting a second one asks the user
for permission to replace whatever is already running. The user can also see
and kill it under Settings → Background App.

The available-APIs section is blunt: workers have no UI, **cannot use
AppMessage**, and cannot load resources. They can use sensors, storage,
DataLogging, and `worker_launch_app()`.

No AppMessage means no PKJS, so no geolocation and no HTTPS. DataLogging
delivers bytes to a native phone companion, which AetherCast does not have
and is not going to grow (the task rules out a cloud component, and a native
companion would be a second app). `worker_launch_app()` starts the
**foreground** app (`APP_LAUNCH_WORKER` in the launch-reason enum). The
guide says not to build a background timer this way; use Wakeup. A worker
whose only job is to call `worker_launch_app()` would spend the single
system-wide worker slot to do a worse wakeup.

## Wakeup is a foreground launch

`wakeup_schedule(timestamp, cookie, notify_if_missed)` asks the system to
launch this app later, including if it is closed.

Limits, from the guide and the C API (they disagree slightly on the near
window; both are quoted):

| Rule | Source |
|---|---|
| At most 8 scheduled events per app | both |
| Cannot schedule within 30 seconds of now | guide |
| A 1-minute window either side of a scheduled wakeup: no app may schedule another inside it | C API: "no application may schedule a wakeup event with 1 minute of a currently scheduled wakeup event" |
| `E_RANGE` (−8) another event owns that minute; `E_INVALID_ARGUMENT` (−4) time is in the past; `E_OUT_OF_RESOURCES` (−7) 8 already scheduled; `E_INTERNAL` (−3) | both |
| `notify_if_missed`: on power-on, alert the user if the wakeup was missed because the watch was off | C API |

The handler passed to `wakeup_service_subscribe` runs only if the app is
**already** running. A launch of a closed app is `launch_reason() ==
APP_LAUNCH_WAKEUP`, and `wakeup_get_launch_event` returns the id and cookie.

### Does it take the screen?

Yes. `APP_LAUNCH_WAKEUP` is an app launch, the same kind of launch as the
user picking the app. The watch has one foreground app. Scheduling a wakeup
does not register a headless callback.

What is on screen at the time:

- **Another app.** The wakeup launches us. The wakeup guide does not say the
  previous app is resumed when we exit. A 24 May 2026 forum report about a
  *phone-triggered* launch (so `APP_LAUNCH_PHONE`, not a wakeup) says that
  author could return the user to the previous screen after closing, except
  in the notification case below. We do not depend on that. We pop our own
  window and leave the system to show whatever it shows next.
- **A notification.** Official `AppFocusService` docs: a notification is a
  modal window that covers the app. The app is running but not visible until
  the notification is dismissed. The 24 May 2026 report matches that for a
  phone-triggered launch: the app starts *under* the notification, and the
  user has to dismiss the notification to see it. The wakeup pages do not
  mention notifications at all. We treat "covered by a notification" as
  unspecified for wakeups and follow the focus-service model: keep running
  the fetch, and exit when it finishes, without waiting to be uncovered.
  Waiting would leave the process up until the user deals with the
  notification.
- **The watch is off.** `notify_if_missed` posts an alert at next power-on.
  A location sample the user slept through does not deserve an alert, so
  AetherCast passes `false`. The missed event is gone; the next manual
  launch re-arms.

There is no API to launch, sample, and exit without being an app. The
mitigation is to be a very short-lived app.

## Battery

The conserving-battery guide names the costs that matter here: keeping the
watch awake, talking on Bluetooth (`AppMessage` lifts the link out of the
low-power sniff interval until a while after the last message), the
backlight, and sensors. A wakeup sample is one of those bursts — screen on,
one request, one reply, then exit — repeated at the interval the user chose.
It is not a worker spinning for hours, and it is not a JS timer holding the
phone radio open between samples.

At one hour, that is at most 24 bursts a day. Each burst is one Open-Meteo
HTTP request per distinct trail place, capped at 6, and usually 1 because
resolved past hours are cached (Part A). 24 × 6 = 144 calls/day against the
free tier's 10 000. The phone pays a GPS fix and one HTTPS call per burst.
There is no measured milliamp figure in the vendor docs; the guide's point
is that the expensive part is the radio and the awake interval, so the
sample has to end.

## Decision

Nothing quieter can record the fix:

- A worker cannot reach PKJS.
- PKJS is not running while the watchapp is closed, and on iOS its timers
  do not fire on schedule even while a watchface is open and the phone app
  is backgrounded.
- We cannot change the Core Devices app.

The implementation is the opt-in wakeup the task describes, default **off**:

- Clay "Background location": Off / Every hour / Every 2 hours / Every 4 hours.
- The watch stores the interval outside `WeatherPayload` (`PERSIST_KEY` 3).
  `SAMPLE_HRS` rides every success and error dict. Absent means "older phone
  build, leave the stored interval alone"; `0` means off. `SCHEMA` stays 3.
- Every launch, wakeup or normal, cancels our wakeup and arms
  `now + interval` (opening the app resets the timer). `E_RANGE` retries up
  to five times, 90 s later each time, which clears the one-minute exclusion
  window. Inbox repeats of the same `SAMPLE_HRS` do not rewrite flash or
  reschedule when a wakeup is already pending (`wakeup_query` on key 4).
- `APP_LAUNCH_WAKEUP` pushes a one-line "Sampling / location" window instead
  of the dashboard, sends the usual `REQUEST`, and exits when the inbox
  lands or the 30 s watchdog fires. A 45 s deadline exits if the phone never
  answers at all, so a covered or half-connected launch cannot sit there.
  Quiet Time (`quiet_time_is_active`, when the SDK has it) or a disconnected
  phone (`connection_service_peek_pebble_app_connection`) aborts before the
  sample window: re-arm and exit with no REQUEST.
- A wakeup that fires while the user already has the app open refreshes in
  place and does not exit.
- `notify_if_missed` is false.

A trail point is written only when Follow me is on. When the trail is
inactive (manual location or "This location only"), JS sends `SAMPLE_HRS = 0`
so the watch cancels its wakeup — there is nothing useful to sample.

### What this does not fix

A wakeup still takes the screen for the length of one refresh. Geolocation's
guard is 20 s and the watch watchdog is 30 s, so a phone that is slow or
asleep can leave "Sampling" up for most of that, or for the 45 s backstop if
JS never starts. iOS may be slow to bring the Core Devices app back for the
Bluetooth session; if it does not answer, the watch exits with no new fix
and tries again next interval. That is the same dependency as opening the
app normally with the phone in a pocket, plus a screen the user did not ask
to see at that moment. It is off unless they opt in.
