<!--
source: https://developer.repebble.com/guides/events-and-services/buttons/
fetched: 2026-09-07T17:42:27Z
-->

[pebble](/)

- [Tutorials](/tutorials/)
- [Get the SDK](/sdk/)
- [Guides](/guides/)
- [Documentation](/docs/)
- [Examples](/examples/)
- [Community](/community/)
- [Blog](https://repebble.com/blog)
- [More](/more/)

[Privacy](https://repebble.com/privacy/)\
[Cookies](https://repebble.com/terms)

[Publish](https://developer.rePebble.com/dashboard)

### [Guides](/guides/)

- [Table of Contents](/guides/toc/)
- [Alloy](/guides/alloy/)
- [App Resources](/guides/app-resources/)
- [Best Practices](/guides/best-practices/)
- [Communication](/guides/communication/)
- [Debugging](/guides/debugging/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
  - [Accelerometer](/guides/events-and-services/accelerometer/)
  - [Background Worker](/guides/events-and-services/background-worker/)
  - [Backlight](/guides/events-and-services/light/)
  - [Buttons](/guides/events-and-services/buttons/)
  - [Compass](/guides/events-and-services/compass/)
  - [Dictation](/guides/events-and-services/dictation/)
  - [Event Services](/guides/events-and-services/events/)
  - [Heart Rate Monitor](/guides/events-and-services/hrm/)
  - [Pebble Health](/guides/events-and-services/health/)
  - [Persistent Storage](/guides/events-and-services/persistent-storage/)
  - [Speaker](/guides/events-and-services/speaker/)
  - [Touch](/guides/events-and-services/touch/)
  - [Wakeups](/guides/events-and-services/wakeups/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

Listening for Button Clicks Types of Click Events - Single Clicks - Single Repeating Clicks - Multiple Clicks - Long Clicks - Raw Clicks

# Buttons

Button [`Clicks`](/docs/c/User_Interface/Clicks/ "Clicks") are the primary input method on Pebble. All Pebble watches come with the same buttons available, shown in the diagram below for Pebble Time:

![button-layout](/assets/images/guides/sensors-and-input/button-layout.png)

These buttons are used in a logical fashion throughout the system:

- Back - Navigates back one [`Window`](/docs/c/User_Interface/Window/ "Window") until the watchface is reached.

- Up - Navigates to the previous item in a list, or opens the past timeline when pressed from the watchface.

- Select - Opens the app launcher from the watchface, accepts a selected option or list item, or launches the next [`Window`](/docs/c/User_Interface/Window/ "Window").

- Down - Navigates to the next item in a list, or opens the future timeline when pressed from the watchface.

Developers are highly encouraged to follow these patterns when using button clicks in their watchapps, since users will already have an idea of what each button will do to a reasonable degree, thus avoiding the need for lengthy usage instructions for each app. Watchapps that wish to use each button for a specific action should use the [`ActionBarLayer`](/docs/c/User_Interface/Layers/ActionBarLayer/ "ActionBarLayer") or [`ActionMenu`](/docs/c/User_Interface/Window/ActionMenu/ "ActionMenu") to give hints about what each button will do.

## Listening for Button Clicks

Button clicks are received via a subscription to one of the types of button click events listed below. Each [`Window`](/docs/c/User_Interface/Window/ "Window") that wishes to receive button click events must provide a [`ClickConfigProvider`](/docs/c/User_Interface/Clicks/#ClickConfigProvider "ClickConfigProvider") that performs these subscriptions.

The first step is to create the [`ClickConfigProvider`](/docs/c/User_Interface/Clicks/#ClickConfigProvider "ClickConfigProvider") function:

static void click_config_provider(void \*context) { // Subcribe to button click events here }

The second step is to register the [`ClickConfigProvider`](/docs/c/User_Interface/Clicks/#ClickConfigProvider "ClickConfigProvider") with the current [`Window`](/docs/c/User_Interface/Window/ "Window"), typically after [`window_create()`](/docs/c/User_Interface/Window/#window_create "window_create"):

// Use this provider to add button click subscriptions window_set_click_config_provider(window, click_config_provider);

The final step is to write a [`ClickHandler`](/docs/c/User_Interface/Clicks/#ClickHandler "ClickHandler") function for each different type of event subscription required by the watchapp. An example for a single click event is shown below:

static void select_click_handler(ClickRecognizerRef recognizer, void \*context) { // A single click has just occured }

## Types of Click Events

There are five types of button click events that apps subscribe to, enabling virtually any combination of up/down/click events to be utilized in a watchapp. The usage of each of these is explained below:

### Single Clicks

Most apps will use this type of click event, which occurs whenever the button specified is pressed and then immediately released. Use [`window_single_click_subscribe()`](/docs/c/User_Interface/Window/#window_single_click_subscribe "window_single_click_subscribe") from a [`ClickConfigProvider`](/docs/c/User_Interface/Clicks/#ClickConfigProvider "ClickConfigProvider") function, supplying the [`ButtonId`](/docs/c/User_Interface/Clicks/#ButtonId "ButtonId") value for the chosen button and the name of the [`ClickHandler`](/docs/c/User_Interface/Clicks/#ClickHandler "ClickHandler") that will receive the events:

static void click_config_provider(void \*context) { ButtonId id = BUTTON_ID_SELECT; // The Select button window_single_click_subscribe(id, select_click_handler); }

### Single Repeating Clicks

Similar to the single click event, the single repeating click event allows repeating events to be received at a specific interval if the chosen button is held down for a longer period of time. This makes the task of scrolling through many list items or incrementing a value significantly easier for the user, and uses fewer button clicks.

static void click_config_provider(void \*context) { ButtonId id = BUTTON_ID_DOWN; // The Down button uint16_t repeat_interval_ms = 200; // Fire every 200 ms while held down window_single_repeating_click_subscribe(id, repeat_interval_ms, down_repeating_click_handler); }

After an initial press (but not release) of the button `id` subscribed to, the callback will be called repeatedly with an interval of `repeat_interval_ms` until it is then released.

Developers can determine if the button is still held down after the first callback by using [`click_recognizer_is_repeating()`](/docs/c/User_Interface/Clicks/#click_recognizer_is_repeating "click_recognizer_is_repeating"), as well as get the number of callbacks counted so far with [`click_number_of_clicks_counted()`](/docs/c/User_Interface/Clicks/#click_number_of_clicks_counted "click_number_of_clicks_counted"):

static void down_repeating_click_handler(ClickRecognizerRef recognizer, void \*context) { // Is the button still held down? bool is_repeating = click_recognizer_is_repeating(recognizer); // How many callbacks have been recorded so far? uint8_t click_count = click_number_of_clicks_counted(recognizer); }

> Single click and single repeating click subscriptions conflict, and cannot be registered for the same button.

### Multiple Clicks

A multi click event will call the [`ClickHandler`](/docs/c/User_Interface/Clicks/#ClickHandler "ClickHandler") after a specified number of single clicks has been recorded. A good example of usage is to detect a double or triple click gesture:

static void click_config_provider(void \*context) { ButtonId id = BUTTON_ID_SELECT; // The Select button uint8_t min_clicks = 2; // Fire after at least two clicks uint8_t max_clicks = 3; // Don't fire after three clicks uint16_t timeout = 300; // Wait 300ms before firing bool last_click_only = true; // Fire only after the last click window_multi_click_subscribe(id, min_clicks, max_clicks, timeout, last_click_only, multi_select_click_handler); }

Similar to the single repeating click event, the [`ClickRecognizerRef`](/docs/c/User_Interface/Clicks/#ClickRecognizerRef "ClickRecognizerRef") can be used to determine how many clicks triggered this multi click event using [`click_number_of_clicks_counted()`](/docs/c/User_Interface/Clicks/#click_number_of_clicks_counted "click_number_of_clicks_counted").

### Long Clicks

A long click event is fired after a button is held down for the specified amount of time. The event also allows two [`ClickHandler`](/docs/c/User_Interface/Clicks/#ClickHandler "ClickHandler")s to be registered - one for when the button is pressed, and another for when the button is released. Only one of these is required.

static void click_config_provider(void \*context) { ButtonId id = BUTTON_ID_SELECT; // The select button uint16_t delay_ms = 500; // Minimum time pressed to fire window_long_click_subscribe(id, delay_ms, long_down_click_handler, long_up_click_handler); }

### Raw Clicks

The last type of button click subcsription is used to track raw button click events. Like the long click event, two [`ClickHandler`](/docs/c/User_Interface/Clicks/#ClickHandler "ClickHandler")s may be supplied to receive each of the pressed and depressed events.

static void click_config_provider(void \*context) { ButtonId id = BUTTON_ID_SELECT; // The select button window_raw_click_subscribe(id, raw_down_click_handler, raw_up_click_handler, NULL); }

> The last parameter is an optional pointer to a context object to be passed to the callback, and is set to `NULL` if not used.

You need JavaScript enabled to read and post comments.

### Overview

- [Listening for Button Clicks](#listening-for-button-clicks)
- [Types of Click Events](#types-of-click-events)
- [Single Clicks](#single-clicks)
- [Single Repeating Clicks](#single-repeating-clicks)
- [Multiple Clicks](#multiple-clicks)
- [Long Clicks](#long-clicks)
- [Raw Clicks](#raw-clicks)

### Related SDK Docs

- [Clicks](/docs/c/User_Interface/Clicks/)
- [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler)

### Examples

- [App Font Browser](https://github.com/pebble-examples/app-font-browser/blob/master/src/app_font_browser.c#L168)
