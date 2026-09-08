<!--
source: https://developer.repebble.com/guides/best-practices/modular-app-architecture/
fetched: 2026-09-07T17:42:34Z
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
  - [Building for Every Pebble](/guides/best-practices/building-for-every-pebble/)
  - [Conserving Battery Life](/guides/best-practices/conserving-battery-life/)
  - [Modular App Architecture](/guides/best-practices/modular-app-architecture/)
- [Communication](/guides/communication/)
- [Debugging](/guides/debugging/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

A Basic Project Creating a Module Keep Multiple Windows Separate Keeping Main Clear

# Modular App Architecture

Most Pebble projects (such as a simple watchface) work fine as a single-file project. This means that all the code is located in one `.c` file. However, as the size of a single-file Pebble project increases, it can become harder to keep track of where all the different components are located, and to track down how they interact with each other. For example, a hypothetical app may have many [`Window`](/docs/c/User_Interface/Window/ "Window")s, perform communication over [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") with many types of data items, store and persist a large number of data items, or include components that may be valuable in other projects.

As a first example, the Pebble SDK is already composed of separate modules such as [`Window`](/docs/c/User_Interface/Window/ "Window"), [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer"), [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") etc. The implementation of each is separate from the rest and the interface for developers to use in each module is clearly defined and will rarely change.

This guide aims to provide techniques that can be used to break up such an app. The advantages of a modular approach include:

- App [`Window`](/docs/c/User_Interface/Window/ "Window")s can be kept separate and are easier to work on.

- A clearly defined interface between components ensures internal changes do not affect other modules.

- Modules can be re-used in other projects, or even made into sharable libraries.

- Inter-component variable dependencies do not occur, which can otherwise cause problems if their type or size changes.

- Sub-component complexity is hidden in each module.

- Simpler individual files promote maintainability.

- Modules can be more easily tested.

## A Basic Project

A basic Pebble project starts life with the `new-project` command:

\$ pebble new-project modular-project

This new project will contain the following default file structure. The `modular-project.c` file will contain the entire app, including `main()`, `init()` and `deinit()`, as well as a [`Window`](/docs/c/User_Interface/Window/ "Window") and a child [`TextLayer`](/docs/c/User_Interface/Layers/TextLayer/ "TextLayer").

modular-project/ resources/ src/ modular-project.c package.json wscript

For most projects, this structure is perfectly adequate. When the `.c` file grows to several hundred lines long and incorporates several sub-components with many points of interaction with each other through shared variables, the complexity reaches a point where some new techniques are needed.

## Creating a Module

In this context, a 'module' can be thought of as a C header and source file 'pair', a `.h` file describing the module's interface and a `.c` file containing the actual logic and code. The header contains standard statements to prevent redefinition from being `#include`d multiple times, as well as all the function prototypes the module makes available for other modules to use.

By making a sub-component of the app into a module, the need for messy global variables is removed and a clear interface between them is defined. The files themselves are located in a `modules` directory inside the project's main `src` directory, keeping them in a separate location to other components of the app. Thus the structure of the project with a `data` module added (and explained below) is now this:

modular-project/ resources/ src/ modules/ data.h data.c modular-project.c package.json wscript

The example module's pair of files is shown below. It manages a dynamically allocated array of integers, and includes an interface to setting and getting values from the array. The array itself is private to the module thanks for the [`static`](https://en.wikipedia.org/wiki/Static_(keyword)) keyword. This technique allows other components of the app to call the 'getters' and 'setters' with the correct parameters as per the module's interface, without worrying about the implementation details.

`src/modules/data.h`

\#pragma once // Prevent errors by being included multiple times \#include \<pebble.h\> // Pebble SDK symbols void data_init(int array_length); void data_deinit(); void data_set_array_value(int index, int new_value); int data_get_array_value(int index);

`src/modules/data.c`

\#include "data.h" static int\* s_array; void data_init(int array_length) { if(!s_array) { s_array = (int\*)malloc(array_length \* sizeof(int)); } } void data_deinit() { if(s_array) { free(s_array); s_array = NULL; } } void data_set_array_value(int index, int new_value) { s_array\[index\] = new_value; } int data_get_array_value(int index) { return s_array\[index\]; }

## Keep Multiple Windows Separate

The [`Window Stack`](/docs/c/User_Interface/Window_Stack/ "Window Stack") lifecycle makes the task of keeping each [`Window`](/docs/c/User_Interface/Window/ "Window") separate quite easy. Each one has a `.load` and `.unload` handler which should be used to create and destroy its UI components and other data.

The first step to modularizing the new app is to keep each [`Window`](/docs/c/User_Interface/Window/ "Window") in its own module. The first [`Window`](/docs/c/User_Interface/Window/ "Window")'s code can be moved out of `src/modular-project.c` into a new module in `src/windows/` called 'main_window':

`src/windows/main_window.h`

\#pragma once \#include \<pebble.h\> void main_window_push();

`src/windows/main_window.c`

\#include "main_window.h" static Window \*s_window; static void window_load(Window \*window) { Layer \*window_layer = window_get_root_layer(window); GRect bounds = layer_get_bounds(window_layer); } static void window_unload(Window \*window) { window_destroy(s_window); } void main_window_push() { if(!s_window) { s_window = window_create(); window_set_window_handlers(s_window, (WindowHandlers) { .load = window_load, .unload = window_unload, }); } window_stack_push(s_window, true); }

## Keeping Main Clear

After moving the [`Window`](/docs/c/User_Interface/Window/ "Window") code out of the main `.c` file, it can be safely renamed `main.c` to reflect its contents. This allows the main `.c` file to show a high-level overview of the app as a whole. Simply `#include` the required modules and windows to initialize and deinitialize the rest of the app as necessary:

`src/main.c`

\#include \<pebble.h\> \#include "modules/data.h" \#include "windows/main_window.h" static void init() { const int array_size = 16; data_init(array_size); main_window_push(); } static void deinit() { data_deinit(); } int main() { init(); app_event_loop(); deinit(); }

Thus the structure of the project is now:

modular-project/ resources/ src/ modules/ data.h data.c windows/ main_window.h main_window.c main.c package.json wscript

With this structured approach to organizing the different functional components of an app, the maintainability of the project will not suffer as it grows in size and complexity. A useful module can even be shared and reused as a library, which is preferrable to pasting chunks of code that may have other messy dependencies elsewhere in the project.

You need JavaScript enabled to read and post comments.

### Overview

- [A Basic Project](#a-basic-project)
- [Creating a Module](#creating-a-module)
- [Keep Multiple Windows Separate](#keep-multiple-windows-separate)
- [Keeping Main Clear](#keeping-main-clear)

### Examples

- [Modular App Example](https://github.com/pebble-examples/modular-app-example/)
