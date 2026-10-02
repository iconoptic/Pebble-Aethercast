<!--
source: https://developer.repebble.com/docs/c/Foundation/Event_Service/AppFocusService/
fetched: 2026-10-02T07:44:21Z
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

Jump to item… FUNCTIONS app_focus_service_subscribe_handlers app_focus_service_subscribe app_focus_service_unsubscribe DATA STRUCTURES AppFocusHandlers TYPEDEFS AppFocusHandler

[](javascript:void(0))

# AppFocusService

Handling app focus The AppFocusService allows developers to be notified when their apps become visible on the screen. Common reasons your app may be running but not be on screen are: it's still in the middle of launching and being revealed by a system animation, or it is being covered by a system window such as a notification. This service is useful for apps that require a high degree of user interactivity, like a game where you'll want to pause when a notification covers your app window. It can be also used for apps that want to sync up an intro animation to the end of the system animation that occurs before your app is visible.

## Function Documentation

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void app_focus_service_subscribe_handlers([AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers) handlers)

Subscribe to the focus event service. Once subscribed, the handlers get called every time the app gains or loses focus.

#### Parameters

 handler  
Handlers which will be called on will-focus and did-focus events.

#### See Also

[AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers)

void app_focus_service_subscribe_handlers([AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers) handlers)

Subscribe to the focus event service. Once subscribed, the handlers get called every time the app gains or loses focus.

#### Parameters

 handler  
Handlers which will be called on will-focus and did-focus events.

#### See Also

[AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers)

void app_focus_service_subscribe_handlers([AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers) handlers)

Subscribe to the focus event service. Once subscribed, the handlers get called every time the app gains or loses focus.

#### Parameters

 handlers  
Handlers which will be called on will-focus and did-focus events.

#### See Also

[AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers)

void app_focus_service_subscribe([AppFocusHandler](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandler) handler)

Subscribe to the focus event service. Once subscribed, the handler gets called every time the app focus changes.

##### Notes

Calling this function is equivalent to

    app_focus_service_subscribe_handlers((AppFocusHandlers){
      .will_focus = handler,
    });

Out focus events are triggered when a modal window is about to open and cover the app.

In focus events are triggered when a modal window which is covering the app is about to close.

#### Parameters

 handler  
A callback to be called on will-focus events.

void app_focus_service_unsubscribe(void)

Unsubscribe from the focus event service. Once unsubscribed, the previously registered handlers will no longer be called.

## Data Structure Documentation

struct AppFocusHandlers

There are two different focus events which take place when transitioning to and from an app being in focus. Below is an example of when these events will occur: 1) The app is launched. Once the system animation to the app has completed and the app is completely in focus, the did_focus handler is called with in_focus set to true. 2) A notification comes in and the animation to show the notification starts. The will_focus handler is called with in_focus set to false. 3) The animation completes and the notification is in focus, with the app being completely covered. The did_focus hander is called with in_focus set to false. 4) The notification is dismissed and the animation to return to the app starts. The will_focus handler is called with in_focus set to true. 5) The animation completes and the app is in focus. The did_focus handler is called with in_focus set to true.

#### Data Fields

[AppFocusHandler](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandler) will_focus  
Handler which will be called right before an app will lose or gain focus.

##### Notes

This will be called with in_focus set to true when a window which is covering the app is about to close and return focus to the app.

This will be called with in_focus set to false when a window which will cover the app is about to open, causing the app to lose focus.

[AppFocusHandler](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandler) did_focus  
Handler which will be called when an animation finished which has put the app into focus or taken the app out of focus.

##### Notes

This will be called with in_focus set to true when a window which was covering the app has closed and the app has gained focus.

This will be called with in_focus set to false when a window has opened which is now covering the app, causing the app to lose focus.

## Typedef Documentation

typedef void(\* AppFocusHandler)(bool in_focus)

Callback type for focus events.

#### Parameters

 in_focus  
True if the app is gaining focus, false otherwise.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [app_focus_service_subscribe_handlers](/docs/c/Foundation/Event_Service/AppFocusService/#app_focus_service_subscribe_handlers)
- [app_focus_service_subscribe](/docs/c/Foundation/Event_Service/AppFocusService/#app_focus_service_subscribe)
- [app_focus_service_unsubscribe](/docs/c/Foundation/Event_Service/AppFocusService/#app_focus_service_unsubscribe)

### [Data Structures](#structs)

- [AppFocusHandlers](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandlers)

### [Typedefs](#typedefs)

- [AppFocusHandler](/docs/c/Foundation/Event_Service/AppFocusService/#AppFocusHandler)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
