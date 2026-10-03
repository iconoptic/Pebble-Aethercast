<!--
source: https://developer.repebble.com/docs/c/Foundation/Launch_Reason/
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

Jump to item… FUNCTIONS launch_reason launch_get_args launch_button launch_get_quick_launch_action ENUMS AppLaunchReason AppQuickLaunchAction

[](javascript:void(0))

# Launch Reason

API for checking what caused the application to launch.

This includes the system, launch by user interaction (User selects the application from the launcher menu), launch by the mobile or a mobile companion application, or launch by a scheduled wakeup event for the specified application.

## Function Documentation

[AppLaunchReason](/docs/c/Foundation/Launch_Reason/#AppLaunchReason) launch_reason(void)

Provides the method used to launch the current application.

#### Returns

The method or reason the current application was launched

[uint32_t](/docs/c/Standard_C/#uint32_t) launch_get_args(void)

Get the argument passed to the app when it was launched.

##### Note

Currently the only way to pass arguments to apps is by using an openWatchApp action on a pin.

#### Returns

The argument passed to the app, or 0 if the app wasn't launched from a Launch App action

[ButtonId](/docs/c/User_Interface/Clicks/#ButtonId) launch_button(void)

Get the button id used to launch the app. Only valid if the launch reason is APP_LAUNCH_USER or APP_LAUNCH_QUICK_LAUNCH.

[AppQuickLaunchAction](/docs/c/Foundation/Launch_Reason/#AppQuickLaunchAction) launch_get_quick_launch_action(void)

Get the action that was used to quick launch the app.

#### Returns

The [AppQuickLaunchAction](/docs/c/Foundation/Launch_Reason/#AppQuickLaunchAction) used to launch the app, or APP_QUICK_LAUNCH_ACTION_NONE if the app was not launched via Quick Launch.

## Enum Documentation

enum AppLaunchReason

[AppLaunchReason](/docs/c/Foundation/Launch_Reason/#AppLaunchReason) is used to inform the application about how it was launched.

New launch reasons may be added in the future. As a best practice, it is recommended to only handle the cases that the app needs to know about, rather than trying to handle all possible launch reasons.

#### Enumerators

APP_LAUNCH_SYSTEM  
App launched by the system.

APP_LAUNCH_USER  
App launched by user selection in launcher menu.

APP_LAUNCH_PHONE  
App launched by mobile or companion app.

APP_LAUNCH_WAKEUP  
App launched by wakeup event.

APP_LAUNCH_WORKER  
App launched by worker calling [worker_launch_app()](/docs/c/Worker/#worker_launch_app)

APP_LAUNCH_QUICK_LAUNCH  
App launched by user using quick launch.

APP_LAUNCH_TIMELINE_ACTION  
App launched by user opening it from a pin.

APP_LAUNCH_SMARTSTRAP  
App launched by a smartstrap.

enum AppQuickLaunchAction

Details about how an app was quick launched. Returned by app_launch_get_quick_launch_action.

#### Enumerators

APP_QUICK_LAUNCH_ACTION_NONE  
App was not launched via Quick Launch.

APP_QUICK_LAUNCH_ACTION_HOLD  
User held a single button.

APP_QUICK_LAUNCH_ACTION_TAP  
User tapped a button (single click)

APP_QUICK_LAUNCH_ACTION_COMBO  
User held a button combination.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [launch_reason](/docs/c/Foundation/Launch_Reason/#launch_reason)
- [launch_get_args](/docs/c/Foundation/Launch_Reason/#launch_get_args)
- [launch_button](/docs/c/Foundation/Launch_Reason/#launch_button)
- [launch_get_quick_launch_action](/docs/c/Foundation/Launch_Reason/#launch_get_quick_launch_action)

### [Enums](#enums)

- [AppLaunchReason](/docs/c/Foundation/Launch_Reason/#AppLaunchReason)
- [AppQuickLaunchAction](/docs/c/Foundation/Launch_Reason/#AppQuickLaunchAction)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
