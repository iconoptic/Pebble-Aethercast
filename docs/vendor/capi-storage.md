<!--
source: https://developer.repebble.com/docs/c/Foundation/Storage/
fetched: 2026-09-07T17:42:37Z
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

Jump to item… FUNCTIONS persist_exists persist_get_size persist_read_bool persist_read_int persist_read_data persist_read_string persist_write_bool persist_write_int persist_write_data persist_write_string persist_delete persist_get_max_size ENUMS StatusCode TYPEDEFS status_t MACRO DEFINTIONS PERSIST_DATA_MAX_LENGTH PERSIST_STRING_MAX_LENGTH

[](javascript:void(0))

# Storage

A mechanism to store persistent application data and state

The Persistent Storage API provides you with a mechanism for performing a variety of tasks, like saving user settings, caching data from the phone app, or counting high scores for Pebble watchapp games.

In Pebble OS, storage is defined by a collection of fields that you can create, modify or delete. In the API, a field is specified as a key with a corresponding value.

Using the Storage API, every app is able to get its own persistent storage space. Each value in that space is associated with a [uint32_t](/docs/c/Standard_C/#uint32_t) key.

Storage supports saving integers, strings and byte arrays. The maximum size of byte arrays and strings is defined by PERSIST_DATA_MAX_LENGTH (currently set to 256 bytes). You call the function persist_exists(key), which returns a boolean indicating if the key exists or not. The Storage API enables your app to save its state, and when compared to using [AppMessage](/docs/c/Foundation/AppMessage/) to retrieve values from the phone, it provides you with a much faster way to restore state. In addition, it draws less power from the battery.

The total size of an app's persisted values is capped; call [persist_get_max_size](/docs/c/Foundation/Storage/#persist_get_max_size) to query the limit at runtime.

## Function Documentation

bool persist_exists(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Checks whether a value has been set for a given key in persistent storage.

#### Parameters

 key  
The key of the field to check.

#### Returns

true if a value exists, otherwise false.

int persist_get_size(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Gets the size of a value for a given key in persistent storage.

#### Parameters

 key  
The key of the field to lookup the data size.

#### Returns

The size of the value in bytes or E_DOES_NOT_EXIST if there is no field matching the given key.

bool persist_read_bool(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Reads a bool value for a given key from persistent storage. If the value has not yet been set, this will return false.

#### Parameters

 key  
The key of the field to read from.

#### Returns

The bool value of the key to read from.

int32_t persist_read_int(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Reads an int value for a given key from persistent storage.

##### Note

The int is a signed 32-bit integer. If the value has not yet been set, this will return 0.

#### Parameters

 key  
The key of the field to read from.

#### Returns

The int value of the key to read from.

int persist_read_data(const [uint32_t](/docs/c/Standard_C/#uint32_t) key, void \* buffer, const [size_t](/docs/c/Standard_C/Memory/#size_t) buffer_size)

Reads a blob of data for a given key from persistent storage into a given buffer. If the value has not yet been set, the given buffer is left unchanged.

#### Parameters

 key  
The key of the field to read from.

 buffer  
The pointer to a buffer to be written to.

 buffer_size  
The maximum size of the given buffer.

#### Returns

The number of bytes written into the buffer or E_DOES_NOT_EXIST if there is no field matching the given key.

int persist_read_string(const [uint32_t](/docs/c/Standard_C/#uint32_t) key, char \* buffer, const [size_t](/docs/c/Standard_C/Memory/#size_t) buffer_size)

Reads a string for a given key from persistent storage into a given buffer. The string will be null terminated. If the value has not yet been set, the given buffer is left unchanged.

#### Parameters

 key  
The key of the field to read from.

 buffer  
The pointer to a buffer to be written to.

 buffer_size  
The maximum size of the given buffer. This includes the null character.

#### Returns

The number of bytes written into the buffer or E_DOES_NOT_EXIST if there is no field matching the given key.

[status_t](/docs/c/Foundation/Storage/#status_t) persist_write_bool(const [uint32_t](/docs/c/Standard_C/#uint32_t) key, const bool value)

Writes a bool value flag for a given key into persistent storage.

#### Parameters

 key  
The key of the field to write to.

 value  
The boolean value to write.

#### Returns

The number of bytes written if successful, a value from [StatusCode](/docs/c/Foundation/Storage/#StatusCode) otherwise.

[status_t](/docs/c/Foundation/Storage/#status_t) persist_write_int(const [uint32_t](/docs/c/Standard_C/#uint32_t) key, const int32_t value)

Writes an int value for a given key into persistent storage.

##### Note

The int is a signed 32-bit integer.

#### Parameters

 key  
The key of the field to write to.

 value  
The int value to write.

#### Returns

The number of bytes written if successful, a value from [StatusCode](/docs/c/Foundation/Storage/#StatusCode) otherwise.

int persist_write_data(const [uint32_t](/docs/c/Standard_C/#uint32_t) key, const void \* data, const [size_t](/docs/c/Standard_C/Memory/#size_t) size)

Writes a blob of data of a specified size in bytes for a given key into persistent storage. The maximum size is [PERSIST_DATA_MAX_LENGTH](/docs/c/Foundation/Storage/#PERSIST_DATA_MAX_LENGTH).

#### Parameters

 key  
The key of the field to write to.

 data  
The pointer to the blob of data.

 size  
The size in bytes.

#### Returns

The number of bytes written if successful, a value from [StatusCode](/docs/c/Foundation/Storage/#StatusCode) otherwise.

int persist_write_string(const [uint32_t](/docs/c/Standard_C/#uint32_t) key, const char \* cstring)

Writes a string a given key into persistent storage. The maximum size is [PERSIST_STRING_MAX_LENGTH](/docs/c/Foundation/Storage/#PERSIST_STRING_MAX_LENGTH) including the null terminator.

#### Parameters

 key  
The key of the field to write to.

 cstring  
The pointer to null terminated string.

#### Returns

The number of bytes written if successful, a value from [StatusCode](/docs/c/Foundation/Storage/#StatusCode) otherwise.

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

[status_t](/docs/c/Foundation/Storage/#status_t) persist_delete(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Deletes the value of a key from persistent storage.

#### Parameters

 key  
The key of the field to delete from.

[status_t](/docs/c/Foundation/Storage/#status_t) persist_delete(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Deletes the value of a key from persistent storage.

#### Parameters

 key  
The key of the field to delete from.

[status_t](/docs/c/Foundation/Storage/#status_t) persist_delete(const [uint32_t](/docs/c/Standard_C/#uint32_t) key)

Deletes the value of a key from persistent storage.

#### Parameters

 key  
The key of the field to delete from.

#### Returns

S_TRUE if successful, E_DOES_NOT_EXIST if a value was not set, or another error value from [StatusCode](/docs/c/Foundation/Storage/#StatusCode).

[size_t](/docs/c/Standard_C/Memory/#size_t) persist_get_max_size(void)

Gets the maximum total size in bytes of all persisted values for the current app on this firmware. Apps targeting older SDKs that don't have this function should assume a 4 KB limit.

#### Returns

The per-app persistent storage capacity in bytes.

## Enum Documentation

enum StatusCode

Status codes. See [status_t](/docs/c/Foundation/Storage/#status_t).

#### Enumerators

S_SUCCESS  
Operation completed successfully.

E_ERROR  
An error occurred (no description).

E_UNKNOWN  
No idea what went wrong.

E_INTERNAL  
There was a generic internal logic error.

E_INVALID_ARGUMENT  
The function was not called correctly.

E_OUT_OF_MEMORY  
Insufficient allocatable memory available.

E_OUT_OF_STORAGE  
Insufficient long-term storage available.

E_OUT_OF_RESOURCES  
Insufficient resources available.

E_RANGE  
Argument out of range (may be dynamic).

E_DOES_NOT_EXIST  
Target of operation does not exist.

E_INVALID_OPERATION  
Operation not allowed (may depend on state).

E_BUSY  
Another operation prevented this one.

E_AGAIN  
Operation not completed; try again.

S_TRUE  
Equivalent of boolean true.

S_FALSE  
Equivalent of boolean false.

S_NO_MORE_ITEMS  
For list-style requests. At end of list.

S_NO_ACTION_REQUIRED  
No action was taken as none was required.

## Typedef Documentation

typedef int32_t status_t

Return value for system operations. See [StatusCode](/docs/c/Foundation/Storage/#StatusCode) for possible values.

## Macro Definition Documentation

\#define PERSIST_DATA_MAX_LENGTH 256

The maximum size of a persist value in bytes.

\#define PERSIST_STRING_MAX_LENGTH [PERSIST_DATA_MAX_LENGTH](/docs/c/Foundation/Storage/#PERSIST_DATA_MAX_LENGTH)

The maximum size of a persist string in bytes including the NULL terminator.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [persist_exists](/docs/c/Foundation/Storage/#persist_exists)
- [persist_get_size](/docs/c/Foundation/Storage/#persist_get_size)
- [persist_read_bool](/docs/c/Foundation/Storage/#persist_read_bool)
- [persist_read_int](/docs/c/Foundation/Storage/#persist_read_int)
- [persist_read_data](/docs/c/Foundation/Storage/#persist_read_data)
- [persist_read_string](/docs/c/Foundation/Storage/#persist_read_string)
- [persist_write_bool](/docs/c/Foundation/Storage/#persist_write_bool)
- [persist_write_int](/docs/c/Foundation/Storage/#persist_write_int)
- [persist_write_data](/docs/c/Foundation/Storage/#persist_write_data)
- [persist_write_string](/docs/c/Foundation/Storage/#persist_write_string)
- [persist_delete](/docs/c/Foundation/Storage/#persist_delete)
- [persist_get_max_size](/docs/c/Foundation/Storage/#persist_get_max_size)

### [Enums](#enums)

- [StatusCode](/docs/c/Foundation/Storage/#StatusCode)

### [Typedefs](#typedefs)

- [status_t](/docs/c/Foundation/Storage/#status_t)

### [Macro Defintions](#defines)

- [PERSIST_DATA_MAX_LENGTH](/docs/c/Foundation/Storage/#PERSIST_DATA_MAX_LENGTH)
- [PERSIST_STRING_MAX_LENGTH](/docs/c/Foundation/Storage/#PERSIST_STRING_MAX_LENGTH)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
