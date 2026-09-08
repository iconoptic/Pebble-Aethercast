<!--
source: https://developer.repebble.com/docs/c/Graphics/Draw_Commands/
fetched: 2026-09-07T17:42:39Z
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

Jump to item… FUNCTIONS gdraw_command_draw gdraw_command_get_type gdraw_command_set_fill_color gdraw_command_get_fill_color gdraw_command_set_stroke_color gdraw_command_get_stroke_color gdraw_command_set_stroke_width gdraw_command_get_stroke_width gdraw_command_get_num_points gdraw_command_set_point gdraw_command_get_point gdraw_command_set_radius gdraw_command_get_radius gdraw_command_set_path_open gdraw_command_get_path_open gdraw_command_set_hidden gdraw_command_get_hidden gdraw_command_frame_draw gdraw_command_frame_set_duration gdraw_command_frame_get_duration gdraw_command_image_create_with_resource gdraw_command_image_clone gdraw_command_image_destroy gdraw_command_image_draw gdraw_command_image_get_bounds_size gdraw_command_image_set_bounds_size gdraw_command_image_get_command_list gdraw_command_list_iterate gdraw_command_list_draw gdraw_command_list_get_command gdraw_command_list_get_num_commands gdraw_command_sequence_create_with_resource gdraw_command_sequence_clone gdraw_command_sequence_destroy gdraw_command_sequence_get_frame_by_elapsed gdraw_command_sequence_get_frame_by_index gdraw_command_sequence_get_bounds_size gdraw_command_sequence_set_bounds_size gdraw_command_sequence_get_play_count gdraw_command_sequence_set_play_count gdraw_command_sequence_get_total_duration gdraw_command_sequence_get_num_frames gdraw_command_frame_get_command_list ENUMS GDrawCommandType TYPEDEFS GDrawCommand GDrawCommandFrame GDrawCommandImage GDrawCommandList GDrawCommandListIteratorCb GDrawCommandSequence

[](javascript:void(0))

# Draw Commands

Pebble Draw Commands are a way to encode arbitrary path draw and fill calls in binary format, so that vector-like graphics can be represented on the watch.

These draw commands can be loaded from resources, manipulated in place and drawn to the current graphics context. Each [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) can be an arbitrary path or a circle with optional fill or stroke. The stroke width and color of the stroke and fill are also encoded within the [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand). Paths can can be drawn open or closed.

All aspects of a draw command can be modified, except for the number of points in a path (a circle only has one point, the center).

Draw commands are grouped into a [GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList), which can be drawn all at once. Each individual [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) can be accessed from a [GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) for modification.

A [GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) forms the basis for [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) and [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) objects. A [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) represents a static image and can be represented by the PDC file format and can be loaded as a resource.

Once you have a [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) loaded in memory you can draw it on the screen in a [LayerUpdateProc](/docs/c/User_Interface/Layers/#LayerUpdateProc) with the [gdraw_command_image_draw()](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_draw).

A [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) represents a single frame of an animated sequence, with multiple frames making up a single [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence), which can also be stored as a PDC and loaded as a resource.

To draw a [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence), use the [gdraw_command_sequence_get_frame_by_elapsed()](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_frame_by_elapsed) to obtain the current [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) and [gdraw_command_frame_draw()](/docs/c/Graphics/Draw_Commands/#gdraw_command_frame_draw) to draw it.

Draw commands also allow access to drawing with sub-pixel precision. The points are treated as Fixed point types in the format 13.3, so that 1/8th of a pixel precision is possible. Only the points in draw commands of the type GDrawCommandTypePrecisePath will be treated as higher precision.

## Function Documentation

void gdraw_command_draw(GContext \* ctx, [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Draw a command.

#### Parameters

 ctx  
The destination graphics context in which to draw

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) to draw

[GDrawCommandType](/docs/c/Graphics/Draw_Commands/#GDrawCommandType) gdraw_command_get_type([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the command type.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the type

#### Returns

The type of the given [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand)

void gdraw_command_set_fill_color([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, GColor fill_color)

Set the fill color of a command.

#### Parameters

 command  
ref DrawCommand for which to set the fill color

 fill_color  
GColor to set for the fill

GColor gdraw_command_get_fill_color([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the fill color of a command.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the fill color

#### Returns

fill color of the given [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand)

void gdraw_command_set_stroke_color([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, GColor stroke_color)

Set the stroke color of a command.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the stroke color

 stroke_color  
GColor to set for the stroke

GColor gdraw_command_get_stroke_color([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the stroke color of a command.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the stroke color

#### Returns

The stroke color of the given [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand)

void gdraw_command_set_stroke_width([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, uint8_t stroke_width)

Set the stroke width of a command.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the stroke width

 stroke_width  
stroke width to set for the command

uint8_t gdraw_command_get_stroke_width([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the stroke width of a command.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the stroke width

#### Returns

The stroke width of the given [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand)

[uint16_t](/docs/c/Standard_C/#uint16_t) gdraw_command_get_num_points([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the number of points in a command.

void gdraw_command_set_point([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, [uint16_t](/docs/c/Standard_C/#uint16_t) point_idx, [GPoint](/docs/c/Graphics/Graphics_Types/#GPoint) point)

Set the value of the point in a command at the specified index.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the value of a point

 point_idx  
Index of the point to set the value for

 point  
new point value to set

[GPoint](/docs/c/Graphics/Graphics_Types/#GPoint) gdraw_command_get_point([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, [uint16_t](/docs/c/Standard_C/#uint16_t) point_idx)

Get the value of a point in a command from the specified index.

##### Note

The index **must** be less than the number of points

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get a point

 point_idx  
The index to get the point for

#### Returns

The point in the [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) specified by point_idx

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void gdraw_command_set_radius([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, [uint16_t](/docs/c/Standard_C/#uint16_t) radius)

Set the radius of a circle command.

##### Note

This only works for commands of type GDrawCommandCircle

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to set the circle radius

 radius  
The radius to set for the circle.

void gdraw_command_set_radius([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, [uint16_t](/docs/c/Standard_C/#uint16_t) radius)

Set the radius of a circle command.

##### Note

This only works for commands of type GDrawCommandCircle

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to set the circle radius

 radius  
The radius to set for the circle.

void gdraw_command_set_radius([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, [uint16_t](/docs/c/Standard_C/#uint16_t) radius)

Set the radius of a circle command.

##### Note

This only works for commands of type GDrawCommandTypeCircle

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to set the circle radius

 radius  
The radius to set for the circle.

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

[uint16_t](/docs/c/Standard_C/#uint16_t) gdraw_command_get_radius([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the radius of a circle command.

##### Note

this only works for commands of typeGDrawCommandCircle.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the circle radius

#### Returns

The radius in pixels if command is of type GDrawCommandCircle

[uint16_t](/docs/c/Standard_C/#uint16_t) gdraw_command_get_radius([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the radius of a circle command.

##### Note

this only works for commands of typeGDrawCommandCircle.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the circle radius

#### Returns

The radius in pixels if command is of type GDrawCommandCircle

[uint16_t](/docs/c/Standard_C/#uint16_t) gdraw_command_get_radius([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Get the radius of a circle command.

##### Note

this only works for commands of type GDrawCommandTypeCircle.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the circle radius

#### Returns

The radius in pixels if command is of type GDrawCommandTypeCircle

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void gdraw_command_set_path_open([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, bool path_open)

Set the path of a stroke command to be open.

##### Note

This only works for commands of type GDrawCommandPath and GDrawCommandPrecisePath

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the path open status

 path_open  
true if path should be hidden

void gdraw_command_set_path_open([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, bool path_open)

Set the path of a stroke command to be open.

##### Note

This only works for commands of type GDrawCommandPath and GDrawCommandPrecisePath

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the path open status

 path_open  
true if path should be hidden

void gdraw_command_set_path_open([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, bool path_open)

Set the path of a stroke command to be open.

##### Note

This only works for commands of type GDrawCommandTypePath and GDrawCommandTypePrecisePath

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the path open status

 path_open  
true if path should be hidden

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

bool gdraw_command_get_path_open([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Return whether a stroke command path is open.

##### Note

This only works for commands of type GDrawCommandPath and GDrawCommandPrecisePath

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the path open status

#### Returns

true if the path is open

bool gdraw_command_get_path_open([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Return whether a stroke command path is open.

##### Note

This only works for commands of type GDrawCommandPath and GDrawCommandPrecisePath

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the path open status

#### Returns

true if the path is open

bool gdraw_command_get_path_open([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Return whether a stroke command path is open.

##### Note

This only works for commands of type GDrawCommandTypePath and GDrawCommandTypePrecisePath

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the path open status

#### Returns

true if the path is open

void gdraw_command_set_hidden([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command, bool hidden)

Set a command as hidden. This command will not be drawn when [gdraw_command_draw](/docs/c/Graphics/Draw_Commands/#gdraw_command_draw) is called with this command.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) for which to set the hidden status

 hidden  
true if command should be hidden

bool gdraw_command_get_hidden([GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* command)

Return whether a command is hidden.

#### Parameters

 command  
[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) from which to get the hidden status

#### Returns

true if command is hidden

void gdraw_command_frame_draw(GContext \* ctx, [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence, [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) \* frame, [GPoint](/docs/c/Graphics/Graphics_Types/#GPoint) offset)

Draw a frame.

#### Parameters

 ctx  
The destination graphics context in which to draw

 sequence  
The sequence from which the frame comes from (this is required)

 frame  
Frame to draw

 offset  
Offset from draw context origin to draw the frame

void gdraw_command_frame_set_duration([GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) \* frame, [uint32_t](/docs/c/Standard_C/#uint32_t) duration)

Set the duration of the frame.

#### Parameters

 frame  
[GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) for which to set the duration

 duration  
duration of the frame in milliseconds

[uint32_t](/docs/c/Standard_C/#uint32_t) gdraw_command_frame_get_duration([GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) \* frame)

Get the duration of the frame.

#### Parameters

 frame  
[GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) from which to get the duration

#### Returns

duration of the frame in milliseconds

[GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* gdraw_command_image_create_with_resource([uint32_t](/docs/c/Standard_C/#uint32_t) resource_id)

Creates a [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) from the specified resource (PDC file)

#### Parameters

 resource_id  
Resource containing data to load and create [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) from.

#### Returns

[GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) pointer if the resource was loaded, NULL otherwise

[GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* gdraw_command_image_clone([GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* image)

Creates a [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) as a copy from a given image.

#### Parameters

 image  
Image to copy.

#### Returns

cloned image or NULL if the operation failed

void gdraw_command_image_destroy([GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* image)

Deletes the [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) structure and frees associated data.

#### Parameters

 image  
Pointer to the image to free (delete)

void gdraw_command_image_draw(GContext \* ctx, [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* image, [GPoint](/docs/c/Graphics/Graphics_Types/#GPoint) offset)

Draw an image.

#### Parameters

 ctx  
The destination graphics context in which to draw

 image  
Image to draw

 offset  
Offset from draw context origin to draw the image

[GSize](/docs/c/Graphics/Graphics_Types/#GSize) gdraw_command_image_get_bounds_size([GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* image)

Get size of the bounding box surrounding all draw commands in the image. This bounding box can be used to set the graphics context or layer bounds when drawing the image.

#### Parameters

 image  
[GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) from which to get the bounding box size

#### Returns

bounding box size

void gdraw_command_image_set_bounds_size([GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* image, [GSize](/docs/c/Graphics/Graphics_Types/#GSize) size)

Set size of the bounding box surrounding all draw commands in the image. This bounding box can be used to set the graphics context or layer bounds when drawing the image.

#### Parameters

 image  
[GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) for which to set the bounding box size

 size  
bounding box size

[GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) \* gdraw_command_image_get_command_list([GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) \* image)

Get the command list of the image.

#### Parameters

 image  
[GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) from which to get the command list

#### Returns

command list

void gdraw_command_list_iterate([GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) \* command_list, [GDrawCommandListIteratorCb](/docs/c/Graphics/Draw_Commands/#GDrawCommandListIteratorCb) handle_command, void \* callback_context)

Iterate over all commands in a command list.

#### Parameters

 command_list  
[GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) over which to iterate

 handle_command  
iterator callback

 callback_context  
context pointer to be passed into the iterator callback

void gdraw_command_list_draw(GContext \* ctx, [GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) \* command_list)

Draw all commands in a command list.

#### Parameters

 ctx  
The destination graphics context in which to draw

 command_list  
list of commands to draw

[GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) \* gdraw_command_list_get_command([GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) \* command_list, [uint16_t](/docs/c/Standard_C/#uint16_t) command_idx)

Get the command at the specified index.

##### Note

the specified index must be less than the number of commands in the list

#### Parameters

 command_list  
[GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) from which to get a command

 command_idx  
index of the command to get

#### Returns

pointer to [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) at the specified index

[uint32_t](/docs/c/Standard_C/#uint32_t) gdraw_command_list_get_num_commands([GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) \* command_list)

Get the number of commands in the list.

#### Parameters

 command_list  
[GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) from which to get the number of commands

#### Returns

number of commands in command list

[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* gdraw_command_sequence_create_with_resource([uint32_t](/docs/c/Standard_C/#uint32_t) resource_id)

Creates a [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from the specified resource (PDC file)

#### Parameters

 resource_id  
Resource containing data to load and create [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from.

#### Returns

[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) pointer if the resource was loaded, NULL otherwise

[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* gdraw_command_sequence_clone([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Creates a [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) as a copy from a given sequence.

#### Parameters

 sequence  
Sequence to copy

#### Returns

cloned sequence or NULL if the operation failed

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void gdraw_command_sequence_destroy([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Deletes the [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) structure and frees associated data.

#### Parameters

 image  
Pointer to the sequence to destroy

void gdraw_command_sequence_destroy([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Deletes the [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) structure and frees associated data.

#### Parameters

 image  
Pointer to the sequence to destroy

void gdraw_command_sequence_destroy([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Deletes the [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) structure and frees associated data.

#### Parameters

 sequence  
Pointer to the sequence to destroy

[GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) \* gdraw_command_sequence_get_frame_by_elapsed([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence, [uint32_t](/docs/c/Standard_C/#uint32_t) elapsed_ms)

Get the frame that should be shown after the specified amount of elapsed time The last frame will be returned if the elapsed time exceeds the total time.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from which to get the frame

 elapsed_ms  
elapsed time in milliseconds

#### Returns

pointer to [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) that should be displayed at the elapsed time

[GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) \* gdraw_command_sequence_get_frame_by_index([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence, [uint32_t](/docs/c/Standard_C/#uint32_t) index)

Get the frame at the specified index.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from which to get the frame

 index  
Index of frame to get

#### Returns

pointer to [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) at the specified index

[GSize](/docs/c/Graphics/Graphics_Types/#GSize) gdraw_command_sequence_get_bounds_size([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Get the size of the bounding box surrounding all draw commands in the sequence. This bounding box can be used to set the graphics context or layer bounds when drawing the frames in the sequence.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from which to get the bounds

#### Returns

bounding box size

void gdraw_command_sequence_set_bounds_size([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence, [GSize](/docs/c/Graphics/Graphics_Types/#GSize) size)

Set size of the bounding box surrounding all draw commands in the sequence. This bounding box can be used to set the graphics context or layer bounds when drawing the frames in the sequence.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) for which to set the bounds

 size  
bounding box size

[uint32_t](/docs/c/Standard_C/#uint32_t) gdraw_command_sequence_get_play_count([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Get the play count of the sequence.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from which to get the play count

#### Returns

play count of sequence

void gdraw_command_sequence_set_play_count([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence, [uint32_t](/docs/c/Standard_C/#uint32_t) play_count)

Set the play count of the sequence.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) for which to set the play count

 play_count  
play count

[uint32_t](/docs/c/Standard_C/#uint32_t) gdraw_command_sequence_get_total_duration([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Get the total duration of the sequence.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from which to get the total duration

#### Returns

total duration of the sequence in milliseconds

[uint32_t](/docs/c/Standard_C/#uint32_t) gdraw_command_sequence_get_num_frames([GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) \* sequence)

Get the number of frames in the sequence.

#### Parameters

 sequence  
[GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) from which to get the number of frames

#### Returns

number of frames in the sequence

[GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) \* gdraw_command_frame_get_command_list([GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) \* frame)

Get the command list of the frame.

#### Parameters

 frame  
[GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) from which to get the command list

#### Returns

command list

## Enum Documentation

enum GDrawCommandType

#### Enumerators

GDrawCommandTypeInvalid  
Invalid draw command type.

GDrawCommandTypePath  
Arbitrary path draw command type.

GDrawCommandTypeCircle  
Circle draw command type.

GDrawCommandTypePrecisePath  
Arbitrary path drawn with sub-pixel precision (1/8th precision)

## Typedef Documentation

typedef struct [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) GDrawCommand

Draw commands are the basic building block of the draw command system, encoding the type of command to draw, the stroke width and color, fill color, and points that define the path (or center of a circle.

typedef struct [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame) GDrawCommandFrame

Draw command frames contain a list of commands to draw for that frame and a duration, indicating the length of time for which the frame should be drawn in an animation sequence. Frames form the building blocks of a [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence), which consists of multiple frames.

typedef struct [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage) GDrawCommandImage

Draw command images contain a list of commands that can be drawn. An image can be loaded from PDC file data.

typedef struct [GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList) GDrawCommandList

Draw command lists contain a list of commands that can be iterated over and drawn all at once.

typedef bool(\* GDrawCommandListIteratorCb)(GDrawCommand \*command, uint32_t index, void \*context)

Callback for iterating over draw command list.

#### Parameters

 command  
current [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand) in iteration

 index  
index of the current command in the list

 context  
context pointer for the iteration operation

#### Returns

true if the iteration should continue after this command is processed

typedef struct [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence) GDrawCommandSequence

Draw command sequences allow the animation of frames over time. Each sequence has a list of frames that can be accessed by the elapsed duration of the animation (not maintained internally) or by index. Sequences can be loaded from PDC file data.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [gdraw_command_draw](/docs/c/Graphics/Draw_Commands/#gdraw_command_draw)
- [gdraw_command_get_type](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_type)
- [gdraw_command_set_fill_color](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_fill_color)
- [gdraw_command_get_fill_color](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_fill_color)
- [gdraw_command_set_stroke_color](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_stroke_color)
- [gdraw_command_get_stroke_color](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_stroke_color)
- [gdraw_command_set_stroke_width](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_stroke_width)
- [gdraw_command_get_stroke_width](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_stroke_width)
- [gdraw_command_get_num_points](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_num_points)
- [gdraw_command_set_point](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_point)
- [gdraw_command_get_point](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_point)
- [gdraw_command_set_radius](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_radius)
- [gdraw_command_get_radius](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_radius)
- [gdraw_command_set_path_open](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_path_open)
- [gdraw_command_get_path_open](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_path_open)
- [gdraw_command_set_hidden](/docs/c/Graphics/Draw_Commands/#gdraw_command_set_hidden)
- [gdraw_command_get_hidden](/docs/c/Graphics/Draw_Commands/#gdraw_command_get_hidden)
- [gdraw_command_frame_draw](/docs/c/Graphics/Draw_Commands/#gdraw_command_frame_draw)
- [gdraw_command_frame_set_duration](/docs/c/Graphics/Draw_Commands/#gdraw_command_frame_set_duration)
- [gdraw_command_frame_get_duration](/docs/c/Graphics/Draw_Commands/#gdraw_command_frame_get_duration)
- [gdraw_command_image_create_with_resource](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_create_with_resource)
- [gdraw_command_image_clone](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_clone)
- [gdraw_command_image_destroy](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_destroy)
- [gdraw_command_image_draw](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_draw)
- [gdraw_command_image_get_bounds_size](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_get_bounds_size)
- [gdraw_command_image_set_bounds_size](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_set_bounds_size)
- [gdraw_command_image_get_command_list](/docs/c/Graphics/Draw_Commands/#gdraw_command_image_get_command_list)
- [gdraw_command_list_iterate](/docs/c/Graphics/Draw_Commands/#gdraw_command_list_iterate)
- [gdraw_command_list_draw](/docs/c/Graphics/Draw_Commands/#gdraw_command_list_draw)
- [gdraw_command_list_get_command](/docs/c/Graphics/Draw_Commands/#gdraw_command_list_get_command)
- [gdraw_command_list_get_num_commands](/docs/c/Graphics/Draw_Commands/#gdraw_command_list_get_num_commands)
- [gdraw_command_sequence_create_with_resource](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_create_with_resource)
- [gdraw_command_sequence_clone](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_clone)
- [gdraw_command_sequence_destroy](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_destroy)
- [gdraw_command_sequence_get_frame_by_elapsed](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_frame_by_elapsed)
- [gdraw_command_sequence_get_frame_by_index](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_frame_by_index)
- [gdraw_command_sequence_get_bounds_size](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_bounds_size)
- [gdraw_command_sequence_set_bounds_size](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_set_bounds_size)
- [gdraw_command_sequence_get_play_count](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_play_count)
- [gdraw_command_sequence_set_play_count](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_set_play_count)
- [gdraw_command_sequence_get_total_duration](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_total_duration)
- [gdraw_command_sequence_get_num_frames](/docs/c/Graphics/Draw_Commands/#gdraw_command_sequence_get_num_frames)
- [gdraw_command_frame_get_command_list](/docs/c/Graphics/Draw_Commands/#gdraw_command_frame_get_command_list)

### [Enums](#enums)

- [GDrawCommandType](/docs/c/Graphics/Draw_Commands/#GDrawCommandType)

### [Typedefs](#typedefs)

- [GDrawCommand](/docs/c/Graphics/Draw_Commands/#GDrawCommand)
- [GDrawCommandFrame](/docs/c/Graphics/Draw_Commands/#GDrawCommandFrame)
- [GDrawCommandImage](/docs/c/Graphics/Draw_Commands/#GDrawCommandImage)
- [GDrawCommandList](/docs/c/Graphics/Draw_Commands/#GDrawCommandList)
- [GDrawCommandListIteratorCb](/docs/c/Graphics/Draw_Commands/#GDrawCommandListIteratorCb)
- [GDrawCommandSequence](/docs/c/Graphics/Draw_Commands/#GDrawCommandSequence)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
