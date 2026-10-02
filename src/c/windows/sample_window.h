#pragma once

// One-line window used only for APP_LAUNCH_WAKEUP. Pops itself (and so
// exits the app) when the phone's reply lands, the 30 s request watchdog
// fires, or a backstop deadline elapses because the phone never answered.
void sample_window_push(void);
