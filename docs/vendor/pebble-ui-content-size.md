<!--
source: https://developer.repebble.com/guides/user-interfaces/content-size/
fetched: 2026-09-07T17:42:31Z
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
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)
  - [App Configuration](/guides/user-interfaces/app-configuration/)
  - [App Exit Reason](/guides/user-interfaces/app-exit-reason/)
  - [AppGlance C API](/guides/user-interfaces/appglance-c/)
  - [AppGlance REST API](/guides/user-interfaces/appglance-rest/)
  - [AppGlance in PebbleKit JS](/guides/user-interfaces/appglance-pebblekit-js/)
  - [Content Size](/guides/user-interfaces/content-size/)
  - [Layers](/guides/user-interfaces/layers/)
  - [Round App UI](/guides/user-interfaces/round-app-ui/)
  - [Unobstructed Area](/guides/user-interfaces/unobstructed-area/)

[](javascript:void(0);)

Detecting ContentSize Adapting Layouts Additional Considerations

# Content Size

The [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size) API, added in SDK 4.2, allows developers to dynamically adapt their watchface and watchapp design based upon the system `Text Size` preference (*Settings \> Notifications \> Text Size*).

While this allows developers to create highly accessible designs, it also serves to provide a mechanism for creating designs which are less focused upon screen size, and more focused upon content size.

![ContentSize](/assets/images/guides/content-size/anim.gif)

The `Text Size` setting displays the following options on all platforms:

- Small
- Medium
- Large

Whereas, the [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size) API will return different content sizes based on the `Text Size` setting, varying by platform. The list of content sizes is:

- Small
- Medium
- Large
- Extra Large

An example of the varying content sizes:

- `Text Size`: `small` on `Basalt` is `ContentSize`: `small`
- `Text Size`: `small` on `Emery` is `ContentSize`: `medium`

The following table describes the relationship between `Text Size`, `Platform` and `ContentSize`:

| Platform | Text Size: Small | Text Size: Medium | Text Size: Large |
|----|----|----|----|
| Aplite, Basalt, Chalk, Diorite, Flint | ContentSize: Small | ContentSize: Medium | ContentSize: Large |
| Emery | ContentSize: Medium | ContentSize: Large | ContentSize: Extra Large |

> *At present the Text Size setting only affects notifications and some system UI components, but other system UI components will be updated to support ContentSize in future versions.*

We highly recommend that developers begin to build and update their applications with consideration for [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size) to provide the best experience to users.

## Detecting ContentSize

In order to detect the current [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size) developers can use the [`preferred_content_size()`](/docs/c/User_Interface/Preferences/#preferred_content_size "preferred_content_size") function.

The [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size) will never change during runtime, so it's perfectly acceptable to check this once during `init()`.

static PreferredContentSize s_content_size; void init() { s_content_size = preferred_content_size(); // ... }

## Adapting Layouts

There are a number of different approaches to adapting the screen layout based upon content size. You could change font sizes, show or hide design elements, or even present an entirely different UI.

In the following example, we will change font sizes based on the [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size)

static TextLayer \*s_text_layer; static PreferredContentSize s_content_size; void init() { s_content_size = preferred_content_size(); // ... switch (s_content_size) { case PreferredContentSizeMedium: // Use a medium font text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)); break; case PreferredContentSizeLarge: case PreferredContentSizeExtraLarge: // Use a large font text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)); break; default: // Use a small font text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD)); break; } // ... }

## Additional Considerations

When developing an application which dynamically adjusts based on the [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size) setting, try to avoid using fixed widths and heights. Calculate coordinates and dimensions based upon the size of the root layer, [`UnobstructedArea`](/docs/c/User_Interface/UnobstructedArea/ "UnobstructedArea") and [ContentSize](/docs/c/User_Interface/Preferences/#preferred_content_size)

You need JavaScript enabled to read and post comments.

### Overview

- [Detecting ContentSize](#detecting-contentsize)
- [Adapting Layouts](#adapting-layouts)
- [Additional Considerations](#additional-considerations)

### Related SDK Docs

- []()
- [UnobstructedArea](/docs/c/User_Interface/UnobstructedArea/)

### Examples

- [Simple Example](https://github.com/pebble-examples/feature-content-size)
