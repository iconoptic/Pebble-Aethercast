<!--
source: https://developer.repebble.com/docs/c/Foundation/AppMessage/
fetched: 2026-09-07T17:42:36Z
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

Jump to item… FUNCTIONS app_message_open app_message_deregister_callbacks app_message_get_context app_message_set_context app_message_register_inbox_received app_message_register_inbox_dropped app_message_register_outbox_sent app_message_register_outbox_failed app_message_inbox_size_maximum app_message_outbox_size_maximum app_message_outbox_begin app_message_outbox_send ENUMS AppMessageResult TYPEDEFS AppMessageInboxReceived AppMessageInboxDropped AppMessageOutboxSent AppMessageOutboxFailed MACRO DEFINTIONS APP_MESSAGE_INBOX_SIZE_MINIMUM APP_MESSAGE_OUTBOX_SIZE_MINIMUM

[](javascript:void(0))

# AppMessage

Bi-directional communication between phone apps and Pebble watchapps

AppMessage is a bi-directional messaging subsystem that enables communication between phone apps and Pebble watchapps. This is accomplished by allowing phone and watchapps to exchange arbitrary sets of key/value pairs. The key/value pairs are stored in the form of a Dictionary, the layout of which is left for the application developer to define.

AppMessage implements a push-oriented messaging protocol, enabling your app to call functions and methods to push messages from Pebble to phone and vice versa. The protocol is symmetric: both Pebble and the phone can send messages. All messages are acknowledged. In this context, there is no client-server model, as such.

During the sending phase, one side initiates the communication by transferring a dictionary over the air. The other side then receives this message and is given an opportunity to perform actions on that data. As soon as possible, the other side is expected to reply to the message with a simple acknowledgment that the message was received successfully.

PebbleKit JavaScript provides you with a set of standard JavaScript APIs that let your app receive messages from the watch, make HTTP requests, and send new messages to the watch. AppMessage APIs are used to send and receive data. A Pebble watchapp can use the resources of the connected phone to fetch information from web services, send information to web APIs, or store login credentials. On the JavaScript side, you communicate with Pebble via a Pebble object exposed in the namespace.

Messages always need to get either ACKnowledged or "NACK'ed," that is, not acknowledged. If not, messages will result in a time-out failure. The AppMessage subsystem takes care of this implicitly. In the phone libraries, this step is a bit more explicit.

The Pebble watch interfaces make a distinction between the Inbox and the Outbox calls. The Inbox receives messages from the phone on the watch; the Outbox sends messages from the watch to the phone. These two buffers can be managed separately.

#### Warning

A critical constraint of AppMessage is that messages are limited in size. An ingoing (outgoing) message larger than the inbox (outbox) will not be transmitted and will generate an error. You can choose your inbox and outbox size when you call [app_message_open()](/docs/c/Foundation/AppMessage/#app_message_open).

Pebble SDK provides a static minimum guaranteed size (APP_MESSAGE_INBOX_SIZE_MINIMUM and APP_MESSAGE_OUTBOX_SIZE_MINIMUM). Requesting a buffer of the minimum guaranteed size (or smaller) is always guaranteed to succeed on all Pebbles in this SDK version or higher, and with every phone.

In some context, Pebble might be able to provide your application with larger inbox/outbox. You can call [app_message_inbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_inbox_size_maximum) and [app_message_outbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_outbox_size_maximum) in your code to get the largest possible value you can use.

To always get the largest buffer available, follow this best practice:

app_message_open([app_message_inbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_inbox_size_maximum), [app_message_outbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_outbox_size_maximum))

AppMessage uses your application heap space. That means that the sizes you pick for the AppMessage inbox and outbox buffers are important in optimizing your app’s performance. The more you use for AppMessage, the less space you’ll have for the rest of your app.

To register callbacks, you should call [app_message_register_inbox_received()](/docs/c/Foundation/AppMessage/#app_message_register_inbox_received), [app_message_register_inbox_dropped()](/docs/c/Foundation/AppMessage/#app_message_register_inbox_dropped), [app_message_register_outbox_sent()](/docs/c/Foundation/AppMessage/#app_message_register_outbox_sent), [app_message_register_outbox_failed()](/docs/c/Foundation/AppMessage/#app_message_register_outbox_failed).

Pebble recommends that you call them before [app_message_open()](/docs/c/Foundation/AppMessage/#app_message_open) to ensure you do not miss a message arriving between starting AppMessage and registering the callback. You can set a context that will be passed to all the callbacks with [app_message_set_context()](/docs/c/Foundation/AppMessage/#app_message_set_context).

In circumstances that may not be ideal, when using AppMessage several types of errors may occur. For example:

- The send can’t start because the system state won't allow for a success. Several reasons you're unable to perform a send: A send() is already occurring (only one is possible at a time) or Bluetooth is not enabled or connected.

- The send and receive occur, but the receiver can’t accept the message. For instance, there is no app that receives such a message.

- The send occurs, but the receiver either does not actually receive the message or can’t handle it in a timely fashion.

- In the case of a dropped message, the phone sends a message to the watchapp, while there is still an unprocessed message in the Inbox.

Other errors are possible and described by [AppMessageResult](/docs/c/Foundation/AppMessage/#AppMessageResult). A client of the AppMessage interface should use the result codes to be more robust in the face of communication problems either in the field or while debugging.

Refer to the

[App Communication in the Pebble Developer Guides](http://developer.getpebble.com/guides/pebble-apps/communications/)

for a conceptual overview and code usage.

For code examples, refer to the SDK Examples that directly use App Message. These include:

- [pebblekit-js-weather](https://github.com/pebble-examples/pebblekit-js-weather)

- [pebblekit-js-quotes](https://github.com/pebble-examples/pebblekit-js-quotes)

## Function Documentation

[AppMessageResult](/docs/c/Foundation/AppMessage/#AppMessageResult) app_message_open(const [uint32_t](/docs/c/Standard_C/#uint32_t) size_inbound, const [uint32_t](/docs/c/Standard_C/#uint32_t) size_outbound)

Open AppMessage to transfers.

Use [dict_calc_buffer_size_from_tuplets()](/docs/c/Foundation/Dictionary/#dict_calc_buffer_size_from_tuplets) or [dict_calc_buffer_size()](/docs/c/Foundation/Dictionary/#dict_calc_buffer_size) to estimate the size you need.

##### Note

It is recommended that if the Inbox will be used, that at least the Inbox callbacks should be registered before this call. Otherwise it is possible for an Inbox message to be NACK'ed without being seen by the application.

#### Parameters

 size_inbound (in)  
The required size for the Inbox buffer

 size_outbound (in)  
The required size for the Outbox buffer

#### Returns

A result code such as APP_MSG_OK or APP_MSG_OUT_OF_MEMORY.

void app_message_deregister_callbacks(void)

Deregisters all callbacks and their context.

void \* app_message_get_context(void)

Gets the context that will be passed to all AppMessage callbacks.

#### Returns

The current context on record.

void \* app_message_set_context(void \* context)

Sets the context that will be passed to all AppMessage callbacks.

#### Parameters

 context (in)  
The context that will be passed to all AppMessage callbacks.

#### Returns

The previous context that was on record.

[AppMessageInboxReceived](/docs/c/Foundation/AppMessage/#AppMessageInboxReceived) app_message_register_inbox_received([AppMessageInboxReceived](/docs/c/Foundation/AppMessage/#AppMessageInboxReceived) received_callback)

Registers a function that will be called after any Inbox message is received successfully.

Only one callback may be registered at a time. Each subsequent call to this function will replace the previous callback. The callback is optional; setting it to NULL will deregister the current callback and no function will be called anymore.

#### Parameters

 received_callback (in)  
The callback that will be called going forward; NULL to not have a callback.

#### Returns

The previous callback (or NULL) that was on record.

[AppMessageInboxDropped](/docs/c/Foundation/AppMessage/#AppMessageInboxDropped) app_message_register_inbox_dropped([AppMessageInboxDropped](/docs/c/Foundation/AppMessage/#AppMessageInboxDropped) dropped_callback)

Registers a function that will be called after any Inbox message is received but dropped by the system.

Only one callback may be registered at a time. Each subsequent call to this function will replace the previous callback. The callback is optional; setting it to NULL will deregister the current callback and no function will be called anymore.

#### Parameters

 dropped_callback (in)  
The callback that will be called going forward; NULL to not have a callback.

#### Returns

The previous callback (or NULL) that was on record.

[AppMessageOutboxSent](/docs/c/Foundation/AppMessage/#AppMessageOutboxSent) app_message_register_outbox_sent([AppMessageOutboxSent](/docs/c/Foundation/AppMessage/#AppMessageOutboxSent) sent_callback)

Registers a function that will be called after any Outbox message is sent and an ACK reply occurs in a timely fashion.

Only one callback may be registered at a time. Each subsequent call to this function will replace the previous callback. The callback is optional; setting it to NULL will deregister the current callback and no function will be called anymore.

#### Parameters

 sent_callback (in)  
The callback that will be called going forward; NULL to not have a callback.

#### Returns

The previous callback (or NULL) that was on record.

[AppMessageOutboxFailed](/docs/c/Foundation/AppMessage/#AppMessageOutboxFailed) app_message_register_outbox_failed([AppMessageOutboxFailed](/docs/c/Foundation/AppMessage/#AppMessageOutboxFailed) failed_callback)

Registers a function that will be called after any Outbox message is not sent with a timely ACK reply. The call to [app_message_outbox_send()](/docs/c/Foundation/AppMessage/#app_message_outbox_send) must have succeeded.

Only one callback may be registered at a time. Each subsequent call to this function will replace the previous callback. The callback is optional; setting it to NULL will deregister the current callback and no function will be called anymore.

#### Parameters

 failed_callback (in)  
The callback that will be called going forward; NULL to not have a callback.

#### Returns

The previous callback (or NULL) that was on record.

[uint32_t](/docs/c/Standard_C/#uint32_t) app_message_inbox_size_maximum(void)

Programatically determine the inbox size maximum in the current configuration.

#### Returns

The inbox size maximum on this firmware.

#### See Also

[APP_MESSAGE_INBOX_SIZE_MINIMUM](/docs/c/Foundation/AppMessage/#APP_MESSAGE_INBOX_SIZE_MINIMUM)\
[app_message_outbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_outbox_size_maximum)

[uint32_t](/docs/c/Standard_C/#uint32_t) app_message_outbox_size_maximum(void)

Programatically determine the outbox size maximum in the current configuration.

#### Returns

The outbox size maximum on this firmware.

#### See Also

[APP_MESSAGE_OUTBOX_SIZE_MINIMUM](/docs/c/Foundation/AppMessage/#APP_MESSAGE_OUTBOX_SIZE_MINIMUM)\
[app_message_inbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_inbox_size_maximum)

[AppMessageResult](/docs/c/Foundation/AppMessage/#AppMessageResult) app_message_outbox_begin([DictionaryIterator](/docs/c/Foundation/Dictionary/#DictionaryIterator) \*\* iterator)

Begin writing to the Outbox's Dictionary buffer.

##### Note

After a successful call, one can add values to the dictionary using functions like [dict_write_data()](/docs/c/Foundation/Dictionary/#dict_write_data) and friends.

#### Parameters

 iterator (out)  
Location to write the [DictionaryIterator](/docs/c/Foundation/Dictionary/#DictionaryIterator) pointer. This will be NULL on failure.

#### Returns

A result code, including but not limited to APP_MSG_OK, APP_MSG_INVALID_ARGS or APP_MSG_BUSY.

#### See Also

[Dictionary](/docs/c/Foundation/Dictionary/)

[AppMessageResult](/docs/c/Foundation/AppMessage/#AppMessageResult) app_message_outbox_send(void)

Sends the outbound dictionary.

#### Returns

A result code, including but not limited to APP_MSG_OK or APP_MSG_BUSY. The APP_MSG_OK code does not mean that the message was sent successfully, but only that the start of processing was successful. Since this call is asynchronous, callbacks provide the final result instead.

#### See Also

[AppMessageOutboxSent](/docs/c/Foundation/AppMessage/#AppMessageOutboxSent)\
[AppMessageOutboxFailed](/docs/c/Foundation/AppMessage/#AppMessageOutboxFailed)

## Enum Documentation

enum AppMessageResult

AppMessage result codes.

#### Enumerators

APP_MSG_OK  
\(0\) All good, operation was successful.

APP_MSG_SEND_TIMEOUT  
\(2\) The other end did not confirm receiving the sent data with an (n)ack in time.

APP_MSG_SEND_REJECTED  
\(4\) The other end rejected the sent data, with a "nack" reply.

APP_MSG_NOT_CONNECTED  
\(8\) The other end was not connected.

APP_MSG_APP_NOT_RUNNING  
\(16\) The local application was not running.

APP_MSG_INVALID_ARGS  
\(32\) The function was called with invalid arguments.

APP_MSG_BUSY  
\(64\) There are pending (in or outbound) messages that need to be processed first before new ones can be received or sent.

APP_MSG_BUFFER_OVERFLOW  
\(128\) The buffer was too small to contain the incoming message.

APP_MSG_ALREADY_RELEASED  
\(512\) The resource had already been released.

APP_MSG_CALLBACK_ALREADY_REGISTERED  
\(1024\) The callback was already registered.

APP_MSG_CALLBACK_NOT_REGISTERED  
\(2048\) The callback could not be deregistered, because it had not been registered before.

APP_MSG_OUT_OF_MEMORY  
\(4096\) The system did not have sufficient application memory to perform the requested operation.

APP_MSG_CLOSED  
\(8192\) App message was closed.

APP_MSG_INTERNAL_ERROR  
\(16384\) An internal OS error prevented AppMessage from completing an operation.

APP_MSG_INVALID_STATE  
\(32768\) The function was called while App Message was not in the appropriate state.

## Typedef Documentation

typedef void(\* AppMessageInboxReceived)(DictionaryIterator \*iterator, void \*context)

Called after an incoming message is received.

#### Parameters

 iterator (in)  
The dictionary iterator to the received message. Never NULL. Note that the iterator cannot be modified or saved off. The library may need to re-use the buffered space where this message is supplied. Returning from the callback indicates to the library that the received message contents are no longer needed or have already been externalized outside its buffering space and iterator.

 context (in)  
Pointer to application data as specified when registering the callback.

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

typedef void(\* AppMessageInboxDropped)(AppMessageResult reason, void \*context)

Called after an incoming message is dropped.

Note that you can call [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin) from this handler to prepare a new message. This will invalidate the previous dictionary iterator; do not use it after calling [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin).

#### Parameters

 result (in)  
The reason why the message was dropped. Some possibilities include APP_MSG_BUSY and APP_MSG_BUFFER_OVERFLOW.

 context (in)  
Pointer to application data as specified when registering the callback.

typedef void(\* AppMessageInboxDropped)(AppMessageResult reason, void \*context)

Called after an incoming message is dropped.

Note that you can call [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin) from this handler to prepare a new message. This will invalidate the previous dictionary iterator; do not use it after calling [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin).

#### Parameters

 result (in)  
The reason why the message was dropped. Some possibilities include APP_MSG_BUSY and APP_MSG_BUFFER_OVERFLOW.

 context (in)  
Pointer to application data as specified when registering the callback.

typedef void(\* AppMessageInboxDropped)(AppMessageResult reason, void \*context)

Called after an incoming message is dropped.

Note that you can call [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin) from this handler to prepare a new message. This will invalidate the previous dictionary iterator; do not use it after calling [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin).

#### Parameters

 reason (in)  
The reason why the message was dropped. Some possibilities include APP_MSG_BUSY and APP_MSG_BUFFER_OVERFLOW.

 context (in)  
Pointer to application data as specified when registering the callback.

typedef void(\* AppMessageOutboxSent)(DictionaryIterator \*iterator, void \*context)

Called after an outbound message has been sent and the reply has been received.

#### Parameters

 iterator (in)  
The dictionary iterator to the sent message. The iterator will be in the final state that was sent. Note that the iterator cannot be modified or saved off as the library will re-open the dictionary with dict_begin() after this callback returns.

 context (in)  
Pointer to application data as specified when registering the callback.

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

typedef void(\* AppMessageOutboxFailed)(DictionaryIterator \*iterator, AppMessageResult reason, void \*context)

Called after an outbound message has not been sent successfully.

Note that you can call [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin) from this handler to prepare a new message. This will invalidate the previous dictionary iterator; do not use it after calling [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin).

#### Parameters

 iterator (in)  
The dictionary iterator to the sent message. The iterator will be in the final state that was sent. Note that the iterator cannot be modified or saved off as the library will re-open the dictionary with dict_begin() after this callback returns.

 result (in)  
The result of the operation. Some possibilities for the value include APP_MSG_SEND_TIMEOUT, APP_MSG_SEND_REJECTED, APP_MSG_NOT_CONNECTED, APP_MSG_APP_NOT_RUNNING, and the combination `(APP_MSG_NOT_CONNECTED | APP_MSG_APP_NOT_RUNNING)`.

 context  
Pointer to application data as specified when registering the callback.

typedef void(\* AppMessageOutboxFailed)(DictionaryIterator \*iterator, AppMessageResult reason, void \*context)

Called after an outbound message has not been sent successfully.

Note that you can call [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin) from this handler to prepare a new message. This will invalidate the previous dictionary iterator; do not use it after calling [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin).

#### Parameters

 iterator (in)  
The dictionary iterator to the sent message. The iterator will be in the final state that was sent. Note that the iterator cannot be modified or saved off as the library will re-open the dictionary with dict_begin() after this callback returns.

 result (in)  
The result of the operation. Some possibilities for the value include APP_MSG_SEND_TIMEOUT, APP_MSG_SEND_REJECTED, APP_MSG_NOT_CONNECTED, APP_MSG_APP_NOT_RUNNING, and the combination `(APP_MSG_NOT_CONNECTED | APP_MSG_APP_NOT_RUNNING)`.

 context  
Pointer to application data as specified when registering the callback.

typedef void(\* AppMessageOutboxFailed)(DictionaryIterator \*iterator, AppMessageResult reason, void \*context)

Called after an outbound message has not been sent successfully.

Note that you can call [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin) from this handler to prepare a new message. This will invalidate the previous dictionary iterator; do not use it after calling [app_message_outbox_begin()](/docs/c/Foundation/AppMessage/#app_message_outbox_begin).

#### Parameters

 iterator (in)  
The dictionary iterator to the sent message. The iterator will be in the final state that was sent. Note that the iterator cannot be modified or saved off as the library will re-open the dictionary with dict_begin() after this callback returns.

 reason (in)  
The result of the operation. Some possibilities for the value include APP_MSG_SEND_TIMEOUT, APP_MSG_SEND_REJECTED, APP_MSG_NOT_CONNECTED, APP_MSG_APP_NOT_RUNNING, and the combination `(APP_MSG_NOT_CONNECTED | APP_MSG_APP_NOT_RUNNING)`.

 context  
Pointer to application data as specified when registering the callback.

## Macro Definition Documentation

\#define APP_MESSAGE_INBOX_SIZE_MINIMUM 124

As long as the firmware maintains its current major version, inboxes of this size or smaller will be allowed.

#### See Also

[app_message_inbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_inbox_size_maximum)\
[APP_MESSAGE_OUTBOX_SIZE_MINIMUM](/docs/c/Foundation/AppMessage/#APP_MESSAGE_OUTBOX_SIZE_MINIMUM)

\#define APP_MESSAGE_OUTBOX_SIZE_MINIMUM 636

As long as the firmware maintains its current major version, outboxes of this size or smaller will be allowed.

#### See Also

[app_message_outbox_size_maximum()](/docs/c/Foundation/AppMessage/#app_message_outbox_size_maximum)\
[APP_MESSAGE_INBOX_SIZE_MINIMUM](/docs/c/Foundation/AppMessage/#APP_MESSAGE_INBOX_SIZE_MINIMUM)

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [app_message_open](/docs/c/Foundation/AppMessage/#app_message_open)
- [app_message_deregister_callbacks](/docs/c/Foundation/AppMessage/#app_message_deregister_callbacks)
- [app_message_get_context](/docs/c/Foundation/AppMessage/#app_message_get_context)
- [app_message_set_context](/docs/c/Foundation/AppMessage/#app_message_set_context)
- [app_message_register_inbox_received](/docs/c/Foundation/AppMessage/#app_message_register_inbox_received)
- [app_message_register_inbox_dropped](/docs/c/Foundation/AppMessage/#app_message_register_inbox_dropped)
- [app_message_register_outbox_sent](/docs/c/Foundation/AppMessage/#app_message_register_outbox_sent)
- [app_message_register_outbox_failed](/docs/c/Foundation/AppMessage/#app_message_register_outbox_failed)
- [app_message_inbox_size_maximum](/docs/c/Foundation/AppMessage/#app_message_inbox_size_maximum)
- [app_message_outbox_size_maximum](/docs/c/Foundation/AppMessage/#app_message_outbox_size_maximum)
- [app_message_outbox_begin](/docs/c/Foundation/AppMessage/#app_message_outbox_begin)
- [app_message_outbox_send](/docs/c/Foundation/AppMessage/#app_message_outbox_send)

### [Enums](#enums)

- [AppMessageResult](/docs/c/Foundation/AppMessage/#AppMessageResult)

### [Typedefs](#typedefs)

- [AppMessageInboxReceived](/docs/c/Foundation/AppMessage/#AppMessageInboxReceived)
- [AppMessageInboxDropped](/docs/c/Foundation/AppMessage/#AppMessageInboxDropped)
- [AppMessageOutboxSent](/docs/c/Foundation/AppMessage/#AppMessageOutboxSent)
- [AppMessageOutboxFailed](/docs/c/Foundation/AppMessage/#AppMessageOutboxFailed)

### [Macro Defintions](#defines)

- [APP_MESSAGE_INBOX_SIZE_MINIMUM](/docs/c/Foundation/AppMessage/#APP_MESSAGE_INBOX_SIZE_MINIMUM)
- [APP_MESSAGE_OUTBOX_SIZE_MINIMUM](/docs/c/Foundation/AppMessage/#APP_MESSAGE_OUTBOX_SIZE_MINIMUM)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
