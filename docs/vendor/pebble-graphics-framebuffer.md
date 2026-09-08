<!--
source: https://developer.repebble.com/guides/graphics-and-animations/framebuffer-graphics/
fetched: 2026-09-07T17:42:29Z
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
  - [Animations](/guides/graphics-and-animations/animations/)
  - [Drawing Primitives, Images and Text](/guides/graphics-and-animations/drawing-primitives-images-and-text/)
  - [Framebuffer Graphics](/guides/graphics-and-animations/framebuffer-graphics/)
  - [Vector Graphics](/guides/graphics-and-animations/vector-graphics/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

Accessing the Framebuffer Modifying the Framebuffer Data Getting and Setting Pixels Learn More

# Framebuffer Graphics

In the context of a Pebble app, the framebuffer is the data region used to store the contents of the what is shown on the display. Using the [`Graphics Context`](/docs/c/Graphics/Graphics_Context/ "Graphics Context") API allows developers to draw primitive shapes and text, but at a slower speed and with a restricted set of drawing patterns. Getting direct access to the framebuffer allows arbitrary transforms, special effects, and other modifications to be applied to the display contents, and allows drawing at a much greater speed than standard SDK APIs.

## Accessing the Framebuffer

Access to the framebuffer can only be obtained during a [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc"), when redrawing is taking place. When the time comes to update the associated [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer"), the framebuffer can be obtained as a [`GBitmap`](/docs/c/Graphics/Graphics_Types/#GBitmap "GBitmap"):

static void layer_update_proc(Layer \*layer, GContext \*ctx) { // Get the framebuffer GBitmap \*fb = graphics_capture_frame_buffer(ctx); // Manipulate the image data... // Finally, release the framebuffer graphics_release_frame_buffer(ctx, fb); }

> Note: Once obtained, the framebuffer **must** be released back to the app so that it may continue drawing.

The format of the data returned will vary by platform, as will the representation of a single pixel, shown in the table below.

| Platform | Framebuffer Bitmap Format | Pixel Format |
|:--:|----|----|
| Aplite | [`GBitmapFormat1Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat1Bit "GBitmapFormat1Bit") | One bit (black or white) |
| Basalt | [`GBitmapFormat8Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8Bit "GBitmapFormat8Bit") | One byte (two bits per color) |
| Chalk | [`GBitmapFormat8BitCircular`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8BitCircular "GBitmapFormat8BitCircular") | One byte (two bits per color) |
| Diorite | [`GBitmapFormat1Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat1Bit "GBitmapFormat1Bit") | One bit (black or white) |
| Flint | [`GBitmapFormat1Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat1Bit "GBitmapFormat1Bit") | One bit (black or white) |
| Emery | [`GBitmapFormat8Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8Bit "GBitmapFormat8Bit") | One byte (two bits per color) |
| Gabbro | [`GBitmapFormat8Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8Bit "GBitmapFormat8Bit") | One byte (two bits per color) |

Note that although Gabbro has a round display, its framebuffer is a regular rectangular [`GBitmapFormat8Bit`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8Bit "GBitmapFormat8Bit") rather than the packed [`GBitmapFormat8BitCircular`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8BitCircular "GBitmapFormat8BitCircular") used on Chalk.

## Modifying the Framebuffer Data

Once the framebuffer has been captured, the underlying data can be manipulated on a row-by-row or even pixel-by-pixel basis. This data region can be obtained using [`gbitmap_get_data()`](/docs/c/Graphics/Graphics_Types/#gbitmap_get_data "gbitmap_get_data"), but the recommended approach is to make use of [`gbitmap_get_data_row_info()`](/docs/c/Graphics/Graphics_Types/#gbitmap_get_data_row_info "gbitmap_get_data_row_info") objects to cater for framebuffer formats (such as [`GBitmapFormat8BitCircular`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat8BitCircular "GBitmapFormat8BitCircular") on Chalk) where not every row is of the same width. The [`GBitmapDataRowInfo`](/docs/c/Graphics/Graphics_Types/#GBitmapDataRowInfo "GBitmapDataRowInfo") object helps with this by providing a `min_x` and `max_x` value for each `y` used to build it. Using it on platforms with a regular rectangular framebuffer is still safe — `min_x` and `max_x` will simply span the full row width.

To iterate over all rows and columns, safely avoiding those with irregular start and end indices, use two nested loops as shown below. The implementation of `set_pixel_color()` is shown in [*Getting and Setting Pixels*](#getting-and-setting-pixels):

> Note: it is only necessary to call [`gbitmap_get_data_row_info()`](/docs/c/Graphics/Graphics_Types/#gbitmap_get_data_row_info "gbitmap_get_data_row_info") once per row. Calling it more often (such as for every pixel) will incur a sigificant speed penalty.

GRect bounds = layer_get_bounds(layer); // Iterate over all rows for(int y = 0; y \< bounds.size.h; y++) { // Get this row's range and data GBitmapDataRowInfo info = gbitmap_get_data_row_info(fb, y); // Iterate over all visible columns for(int x = info.min_x; x \<= info.max_x; x++) { // Manipulate the pixel at x,y... const GColor random_color = (GColor){ .argb = rand() % 255 }; // ...to be a random color set_pixel_color(info, GPoint(x, y), random_color); } }

## Getting and Setting Pixels

To modify a pixel's value, simply set a new value at the appropriate position in the `data` field of that row's [`GBitmapDataRowInfo`](/docs/c/Graphics/Graphics_Types/#GBitmapDataRowInfo "GBitmapDataRowInfo") object. This will modify the underlying data, and update the display once the frame buffer is released.

This process will be different depending on the [`GBitmapFormat`](/docs/c/Graphics/Graphics_Types/#GBitmapFormat "GBitmapFormat") of the captured framebuffer. On a color platform, each pixel is stored as a single byte. However, on black and white platforms this will be one bit per byte. Using [`memset()`](/docs/c/Standard_C/Memory/#memset "memset") to read or modify the correct pixel on a black and white display requires a bit more logic, shown below:

static GColor get_pixel_color(GBitmapDataRowInfo info, GPoint point) { \#if defined(PBL_COLOR) // Read the single byte color pixel return (GColor){ .argb = info.data\[point.x\] }; \#elif defined(PBL_BW) // Read the single bit of the correct byte uint8_t byte = point.x / 8; uint8_t bit = point.x % 8; return byte_get_bit(&info.data\[byte\], bit) ? GColorWhite : GColorBlack; \#endif }

Setting a pixel value is achieved in much the same way, with different logic depending on the format of the framebuffer on each platform:

static void set_pixel_color(GBitmapDataRowInfo info, GPoint point, GColor color) { \#if defined(PBL_COLOR) // Write the pixel's byte color memset(&info.data\[point.x\], color.argb, 1); \#elif defined(PBL_BW) // Find the correct byte, then set the appropriate bit uint8_t byte = point.x / 8; uint8_t bit = point.x % 8; byte_set_bit(&info.data\[byte\], bit, gcolor_equal(color, GColorWhite) ? 1 : 0); \#endif }

The `byte_get_bit()` and `byte_set_bit()` implementations are shown here for convenience:

static bool byte_get_bit(uint8_t \*byte, uint8_t bit) { return ((\*byte) \>\> bit) & 1; } static void byte_set_bit(uint8_t \*byte, uint8_t bit, uint8_t value) { \*byte ^= (-value ^ \*byte) & (1 \<\< bit); }

## Learn More

To see an example of what can be achieved with direct access to the framebuffer and learn more about the underlying principles, watch the [talk given at the 2014 Developer Retreat](https://www.youtube.com/watch?v=lYoHh19RNy4).

You need JavaScript enabled to read and post comments.

### Overview

- [Accessing the Framebuffer](#accessing-the-framebuffer)
- [Modifying the Framebuffer Data](#modifying-the-framebuffer-data)
- [Getting and Setting Pixels](#getting-and-setting-pixels)
- [Learn More](#learn-more)

### Related SDK Docs

- [Graphics Context](/docs/c/Graphics/Graphics_Context/)
- [GBitmap](/docs/c/Graphics/Graphics_Types/#GBitmap)
- [GBitmapDataRowInfo](/docs/c/Graphics/Graphics_Types/#GBitmapDataRowInfo)
