<!--
source: https://developer.repebble.com/docs/c/Foundation/Event_Service/ConnectionService/
fetched: 2026-09-07T17:42:42Z
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

Jump to item… FUNCTIONS connection_service_peek_pebble_app_connection connection_service_peek_pebblekit_connection connection_service_subscribe connection_service_unsubscribe bluetooth_connection_service_peek bluetooth_connection_service_subscribe bluetooth_connection_service_unsubscribe DATA STRUCTURES ConnectionHandlers TYPEDEFS ConnectionHandler BluetoothConnectionHandler

[](javascript:void(0))

# ConnectionService

Determine what the Pebble watch is connected to

The ConnectionService allows your app to learn about the apps the Pebble watch is connected to. You can ask the system for this information at a given time or you can register to receive events every time connection or disconnection events occur.

It allows you to determine whether the watch is connected to the Pebble mobile app by subscribing to the pebble_app_connection_handler or by calling the connection_service_peek_pebble_app_connection function. Note that when the Pebble app is connected, you can assume PebbleKit JS apps will also be running correctly.

The service also allows you to determine if the Pebble watch can establish a connection to a PebbleKit companion app by subscribing to the pebblekit_connection_handler or by calling the connection_service_peek_pebblekit_connection function. Today, due to architectural differences between iOS and Android, this will return true for Android anytime a connection with the Pebble mobile app is established (since PebbleKit messages are routed through the Android app). For iOS, this will return true when any PebbleKit companion app has established a connection with the Pebble watch (since companion app messages are routed directly to the watch)

## Function Documentation

bool connection_service_peek_pebble_app_connection(void)

Query the bluetooth connection service for the current Pebble app connection status.

#### Returns

true if the Pebble app is connected, false otherwise

bool connection_service_peek_pebblekit_connection(void)

Query the bluetooth connection service for the current PebbleKit connection status.

#### Returns

true if a PebbleKit companion app is connected, false otherwise

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void connection_service_subscribe(ConnectionHandlers conn_handlers)

Subscribe to the connection event service. Once subscribed, the appropriate handler gets called based on the type of connection event and user provided handlers.

#### Parameters

 ConnectionHandlers  
A struct populated with the handlers to be called when the specified connection event occurs. If a given handler is NULL, no function will be called.

void connection_service_subscribe(ConnectionHandlers conn_handlers)

Subscribe to the connection event service. Once subscribed, the appropriate handler gets called based on the type of connection event and user provided handlers.

#### Parameters

 ConnectionHandlers  
A struct populated with the handlers to be called when the specified connection event occurs. If a given handler is NULL, no function will be called.

void connection_service_subscribe(ConnectionHandlers conn_handlers)

Subscribe to the connection event service. Once subscribed, the appropriate handler gets called based on the type of connection event and user provided handlers.

#### Parameters

 conn_handlers  
A struct populated with the handlers to be called when the specified connection event occurs. If a given handler is NULL, no function will be called.

void connection_service_unsubscribe(void)

Unsubscribe from the bluetooth event service. Once unsubscribed, the previously registered handler will no longer be called.

bool bluetooth_connection_service_peek(void)

Deprecated

Backward compatibility function for connection_service_peek_pebble_app_connection. New code should use connection_service_peek_pebble_app_connection directly. This will be removed in a future version of the Pebble SDK

void bluetooth_connection_service_subscribe(ConnectionHandler handler)

Deprecated

Backward compatibility function for connection_service_subscribe. New code should use connection_service_subscribe directly. This will be removed in a future version of the Pebble SDK

void bluetooth_connection_service_unsubscribe(void)

Deprecated

Backward compatibility function for connection_service_unsubscribe. New code should use connection_service_unsubscribe directly. This will be removed in a future version of the Pebble SDK

## Data Structure Documentation

struct ConnectionHandlers

#### Data Fields

ConnectionHandler pebble_app_connection_handler  
callback to be executed when the connection state between the watch and the phone app has changed. Note, if the phone App is connected, PebbleKit JS apps will also be working correctly

ConnectionHandler pebblekit_connection_handler  
ID for callback to be executed on PebbleKit connection event.

## Typedef Documentation

typedef void(\* ConnectionHandler)(bool connected)

typedef ConnectionHandler BluetoothConnectionHandler

Deprecated

Backwards compatibility typedef for ConnectionHandler. New code should use ConnectionHandler directly. This will be removed in a future version of the Pebble SDK.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [connection_service_peek_pebble_app_connection](/docs/c/Foundation/Event_Service/ConnectionService/#connection_service_peek_pebble_app_connection)
- [connection_service_peek_pebblekit_connection](/docs/c/Foundation/Event_Service/ConnectionService/#connection_service_peek_pebblekit_connection)
- [connection_service_subscribe](/docs/c/Foundation/Event_Service/ConnectionService/#connection_service_subscribe)
- [connection_service_unsubscribe](/docs/c/Foundation/Event_Service/ConnectionService/#connection_service_unsubscribe)
- [bluetooth_connection_service_peek](/docs/c/Foundation/Event_Service/ConnectionService/#bluetooth_connection_service_peek)
- [bluetooth_connection_service_subscribe](/docs/c/Foundation/Event_Service/ConnectionService/#bluetooth_connection_service_subscribe)
- [bluetooth_connection_service_unsubscribe](/docs/c/Foundation/Event_Service/ConnectionService/#bluetooth_connection_service_unsubscribe)

### [Data Structures](#structs)

- [ConnectionHandlers](/docs/c/Foundation/Event_Service/ConnectionService/#ConnectionHandlers)

### [Typedefs](#typedefs)

- [ConnectionHandler](/docs/c/Foundation/Event_Service/ConnectionService/#ConnectionHandler)
- [BluetoothConnectionHandler](/docs/c/Foundation/Event_Service/ConnectionService/#BluetoothConnectionHandler)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
