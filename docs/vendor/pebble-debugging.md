<!--
source: https://developer.repebble.com/guides/debugging/
fetched: 2026-09-07T17:42:35Z
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
- [Debugging](/guides/debugging/)
  - [Common Runtime Errors](/guides/debugging/common-runtime-errors/)
  - [Common Syntax Errors](/guides/debugging/common-syntax-errors/)
  - [Debugging Alloy Apps with xsbug](/guides/debugging/debugging-alloy-with-xsbug/)
  - [Debugging with App Logs](/guides/debugging/debugging-with-app-logs/)
  - [Debugging with GDB](/guides/debugging/debugging-with-gdb/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

# Debugging

When writing apps, everyone makes mistakes. Sometimes a simple typo or omission can lead to all kinds of mysterious behavior or crashes. The guides in this section are aimed at trying to help developers identify and fix a variety of issues that can arise when writing C code (compile-time) or running the compiled app on Pebble (runtime).

There are also a few strategies outlined here, such as app logging and other features of the `pebble` [*Command Line Tool*](/guides/tools-and-resources/pebble-tool/) that can indicate the source of a problem in the vast majority of cases.

## Contents

- [**Common Runtime Errors**](/guides/debugging/common-runtime-errors/) - Examples of commonly encountered runtime problems that cannot be detected at compile time and can usually be fixed by logical thought and experimentation.

- [**Common Syntax Errors**](/guides/debugging/common-syntax-errors/) - Details of common problems encountered when writing C apps for Pebble, and how to resolve them.

- [**Debugging Alloy Apps with xsbug**](/guides/debugging/debugging-alloy-with-xsbug/) - Use the xsbug JavaScript debugger to set breakpoints and inspect your Alloy app while it runs on the watch.

- [**Debugging with App Logs**](/guides/debugging/debugging-with-app-logs/) - How to use the app logs to debug problems with an app, as well as tips on interpreting common run time errors.

- [**Debugging with GDB**](/guides/debugging/debugging-with-gdb/) - How to use GDB to debug a Pebble app in the emulator.

### Related SDK Docs

- [Logging](/docs/c/Foundation/Logging/)
