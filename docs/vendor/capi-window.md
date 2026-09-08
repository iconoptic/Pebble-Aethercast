<!--
source: https://developer.repebble.com/docs/c/User_Interface/Window/
fetched: 2026-09-07T17:42:40Z
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

### [Pebble C API](/docs/c/)[](#)

- [Pebble C API](/docs/c/)
- [Moddable API (Alloy)](https://www.moddable.com/documentation/readme)
- [PebbleKit JS](/docs/pebblekit-js/)
- [PebbleKit iOS](/docs/pebblekit-ios/)
- [PebbleKit Android](https://github.com/pebble-dev/PebbleKitAndroid2)

- [Foundation](/docs/c/Foundation/)
  - [Alloy](/docs/c/Foundation/Alloy/)
  - [App](/docs/c/Foundation/App/)
  - [App Communication](/docs/c/Foundation/App_Communication/)
  - [App Glance](/docs/c/Foundation/App_Glance/)
  - [AppMessage](/docs/c/Foundation/AppMessage/)
  - [AppSync](/docs/c/Foundation/AppSync/)
  - [AppWorker](/docs/c/Foundation/AppWorker/)
  - [DataLogging](/docs/c/Foundation/DataLogging/)
  - [DataStructures](/docs/c/Foundation/DataStructures/)
    - [UUID](/docs/c/Foundation/DataStructures/UUID/)
  - [Dictation](/docs/c/Foundation/Dictation/)
  - [Dictionary](/docs/c/Foundation/Dictionary/)
  - [Event Service](/docs/c/Foundation/Event_Service/)
    - [AccelerometerService](/docs/c/Foundation/Event_Service/AccelerometerService/)
    - [AlarmService](/docs/c/Foundation/Event_Service/AlarmService/)
    - [AppFocusService](/docs/c/Foundation/Event_Service/AppFocusService/)
    - [BacklightService](/docs/c/Foundation/Event_Service/BacklightService/)
    - [BatteryStateService](/docs/c/Foundation/Event_Service/BatteryStateService/)
    - [CompassService](/docs/c/Foundation/Event_Service/CompassService/)
    - [ConnectionService](/docs/c/Foundation/Event_Service/ConnectionService/)
    - [HealthService](/docs/c/Foundation/Event_Service/HealthService/)
    - [TickTimerService](/docs/c/Foundation/Event_Service/TickTimerService/)
    - [TouchService](/docs/c/Foundation/Event_Service/TouchService/)
  - [Exit Reason](/docs/c/Foundation/Exit_Reason/)
  - [Internationalization](/docs/c/Foundation/Internationalization/)
  - [Launch Reason](/docs/c/Foundation/Launch_Reason/)
  - [Logging](/docs/c/Foundation/Logging/)
  - [Math](/docs/c/Foundation/Math/)
  - [Memory Management](/docs/c/Foundation/Memory_Management/)
  - [Platform](/docs/c/Foundation/Platform/)
  - [Resources](/docs/c/Foundation/Resources/)
    - [File Formats](/docs/c/Foundation/Resources/File_Formats/)
  - [Storage](/docs/c/Foundation/Storage/)
  - [Timer](/docs/c/Foundation/Timer/)
  - [Wakeup](/docs/c/Foundation/Wakeup/)
  - [Wall Time](/docs/c/Foundation/Wall_Time/)
  - [WatchInfo](/docs/c/Foundation/WatchInfo/)
- [Graphics](/docs/c/Graphics/)
  - [Draw Commands](/docs/c/Graphics/Draw_Commands/)
  - [Drawing Paths](/docs/c/Graphics/Drawing_Paths/)
  - [Drawing Primitives](/docs/c/Graphics/Drawing_Primitives/)
  - [Drawing Text](/docs/c/Graphics/Drawing_Text/)
  - [Fonts](/docs/c/Graphics/Fonts/)
  - [Graphics Context](/docs/c/Graphics/Graphics_Context/)
  - [Graphics Types](/docs/c/Graphics/Graphics_Types/)
    - [Color Definitions](/docs/c/Graphics/Graphics_Types/Color_Definitions/)
- [User Interface](/docs/c/User_Interface/)
  - [Animation](/docs/c/User_Interface/Animation/)
    - [PropertyAnimation](/docs/c/User_Interface/Animation/PropertyAnimation/)
  - [Clicks](/docs/c/User_Interface/Clicks/)
  - [Gesture Recognizers](/docs/c/User_Interface/Gesture_Recognizers/)
  - [Layers](/docs/c/User_Interface/Layers/)
    - [ActionBarLayer](/docs/c/User_Interface/Layers/ActionBarLayer/)
    - [BitmapLayer](/docs/c/User_Interface/Layers/BitmapLayer/)
    - [MenuLayer](/docs/c/User_Interface/Layers/MenuLayer/)
    - [RotBitmapLayer](/docs/c/User_Interface/Layers/RotBitmapLayer/)
    - [ScrollLayer](/docs/c/User_Interface/Layers/ScrollLayer/)
    - [SimpleMenuLayer](/docs/c/User_Interface/Layers/SimpleMenuLayer/)
    - [StatusBarLayer](/docs/c/User_Interface/Layers/StatusBarLayer/)
    - [TextLayer](/docs/c/User_Interface/Layers/TextLayer/)
  - [Light](/docs/c/User_Interface/Light/)
  - [Preferences](/docs/c/User_Interface/Preferences/)
  - [Speaker](/docs/c/User_Interface/Speaker/)
  - [UnobstructedArea](/docs/c/User_Interface/UnobstructedArea/)
  - [Vibes](/docs/c/User_Interface/Vibes/)
  - [Window](/docs/c/User_Interface/Window/)
    - [ActionMenu](/docs/c/User_Interface/Window/ActionMenu/)
    - [NumberWindow](/docs/c/User_Interface/Window/NumberWindow/)
  - [Window Stack](/docs/c/User_Interface/Window_Stack/)
- [Standard C](/docs/c/Standard_C/)
  - [Format](/docs/c/Standard_C/Format/)
  - [Locale](/docs/c/Standard_C/Locale/)
  - [Math](/docs/c/Standard_C/Math/)
  - [Memory](/docs/c/Standard_C/Memory/)
  - [String](/docs/c/Standard_C/String/)
  - [Time](/docs/c/Standard_C/Time/)

[](javascript:void(0);)

Jump to item… MODULES ActionMenu NumberWindow FUNCTIONS window_create window_destroy window_set_click_config_provider window_set_click_config_provider_with_context window_get_click_config_provider window_get_click_config_context window_set_window_handlers window_get_root_layer window_set_background_color window_is_loaded window_set_user_data window_get_user_data window_single_click_subscribe window_single_repeating_click_subscribe window_multi_click_subscribe window_long_click_subscribe window_raw_click_subscribe window_set_click_context window_attach_recognizer window_detach_recognizer window_set_touch_bridge_disabled DATA STRUCTURES WindowHandlers TYPEDEFS Window WindowHandler

[](javascript:void(0))

# Window

The basic building block of the user interface

Windows are the top-level elements in the UI hierarchy and the basic building blocks for a Pebble UI. A single window is always displayed at a time on Pebble, with the exception of when animating from one window to the other, which, in that case, is managed by the window stack. You can stack windows on top of each other, but only the topmost window will be visible.

Users wearing a Pebble typically interact with the content and media displayed in a window, clicking and pressing buttons on the watch, depending on what they see and wish to respond to in a window.

Windows serve to display a hierarchy of layers on the screen and handle user input. When a window is visible, its root Layer (and all its child layers) are drawn onto the screen automatically.

You need a window, which always fills the entire screen, to display images, text, and graphics in your Pebble app. A layer by itself doesn’t display on Pebble; it must be in the current window’s layer hierarchy to be visible.

The Window Stack serves as the global manager of what window is presented and makes sure that input events are forwarded to the topmost window.

Refer to the

[User Interface Layers chapter in the Pebble Developer Guides](https://developer.repebble.com/guides/user-interfaces/layers/)

(chapter "Window") for a conceptual overview of Window, the Window Stack and relevant code examples.

## Modules

#### [ActionMenu](/docs/c/User_Interface/Window/ActionMenu/)

 

#### [NumberWindow](/docs/c/User_Interface/Window/NumberWindow/)

A ready-made Window prompting the user to pick a number

## Function Documentation

Window \* window_create(void)

Creates a new Window on the heap and initalizes it with the default values.

- Background color : `GColorWhite`

- Root layer's `update_proc` : function that fills the window's background using `background_color`.

- `click_config_provider` : `NULL`

- `window_handlers` : all `NULL`

#### Returns

A pointer to the window. `NULL` if the window could not be created

void window_destroy(Window \* window)

Destroys a Window previously created by window_create.

void window_set_click_config_provider(Window \* window, [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider) click_config_provider)

Sets the click configuration provider callback function on the window. This will automatically setup the input handlers of the window as well to use the click recognizer subsystem.

#### Parameters

 window  
The window for which to set the click config provider

 click_config_provider  
The callback that will be called to configure the click recognizers with the window

#### See Also

[Clicks](/docs/c/User_Interface/Clicks/)\
[ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider)

void window_set_click_config_provider_with_context(Window \* window, [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider) click_config_provider, void \* context)

Same as [window_set_click_config_provider()](/docs/c/User_Interface/Window/#window_set_click_config_provider), but will assign a custom context pointer (instead of the window pointer) that will be passed into the [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) click event handlers.

#### Parameters

 window  
The window for which to set the click config provider

 click_config_provider  
The callback that will be called to configure the click recognizers with the window

 context  
Pointer to application specific data that will be passed to the click configuration provider callback (defaults to the window).

#### See Also

[Clicks](/docs/c/User_Interface/Clicks/)\
[window_set_click_config_provider](/docs/c/User_Interface/Window/#window_set_click_config_provider)

[ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider) window_get_click_config_provider(const Window \* window)

Gets the current click configuration provider of the window.

#### Parameters

 window  
The window for which to get the click config provider

void \* window_get_click_config_context(Window \* window)

Gets the current click configuration provider context of the window.

#### Parameters

 window  
The window for which to get the click config provider context

void window_set_window_handlers(Window \* window, [WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers) handlers)

Sets the window handlers of the window. These handlers get called e.g. when the user enters or leaves the window.

#### Parameters

 window  
The window for which to set the window handlers

 handlers  
The handlers for the specified window

#### See Also

[WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers)

struct Layer \* window_get_root_layer(const Window \* window)

Gets the root Layer of the window. The root layer is the layer at the bottom of the layer hierarchy for this window. It is the window's "canvas" if you will. By default, the root layer only draws a solid fill with the window's background color.

#### Parameters

 window  
The window for which to get the root layer

#### Returns

The window's root layer

void window_set_background_color(Window \* window, GColor background_color)

Sets the background color of the window, which is drawn automatically by the root layer of the window.

#### Parameters

 window  
The window for which to set the background color

 background_color  
The new background color

#### See Also

[window_get_root_layer()](/docs/c/User_Interface/Window/#window_get_root_layer)

bool window_is_loaded(Window \* window)

Gets whether the window has been loaded. If a window is loaded, its `.load` handler has been called (and the `.unload` handler has not been called since).

#### Parameters

 window  
The window to query its loaded status

#### Returns

true if the window is currently loaded or false if not.

#### See Also

[WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers)

void window_set_user_data(Window \* window, void \* data)

Sets a pointer to developer-supplied data that the window uses, to provide a means to access the data at later times in one of the window event handlers.

#### Parameters

 window  
The window for which to set the user data

 data  
A pointer to user data.

#### See Also

[window_get_user_data](/docs/c/User_Interface/Window/#window_get_user_data)

void \* window_get_user_data(const Window \* window)

Gets the pointer to developer-supplied data that was previously set using [window_set_user_data()](/docs/c/User_Interface/Window/#window_set_user_data).

#### Parameters

 window  
The window for which to get the user data

#### See Also

[window_set_user_data](/docs/c/User_Interface/Window/#window_set_user_data)

void window_single_click_subscribe([ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) button_id, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) handler)

Subscribe to single click events.

##### Notes

Must be called from the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider).

[window_single_click_subscribe()](/docs/c/User_Interface/Window/#window_single_click_subscribe) and [window_single_repeating_click_subscribe()](/docs/c/User_Interface/Window/#window_single_repeating_click_subscribe) conflict, and cannot both be used on the same button.

When there is a multi_click and/or long_click setup, there will be a delay before the single click

#### Parameters

 button_id  
The button events to subscribe to.

 handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire on this event. handler will get fired. On the other hand, when there is no multi_click nor long_click setup, the single click handler will fire directly on button down.

#### See Also

[ButtonId](/docs/c/User_Interface/Clicks/#ButtonId)\
[Clicks](/docs/c/User_Interface/Clicks/)\
[window_single_repeating_click_subscribe](/docs/c/User_Interface/Window/#window_single_repeating_click_subscribe)

void window_single_repeating_click_subscribe([ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) button_id, [uint16_t](/docs/c/Standard_C/#uint16_t) repeat_interval_ms, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) handler)

Subscribe to single click event, with a repeat interval. A single click is detected every time "repeat_interval_ms" has been reached.

##### Notes

Must be called from the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider).

[window_single_click_subscribe()](/docs/c/User_Interface/Window/#window_single_click_subscribe) and [window_single_repeating_click_subscribe()](/docs/c/User_Interface/Window/#window_single_repeating_click_subscribe) conflict, and cannot both be used on the same button.

The back button cannot be overridden with a repeating click.

#### Parameters

 button_id  
The button events to subscribe to.

 repeat_interval_ms  
When holding down, how many milliseconds before the handler is fired again. A value of 0ms means "no repeat timer". The minimum is 30ms, and values below will be disregarded. If there is a long-click handler subscribed on this button, `repeat_interval_ms` will not be used.

 handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire on this event.

#### See Also

[window_single_click_subscribe](/docs/c/User_Interface/Window/#window_single_click_subscribe)

void window_multi_click_subscribe([ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) button_id, uint8_t min_clicks, uint8_t max_clicks, [uint16_t](/docs/c/Standard_C/#uint16_t) timeout, bool last_click_only, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) handler)

Subscribe to multi click events.

##### Note

Must be called from the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider).

#### Parameters

 button_id  
The button events to subscribe to.

 min_clicks  
Minimum number of clicks before handler is fired. Defaults to 2.

 max_clicks  
Maximum number of clicks after which the click counter is reset. A value of 0 means use "min" also as "max".

 timeout  
The delay after which a sequence of clicks is considered finished, and the click counter is reset. A value of 0 means to use the system default 300ms.

 last_click_only  
Defaults to false. When true, only the handler for the last multi-click is called.

 handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire on this event. Fired for multi-clicks, as "filtered" by the `last_click_only`, `min`, and `max` parameters.

void window_long_click_subscribe([ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) button_id, [uint16_t](/docs/c/Standard_C/#uint16_t) delay_ms, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) down_handler, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) up_handler)

Subscribe to long click events.

##### Notes

Must be called from the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider).

The back button cannot be overridden with a long click.

#### Parameters

 button_id  
The button events to subscribe to.

 delay_ms  
Milliseconds after which "handler" is fired. A value of 0 means to use the system default 500ms.

 down_handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire as soon as the button has been held for `delay_ms`. This may be NULL to have no down handler.

 up_handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire on the release of a long click. This may be NULL to have no up handler.

void window_raw_click_subscribe([ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) button_id, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) down_handler, [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) up_handler, void \* context)

Subscribe to raw click events.

##### Notes

Must be called from within the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider).

The back button cannot be overridden with a raw click.

#### Parameters

 button_id  
The button events to subscribe to.

 down_handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire as soon as the button has been pressed. This may be NULL to have no down handler.

 up_handler  
The [ClickHandler](/docs/c/User_Interface/Clicks/#ClickHandler) to fire on the release of the button. This may be NULL to have no up handler.

 context  
If this context is not NULL, it will override the general context.

void window_set_click_context([ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) button_id, void \* context)

Set the context that will be passed to handlers for the given button's events. By default the context passed to handlers is equal to the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider) context (defaults to the window).

##### Note

Must be called from within the [ClickConfigProvider](/docs/c/User_Interface/Clicks/#ClickConfigProvider).

#### Parameters

 button_id  
The button to set the context for.

 context  
Set the context that will be passed to handlers for the given button's events.

void window_attach_recognizer(Window \* window, Recognizer \* recognizer)

Attach a recognizer to the window.

#### Parameters

 window  
[Window](/docs/c/User_Interface/Window/) to which to attach the [Gesture Recognizers](/docs/c/User_Interface/Gesture_Recognizers/)

 recognizer  
[Gesture Recognizers](/docs/c/User_Interface/Gesture_Recognizers/) to attach

void window_detach_recognizer(Window \* window, Recognizer \* recognizer)

Detach a recognizer from the window.

#### Parameters

 window  
[Window](/docs/c/User_Interface/Window/) from which to detach the [Gesture Recognizers](/docs/c/User_Interface/Gesture_Recognizers/)

 recognizer  
[Gesture Recognizers](/docs/c/User_Interface/Gesture_Recognizers/) to detach

void window_set_touch_bridge_disabled(Window \* window, bool disabled)

Disable the system touch-navigation bridge for a window. With the bridge disabled the system recognizer set fails on Touchdown for this window, handing every touch to the recognizers the app attached, so an app can take over touch input instead of the built-in button emulation.

#### Parameters

 window  
[Window](/docs/c/User_Interface/Window/) to configure

 disabled  
true to disable the bridge for this window

## Data Structure Documentation

struct WindowHandlers

[WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers) These handlers are called by the [Window Stack](/docs/c/User_Interface/Window_Stack/) as windows get pushed on / popped. All these handlers use [WindowHandler](/docs/c/User_Interface/Window/#WindowHandler) as their function signature.

#### Data Fields

[WindowHandler](/docs/c/User_Interface/Window/#WindowHandler) load  
Called when the window is pushed to the screen when it's not loaded. This is a good moment to do the layout of the window.

[WindowHandler](/docs/c/User_Interface/Window/#WindowHandler) appear  
Called when the window comes on the screen (again). E.g. when second-top-most window gets revealed (again) after popping the top-most window, but also when the window is pushed for the first time. This is a good moment to start timers related to the window, or reset the UI, etc.

[WindowHandler](/docs/c/User_Interface/Window/#WindowHandler) disappear  
Called when the window leaves the screen, e.g. when another window is pushed, or this window is popped. Good moment to stop timers related to the window.

[WindowHandler](/docs/c/User_Interface/Window/#WindowHandler) unload  
Called when the window is deinited, but could be used in the future to free resources bound to windows that are not on screen.

#### See Also

[window_set_window_handlers()](/docs/c/User_Interface/Window/#window_set_window_handlers)\
[Window Stack](/docs/c/User_Interface/Window_Stack/)

## Typedef Documentation

typedef struct Window Window

typedef void(\* WindowHandler)(struct Window \*window)

Function signature for a handler that deals with transition events of a window.

#### See Also

[WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers)\
[window_set_window_handlers()](/docs/c/User_Interface/Window/#window_set_window_handlers)

[Need some help?](javascript:void(0))

### [Modules](#modules)

- [ActionMenu](/docs/c/User_Interface/Window/ActionMenu/)
- [NumberWindow](/docs/c/User_Interface/Window/NumberWindow/)

### [Functions](#functions)

- [window_create](/docs/c/User_Interface/Window/#window_create)
- [window_destroy](/docs/c/User_Interface/Window/#window_destroy)
- [window_set_click_config_provider](/docs/c/User_Interface/Window/#window_set_click_config_provider)
- [window_set_click_config_provider_with_context](/docs/c/User_Interface/Window/#window_set_click_config_provider_with_context)
- [window_get_click_config_provider](/docs/c/User_Interface/Window/#window_get_click_config_provider)
- [window_get_click_config_context](/docs/c/User_Interface/Window/#window_get_click_config_context)
- [window_set_window_handlers](/docs/c/User_Interface/Window/#window_set_window_handlers)
- [window_get_root_layer](/docs/c/User_Interface/Window/#window_get_root_layer)
- [window_set_background_color](/docs/c/User_Interface/Window/#window_set_background_color)
- [window_is_loaded](/docs/c/User_Interface/Window/#window_is_loaded)
- [window_set_user_data](/docs/c/User_Interface/Window/#window_set_user_data)
- [window_get_user_data](/docs/c/User_Interface/Window/#window_get_user_data)
- [window_single_click_subscribe](/docs/c/User_Interface/Window/#window_single_click_subscribe)
- [window_single_repeating_click_subscribe](/docs/c/User_Interface/Window/#window_single_repeating_click_subscribe)
- [window_multi_click_subscribe](/docs/c/User_Interface/Window/#window_multi_click_subscribe)
- [window_long_click_subscribe](/docs/c/User_Interface/Window/#window_long_click_subscribe)
- [window_raw_click_subscribe](/docs/c/User_Interface/Window/#window_raw_click_subscribe)
- [window_set_click_context](/docs/c/User_Interface/Window/#window_set_click_context)
- [window_attach_recognizer](/docs/c/User_Interface/Window/#window_attach_recognizer)
- [window_detach_recognizer](/docs/c/User_Interface/Window/#window_detach_recognizer)
- [window_set_touch_bridge_disabled](/docs/c/User_Interface/Window/#window_set_touch_bridge_disabled)

### [Data Structures](#structs)

- [WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers)

### [Typedefs](#typedefs)

- [Window](/docs/c/User_Interface/Window/#Window)
- [WindowHandler](/docs/c/User_Interface/Window/#WindowHandler)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
