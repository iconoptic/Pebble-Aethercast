<!--
source: https://developer.repebble.com/docs/c/Graphics/Graphics_Context/
fetched: 2026-09-07T17:42:38Z
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

Jump to item… FUNCTIONS graphics_context_set_stroke_color graphics_context_set_fill_color graphics_context_set_text_color graphics_context_set_compositing_mode graphics_context_set_antialiased graphics_context_set_stroke_width

[](javascript:void(0))

# Graphics Context

The "canvas" into which an application draws

The Pebble OS graphics engine, inspired by several notable graphics systems, including Apple’s Quartz 2D and its predecessor QuickDraw, provides your app with a canvas into which to draw, namely, the graphics context. A graphics context is the target into which graphics functions can paint, using Pebble drawing routines (see [Drawing Primitives](/docs/c/Graphics/Drawing_Primitives/), [Drawing Paths](/docs/c/Graphics/Drawing_Paths/) and [Drawing Text](/docs/c/Graphics/Drawing_Text/)).

A graphics context holds a reference to the bitmap into which to paint. It also holds the current drawing state, like the current fill color, stroke color, clipping box, drawing box, compositing mode, and so on. The GContext struct is the type representing the graphics context.

For drawing in your Pebble watchface or watchapp, you won't need to create a GContext yourself. In most cases, it is provided by Pebble OS as an argument passed into a render callback (the .update_proc of a Layer).

Your app can’t call drawing functions at any given point in time: Pebble OS will request your app to render. Typically, your app will be calling out to graphics functions in the .update_proc callback of a Layer.

## Function Documentation

void graphics_context_set_stroke_color(GContext \* ctx, GColor color)

Sets the current stroke color of the graphics context.

#### Parameters

 ctx  
The graphics context onto which to set the stroke color

 color  
The new stroke color

void graphics_context_set_fill_color(GContext \* ctx, GColor color)

Sets the current fill color of the graphics context.

#### Parameters

 ctx  
The graphics context onto which to set the fill color

 color  
The new fill color

void graphics_context_set_text_color(GContext \* ctx, GColor color)

Sets the current text color of the graphics context.

#### Parameters

 ctx  
The graphics context onto which to set the text color

 color  
The new text color

- [SDK 3](javascript:void(0);)
- [SDK 4](javascript:void(0);)
- [SDK 4.9+](javascript:void(0);)

void graphics_context_set_compositing_mode(GContext \* ctx, [GCompOp](/docs/c/Graphics/Graphics_Types/#GCompOp) mode)

Sets the current bitmap compositing mode of the graphics context.

##### Note

At the moment, this only affects the bitmaps drawing operations - [graphics_draw_bitmap_in_rect()](/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect), [graphics_draw_rotated_bitmap](/docs/c/Graphics/Drawing_Primitives/#graphics_draw_rotated_bitmap), and anything that uses those APIs -, but it currently does not affect the filling or stroking operations.

#### Parameters

 ctx  
The graphics context onto which to set the compositing mode

 mode  
The new compositing mode

#### See Also

[GCompOp](/docs/c/Graphics/Graphics_Types/#GCompOp)\
[bitmap_layer_set_compositing_mode()](/docs/c/User_Interface/Layers/BitmapLayer/#bitmap_layer_set_compositing_mode)

void graphics_context_set_compositing_mode(GContext \* ctx, [GCompOp](/docs/c/Graphics/Graphics_Types/#GCompOp) mode)

Sets the current bitmap compositing mode of the graphics context.

##### Note

At the moment, this only affects the bitmaps drawing operations - [graphics_draw_bitmap_in_rect()](/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect), [graphics_draw_rotated_bitmap](/docs/c/Graphics/Drawing_Primitives/#graphics_draw_rotated_bitmap), and anything that uses those APIs -, but it currently does not affect the filling or stroking operations.

#### Parameters

 ctx  
The graphics context onto which to set the compositing mode

 mode  
The new compositing mode

#### See Also

[GCompOp](/docs/c/Graphics/Graphics_Types/#GCompOp)\
[bitmap_layer_set_compositing_mode()](/docs/c/User_Interface/Layers/BitmapLayer/#bitmap_layer_set_compositing_mode)

void graphics_context_set_compositing_mode(GContext \* ctx, [GCompOp](/docs/c/Graphics/Graphics_Types/#GCompOp) mode)

Sets the current bitmap compositing mode of the graphics context. The default mode is GCompOpAssign i.e. bitmap transparency disabled.

##### Note

At the moment, this only affects the bitmaps drawing operations - [graphics_draw_bitmap_in_rect()](/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect), [graphics_draw_rotated_bitmap](/docs/c/Graphics/Drawing_Primitives/#graphics_draw_rotated_bitmap), and anything that uses those APIs -, but it currently does not affect the filling or stroking operations.

#### Parameters

 ctx  
The graphics context onto which to set the compositing mode

 mode  
The new compositing mode

#### See Also

[GCompOp](/docs/c/Graphics/Graphics_Types/#GCompOp)\
[bitmap_layer_set_compositing_mode()](/docs/c/User_Interface/Layers/BitmapLayer/#bitmap_layer_set_compositing_mode)

void graphics_context_set_antialiased(GContext \* ctx, bool enable)

Sets whether antialiasing is applied to stroke drawing.

##### Note

Default value is true.

#### Parameters

 ctx  
The graphics context onto which to set the antialiasing

 enable  
True = antialiasing enabled, False = antialiasing disabled

void graphics_context_set_stroke_width(GContext \* ctx, uint8_t stroke_width)

Sets the width of the stroke for drawing routines.

##### Note

If stroke width of zero is passed, it will be ignored and will not change the value stored in GContext. Currently, only odd stroke_width values are supported. If an even value is passed in, the value will be stored as is, but the drawing routines will round down to the previous integral value when drawing. Default value is 1.

#### Parameters

 ctx  
The graphics context onto which to set the stroke width

 stroke_width  
Width in pixels of the stroke.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [graphics_context_set_stroke_color](/docs/c/Graphics/Graphics_Context/#graphics_context_set_stroke_color)
- [graphics_context_set_fill_color](/docs/c/Graphics/Graphics_Context/#graphics_context_set_fill_color)
- [graphics_context_set_text_color](/docs/c/Graphics/Graphics_Context/#graphics_context_set_text_color)
- [graphics_context_set_compositing_mode](/docs/c/Graphics/Graphics_Context/#graphics_context_set_compositing_mode)
- [graphics_context_set_antialiased](/docs/c/Graphics/Graphics_Context/#graphics_context_set_antialiased)
- [graphics_context_set_stroke_width](/docs/c/Graphics/Graphics_Context/#graphics_context_set_stroke_width)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
