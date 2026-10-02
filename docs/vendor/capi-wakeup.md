<!--
source: https://developer.repebble.com/docs/c/Foundation/Wakeup/
fetched: 2026-10-02T07:38:14Z
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

[Privacy](https://repebble.com/privacy/)  
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

Jump to item… FUNCTIONS wakeup_service_subscribe wakeup_schedule wakeup_cancel wakeup_cancel_all wakeup_get_launch_event wakeup_query TYPEDEFS WakeupId WakeupHandler

[](javascript:void(0))

# Wakeup

Allows applications to schedule to be launched even if they are not running.

## Function Documentation

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void wakeup_service_subscribe([WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler) handler)

Registers a [WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler) to be called when wakeup events occur.

#### Parameters

 handler  
The callback that gets called when the wakeup event occurs

void wakeup_service_subscribe([WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler) handler)

Registers a [WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler) to be called when wakeup events occur.

#### Parameters

 handler  
The callback that gets called when the wakeup event occurs

void wakeup_service_subscribe([WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler) handler)

Registers a [WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler) to be called when wakeup events occur.

##### Note

The handler is only called for wakeup events which occur while the app is already running; use `launch_reason()` === APP_LAUNCH_WAKEUP to detect when the app was launched by a wakeup event.

#### Parameters

 handler  
The callback that gets called when the wakeup event occurs

[WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) wakeup_schedule([time_t](/docs/c/Standard_C/Time/#time_t) timestamp, int32_t cookie, bool notify_if_missed)

Registers a wakeup event that triggers a callback at the specified time. Applications may only schedule up to 8 wakeup events. Wakeup events are given a 1 minute duration window, in that no application may schedule a wakeup event with 1 minute of a currently scheduled wakeup event.

#### Parameters

 timestamp  
The requested time (UTC) for the wakeup event to occur

 cookie  
The application specific reason for the wakeup event

 notify_if_missed  
On powering on Pebble, will alert user when notifications were missed due to Pebble being off.

#### Returns

negative values indicate errors ([StatusCode](/docs/c/Foundation/Storage/#StatusCode)) E_RANGE if the event cannot be scheduled due to another event in that period. E_INVALID_ARGUMENT if the time requested is in the past. E_OUT_OF_RESOURCES if the application has already scheduled all 8 wakeup events. E_INTERNAL if a system error occurred during scheduling.

void wakeup_cancel([WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) wakeup_id)

Cancels a wakeup event.

#### Parameters

 wakeup_id  
Wakeup event to cancel

void wakeup_cancel_all(void)

Cancels all wakeup event for the app.

bool wakeup_get_launch_event([WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) \* wakeup_id, int32_t \* cookie)

Retrieves the wakeup event info for an app that was launched by a wakeup_event (ie. `launch_reason()` === APP_LAUNCH_WAKEUP) so that an app may display information regarding the wakeup event.

#### Parameters

 wakeup_id  
[WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) for the wakeup event that caused the app to wakeup

 cookie  
App provided reason for the wakeup event

#### Returns

True if app was launched due to a wakeup event, false otherwise

bool wakeup_query([WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) wakeup_id, [time_t](/docs/c/Standard_C/Time/#time_t) \* timestamp)

Checks if the current [WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) is still scheduled and therefore valid.

#### Parameters

 wakeup_id  
Wakeup event to query for validity and scheduled time

 timestamp  
Optionally points to an address of a [time_t](/docs/c/Standard_C/Time/#time_t) variable to store the time that the wakeup event is scheduled to occur. (The time is in UTC, but local time when [clock_is_timezone_set](/docs/c/Foundation/Wall_Time/#clock_is_timezone_set) returns false). You may pass NULL instead if you do not need it.

#### Returns

True if [WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) is still scheduled, false if it doesn't exist or has already occurred

## Typedef Documentation

typedef int32_t WakeupId

[WakeupId](/docs/c/Foundation/Wakeup/#WakeupId) is an identifier for a wakeup event.

typedef void(\* WakeupHandler)(WakeupId wakeup_id, int32_t cookie)

The type of function which can be called when a wakeup event occurs.  
The arguments will be the id of the wakeup event that occurred, as well as the scheduled cookie provided to `wakeup_schedule`.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [wakeup_service_subscribe](/docs/c/Foundation/Wakeup/#wakeup_service_subscribe)
- [wakeup_schedule](/docs/c/Foundation/Wakeup/#wakeup_schedule)
- [wakeup_cancel](/docs/c/Foundation/Wakeup/#wakeup_cancel)
- [wakeup_cancel_all](/docs/c/Foundation/Wakeup/#wakeup_cancel_all)
- [wakeup_get_launch_event](/docs/c/Foundation/Wakeup/#wakeup_get_launch_event)
- [wakeup_query](/docs/c/Foundation/Wakeup/#wakeup_query)

### [Typedefs](#typedefs)

- [WakeupId](/docs/c/Foundation/Wakeup/#WakeupId)
- [WakeupHandler](/docs/c/Foundation/Wakeup/#WakeupHandler)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
