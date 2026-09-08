<!--
source: https://developer.repebble.com/guides/communication/sending-and-receiving-data/
fetched: 2026-09-07T17:42:25Z
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
  - [Advanced Communication](/guides/communication/advanced-communication/)
  - [Datalogging](/guides/communication/datalogging/)
  - [PebbleKit Android](/guides/communication/using-pebblekit-android/)
  - [PebbleKit JS](/guides/communication/using-pebblekit-js/)
  - [PebbleKit iOS](/guides/communication/using-pebblekit-ios/)
  - [Sending and Receiving Data](/guides/communication/sending-and-receiving-data/)
  - [Sports API](/guides/communication/using-the-sports-api/)
- [Debugging](/guides/debugging/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

Message Structure Data Types Buffer Sizes Choosing Keys Using Callbacks - Inbox Received - Inbox Dropped - Outbox Sent - Outbox Failed Constructing an Outgoing Message - Example Outgoing Message Construction Reading an Incoming Message - Reading an Integer - Reading a String - Reading Binary Data

# Sending and Receiving Data

Before using [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage"), a Pebble C app must set up the buffers used for the inbox and outbox. These are used to store received messages that have not yet been processed, and sent messages that have not yet been transmitted. In addition, callbacks may be registered to allow an app to respond to any success or failure events that occur when dealing with messages. Doing all of this is discussed in this guide.

## Message Structure

Every message sent or received using the [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") API is stored in a [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") structure, which is essentially a list of [`Tuple`](/docs/c/Foundation/Dictionary/#Tuple "Tuple") objects. Each [`Tuple`](/docs/c/Foundation/Dictionary/#Tuple "Tuple") contains a key used to 'label' the value associated with that key.

When a message is sent, a [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") is filled with a [`Tuple`](/docs/c/Foundation/Dictionary/#Tuple "Tuple") for each item of outgoing data. Conversely, when a message is received the [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") provided by the callback is examined for the presence of each key. If a key is present, the value associated with it can be read.

## Data Types

The [`Tuple.value`](/docs/c/Foundation/Dictionary/#Tuple "Tuple") union allows multiple data types to be stored in and read from each received message. These are detailed below:

| Name    | Type        | Size in Bytes         | Signed? |
|---------|-------------|-----------------------|---------|
| uint8   | `uint8_t`   | 1                     | No      |
| uint16  | `uint16_t`  | 2                     | No      |
| uint32  | `uint32_t`  | 4                     | No      |
| int8    | `int8_t`    | 1                     | Yes     |
| int16   | `int16_t`   | 2                     | Yes     |
| int32   | `int32_t`   | 4                     | Yes     |
| cstring | `char[]`    | Variable length array | N/A     |
| data    | `uint8_t[]` | Variable length array | N/A     |

## Buffer Sizes

The size of each of the outbox and inbox buffers must be set chosen such that the largest message that the app will ever send or receive will fit. Incoming messages that exceed the size of the inbox buffer, and outgoing messages that exceed that size of the outbox buffer will be dropped.

These sizes are specified when the [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") system is 'opened', allowing communication to occur:

// Largest expected inbox and outbox message sizes const uint32_t inbox_size = 64; const uint32_t outbox_size = 256; // Open AppMessage app_message_open(inbox_size, outbox_size);

Each of these buffers is allocated at this moment, and comes out of the app's memory budget, so the sizes of the inbox and outbox should be conservative. Calculate the size of the buffer you require by summing the sizes of all the keys and values in the larges message the app will handle. For example, a message containing three integer keys and values will work with a 32 byte buffer size.

## Choosing Keys

For each message sent and received, the contents are accessible using keys-value pairs in a [`Tuple`](/docs/c/Foundation/Dictionary/#Tuple "Tuple"). This allows each piece of data in the message to be uniquely identifiable using its key, and also allows many different data types to be stored inside a single message.

Each possible piece of data that may be transmitted should be assigned a unique key value, used to read the associated value when it is found in a received message. An example for a weather app is shown below::

- Temperature
- WindSpeed
- WindDirection
- RequestData
- LocationName

These values will be made available in any file that includes `pebble.h` prefixed with `MESSAGE_KEY_`, such as `MESSAGE_KEY_Temperature` and `MESSAGE_KEY_WindSpeed`.

Examples of how these key values would be used in the phone-side app are shown in [*PebbleKit JS*](/guides/communication/using-pebblekit-js/), [*PebbleKit iOS*](/guides/communication/using-pebblekit-ios/), and [*PebbleKit Android*](/guides/communication/using-pebblekit-android/).

## Using Callbacks

Like many other aspects of the Pebble C API, the [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") system makes use of developer-defined callbacks to allow an app to gracefully handle all events that may occur, such as successfully sent or received messages as well as any errors that may occur.

These callback types are discussed below. Each is used by first creating a function that matches the signature of the callback type, and then registering it with the [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") system to be called when that event type occurs. Good use of callbacks to drive the app's UI will result in an improved user experience, especially when errors occur that the user can be guided in fixing.

### Inbox Received

The [`AppMessageInboxReceived`](/docs/c/Foundation/AppMessage/#AppMessageInboxReceived "AppMessageInboxReceived") callback is called when a new message has been received from the connected phone. This is the moment when the contents can be read and used to drive what the app does next, using the provided [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") to read the message. An example is shown below under [*Reading an Incoming Message*](#reading-an-incoming-message):

static void inbox_received_callback(DictionaryIterator \*iter, void \*context) { // A new message has been successfully received }

Register this callback so that it is called at the appropriate time:

// Register to be notified about inbox received events app_message_register_inbox_received(inbox_received_callback);

### Inbox Dropped

The [`AppMessageInboxDropped`](/docs/c/Foundation/AppMessage/#AppMessageInboxDropped "AppMessageInboxDropped") callback is called when a message was received, but it was dropped. A common cause of this is that the message was too big for the inbox. The reason for failure can be determined using the [`AppMessageResult`](/docs/c/Foundation/AppMessage/#AppMessageResult "AppMessageResult") provided by the callback:

static void inbox_dropped_callback(AppMessageResult reason, void \*context) { // A message was received, but had to be dropped APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped. Reason: %d", (int)reason); }

Register this callback so that it is called at the appropriate time:

// Register to be notified about inbox dropped events app_message_register_inbox_dropped(inbox_dropped_callback);

### Outbox Sent

The [`AppMessageOutboxSent`](/docs/c/Foundation/AppMessage/#AppMessageOutboxSent "AppMessageOutboxSent") callback is called when a message sent from Pebble has been successfully delivered to the connected phone. The provided [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") can be optionally used to inspect the contents of the message just sent.

> When sending multiple messages in a short space of time, it is **strongly** recommended to make use of this callback to wait until the previous message has been sent before sending the next.

static void outbox_sent_callback(DictionaryIterator \*iter, void \*context) { // The message just sent has been successfully delivered }

Register this callback so that it is called at the appropriate time:

// Register to be notified about outbox sent events app_message_register_outbox_sent(outbox_sent_callback);

### Outbox Failed

The [`AppMessageOutboxFailed`](/docs/c/Foundation/AppMessage/#AppMessageOutboxFailed "AppMessageOutboxFailed") callback is called when a message just sent failed to be successfully delivered to the connected phone. The reason can be determined by reading the value of the provided [`AppMessageResult`](/docs/c/Foundation/AppMessage/#AppMessageResult "AppMessageResult"), and the contents of the failed message inspected with the provided [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator").

Use of this callback is strongly encouraged, since it allows an app to detect a failed message and either retry its transmission, or inform the user of the failure so that they can attempt their action again.

static void outbox_failed_callback(DictionaryIterator \*iter, AppMessageResult reason, void \*context) { // The message just sent failed to be delivered APP_LOG(APP_LOG_LEVEL_ERROR, "Message send failed. Reason: %d", (int)reason); }

Register this callback so that it is called at the appropriate time:

// Register to be notified about outbox failed events app_message_register_outbox_failed(outbox_failed_callback);

## Constructing an Outgoing Message

A message is constructed and sent from the C app via [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") using a [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") object and the [`Dictionary`](/docs/c/Foundation/Dictionary/ "Dictionary") APIs. Ensure that [`app_message_open()`](/docs/c/Foundation/AppMessage/#app_message_open "app_message_open") has been called before sending or receiving any messages.

The first step is to begin an outgoing message by preparing a [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator") pointer, used to keep track of the state of the dictionary being constructed:

// Declare the dictionary's iterator DictionaryIterator \*out_iter; // Prepare the outbox buffer for this message AppMessageResult result = app_message_outbox_begin(&out_iter);

The [`AppMessageResult`](/docs/c/Foundation/AppMessage/#AppMessageResult "AppMessageResult") should be checked to make sure the outbox was successfully prepared:

if(result == APP_MSG_OK) { // Construct the message } else { // The outbox cannot be used right now APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result); }

If the result is [`APP_MSG_OK`](/docs/c/Foundation/AppMessage/#APP_MSG_OK "APP_MSG_OK"), the message construction can continue. Data is now written to the dictionary according to data type using the [`Dictionary`](/docs/c/Foundation/Dictionary/ "Dictionary") APIs. An example from the hypothetical weather app is shown below:

if(result == APP_MSG_OK) { // A dummy value int value = 0; // Add an item to ask for weather data dict_write_int(out_iter, MESSAGE_KEY_RequestData, &value, sizeof(int), true); }

After all desired data has been written to the dictionary, the message may be sent:

// Send this message result = app_message_outbox_send(); // Check the result if(result != APP_MSG_OK) { APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result); }

**Important**

Any app that wishes to send data from the watch to the phone via PebbleKit JS must wait until the `ready` event has occured, indicating that the phone has loaded the JavaScript for the app and it is ready to receive data. See [*Advanced Communication*](/guides/communication/advanced-communication#waiting-for-pebblekit-js) for more information.

Once the message send operation has been completed, either the [`AppMessageOutboxSent`](/docs/c/Foundation/AppMessage/#AppMessageOutboxSent "AppMessageOutboxSent") or [`AppMessageOutboxFailed`](/docs/c/Foundation/AppMessage/#AppMessageOutboxFailed "AppMessageOutboxFailed") callbacks will be called (if they have been registered), depending on either a success or failure outcome.

### Example Outgoing Message Construction

A complete example of assembling an outgoing message is shown below:

// Declare the dictionary's iterator DictionaryIterator \*out_iter; // Prepare the outbox buffer for this message AppMessageResult result = app_message_outbox_begin(&out_iter); if(result == APP_MSG_OK) { // Add an item to ask for weather data int value = 0; dict_write_int(out_iter, MESSAGE_KEY_RequestData, &value, sizeof(int), true); // Send this message result = app_message_outbox_send(); if(result != APP_MSG_OK) { APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result); } } else { // The outbox cannot be used right now APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result); }

## Reading an Incoming Message

When a message is received from the connected phone the [`AppMessageInboxReceived`](/docs/c/Foundation/AppMessage/#AppMessageInboxReceived "AppMessageInboxReceived") callback is called, and the message's contents can be read using the provided [`DictionaryIterator`](/docs/c/Foundation/Dictionary/#DictionaryIterator "DictionaryIterator"). This should be done by looking for the presence of each expectd `Tuple` key value, and using the associated value as required.

Most apps will deal with integer values or strings to pass signals or some human-readable information respectively. These common use cases are discussed below.

### Reading an Integer

**From JS**

var dict = { 'Temperature': 29 };

**In C**

static void inbox_received_callback(DictionaryIterator \*iter, void \*context) { // A new message has been successfully received // Does this message contain a temperature value? Tuple \*temperature_tuple = dict_find(iter, MESSAGE_KEY_Temperature); if(temperature_tuple) { // This value was stored as JS Number, which is stored here as int32_t int32_t temperature = temperature_tuple-\>value-\>int32; } }

### Reading a String

A common use of transmitted strings is to display them in a [`TextLayer`](/docs/c/User_Interface/Layers/TextLayer/ "TextLayer"). Since the displayed text is required to be long-lived, a `static` `char` buffer can be used when the data is received:

**From JS**

var dict = { 'LocationName': 'London, UK' };

**In C**

static void inbox_received_callback(DictionaryIterator \*iter, void \*context) { // Is the location name inside this message? Tuple \*location_tuple = dict_find(iter, MESSAGE_KEY_LocationName); if(location_tuple) { // This value was stored as JS String, which is stored here as a char string char \*location_name = location_tuple-\>value-\>cstring; // Use a static buffer to store the string for display static char s_buffer\[MAX_LENGTH\]; snprintf(s_buffer, sizeof(s_buffer), "Location: %s", location_name); // Display in the TextLayer text_layer_set_text(s_text_layer, s_buffer); } }

### Reading Binary Data

Apps that deal in packed binary data can send this data and pack/unpack as required on either side:

**From JS**

var dict = { 'Data': \[1, 2, 4, 8, 16, 32, 64\] };

**In C**

static void inbox_received_callback(DictionaryIterator \*iter, void \*context) { // Expected length of the binary data const int length = 32; // Does this message contain the data tuple? Tuple \*data_tuple = dict_find(iter, MESSAGE_KEY_Data); if(data_tuple) { // Read the binary data value uint8_t \*data = data_tuple-\>value-\>data; // Inspect the first byte, for example uint8_t byte_zero = data\[0\]; // Store into an app-defined buffer memcpy(s_buffer, data, length); }

You need JavaScript enabled to read and post comments.

### Overview

- [Message Structure](#message-structure)
- [Data Types](#data-types)
- [Buffer Sizes](#buffer-sizes)
- [Choosing Keys](#choosing-keys)
- [Using Callbacks](#using-callbacks)
- [Inbox Received](#inbox-received)
- [Inbox Dropped](#inbox-dropped)
- [Outbox Sent](#outbox-sent)
- [Outbox Failed](#outbox-failed)
- [Constructing an Outgoing Message](#constructing-an-outgoing-message)
- [Example Outgoing Message Construction](#example-outgoing-message-construction)
- [Reading an Incoming Message](#reading-an-incoming-message)
- [Reading an Integer](#reading-an-integer)
- [Reading a String](#reading-a-string)
- [Reading Binary Data](#reading-binary-data)
