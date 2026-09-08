<!--
source: https://developer.repebble.com/docs/c/User_Interface/Window_Stack/
fetched: 2026-09-07T17:42:41Z
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

Jump to item… FUNCTIONS window_stack_push window_stack_pop window_stack_pop_all window_stack_remove window_stack_get_top_window window_stack_contains_window

[](javascript:void(0))

# Window Stack

The multiple window manager

In Pebble OS, the window stack serves as the global manager of what window is presented, ensuring that input events are forwarded to the topmost window. The navigation model of Pebble centers on the concept of a vertical “stack” of windows, similar to mobile app interactions.

In working with the Window Stack API, the basic operations include push and pop. When an app wants to display a new window, it pushes a new window onto the stack. This appears like a window sliding in from the right. As an app is closed, the window is popped off the stack and disappears.

For more complicated operations, involving multiple windows, you can determine which windows reside on the stack, using [window_stack_contains_window()](/docs/c/User_Interface/Window_Stack/#window_stack_contains_window) and remove any specific window, using [window_stack_remove()](/docs/c/User_Interface/Window_Stack/#window_stack_remove).

Refer to the

[User Interface Layers chapter in the Pebble Developer Guides](https://developer.repebble.com/guides/user-interfaces/layers/)

(chapter "Window Stack") for a conceptual overview of the window stack and relevant code examples.

Also see the [WindowHandlers](/docs/c/User_Interface/Window/#WindowHandlers) of a [Window](/docs/c/User_Interface/Window/) for the callbacks that can be added to a window in order to act upon window stack transitions.

## Function Documentation

void window_stack_push(Window \* window, bool animated)

Pushes the given window on the window navigation stack, on top of the current topmost window of the app.

#### Parameters

 window  
The window to push on top

 animated  
Pass in `true` to animate the push using a sliding animation, or `false` to skip the animation.

Window \* window_stack_pop(bool animated)

Pops the topmost window on the navigation stack.

#### Parameters

 animated  
See [window_stack_remove()](/docs/c/User_Interface/Window_Stack/#window_stack_remove)

#### Returns

The window that is popped, or NULL if there are no windows to pop.

void window_stack_pop_all(const bool animated)

Pops all windows. See [window_stack_remove()](/docs/c/User_Interface/Window_Stack/#window_stack_remove) for a description of the `animated` parameter and notes.

bool window_stack_remove(Window \* window, bool animated)

Removes a given window from the window stack that belongs to the app task.

##### Note

If there are no windows for the app left on the stack, the app will be killed by the system, shortly. To avoid this, make sure to push another window shortly after or before removing the last window.

#### Parameters

 window  
The window to remove. If the window is NULL or if it is not on the stack, this function is a no-op.

 animated  
Pass in `true` to animate the removal of the window using a side-to-side sliding animation to reveal the next window. This is only used in case the window happens to be on top of the window stack (thus visible).

#### Returns

True if window was successfully removed, false otherwise.

Window \* window_stack_get_top_window(void)

Gets the topmost window on the stack that belongs to the app.

#### Returns

The topmost window on the stack that belongs to the app or NULL if no app window could be found.

bool window_stack_contains_window(Window \* window)

Checks if the window is on the window stack.

#### Parameters

 window  
The window to look for on the window stack

#### Returns

true if the window is currently on the window stack.

[Need some help?](javascript:void(0))

### [Functions](#functions)

- [window_stack_push](/docs/c/User_Interface/Window_Stack/#window_stack_push)
- [window_stack_pop](/docs/c/User_Interface/Window_Stack/#window_stack_pop)
- [window_stack_pop_all](/docs/c/User_Interface/Window_Stack/#window_stack_pop_all)
- [window_stack_remove](/docs/c/User_Interface/Window_Stack/#window_stack_remove)
- [window_stack_get_top_window](/docs/c/User_Interface/Window_Stack/#window_stack_get_top_window)
- [window_stack_contains_window](/docs/c/User_Interface/Window_Stack/#window_stack_contains_window)

#### Getting Help

Do you have questions about the Pebble SDK?

Do you need some help understanding something on this page?

You can either take advantage of our awesome developer community and [check out the SDK Help forums](https://forum.repebble.com/c/developers-ask-questions-and-get-help), or you can [join us on the Discord](https://discordapp.com/invite/aRUAYFN)!
