<!--
source: https://developer.repebble.com/guides/graphics-and-animations/drawing-primitives-images-and-text/
fetched: 2026-09-07T17:42:28Z
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

Obtaining a Drawing Context Drawing Primitive Shapes - Lines - Rectangles - Circles Bitmaps Drawing Text

# Drawing Primitives, Images and Text

While [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") types such as [`TextLayer`](/docs/c/User_Interface/Layers/TextLayer/ "TextLayer") and [`BitmapLayer`](/docs/c/User_Interface/Layers/BitmapLayer/ "BitmapLayer") allow easy rendering of text and bitmaps, more precise drawing can be achieved through the use of the [`Graphics Context`](/docs/c/Graphics/Graphics_Context/ "Graphics Context") APIs. Custom drawing of primitive shapes such as line, rectangles, and circles is also supported. Clever use of these functions can remove the need to pre-prepare bitmap images for many UI elements and icons.

## Obtaining a Drawing Context

All custom drawing requires a [`GContext`](/docs/c/Graphics/Graphics_Types/#GContext "GContext") instance. These cannot be created, and are only available inside a [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc"). This update procedure is simply a function that is called when a [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") is to be rendered, and is defined by the developer as opposed to the system. For example, a [`BitmapLayer`](/docs/c/User_Interface/Layers/BitmapLayer/ "BitmapLayer") is simply a [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") with a [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc") abstracted away for convenience by the SDK.

First, create the [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") that will have a custom drawing procedure:

static Layer \*s_canvas_layer;

Allocate the [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") during [`Window`](/docs/c/User_Interface/Window/ "Window") creation:

GRect bounds = layer_get_bounds(window_get_root_layer(window)); // Create canvas layer s_canvas_layer = layer_create(bounds);

Next, define the [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc") according to the function specification:

static void canvas_update_proc(Layer \*layer, GContext \*ctx) { // Custom drawing happens here! }

Assign this procedure to the canvas layer and add it to the [`Window`](/docs/c/User_Interface/Window/ "Window") to make it visible:

// Assign the custom drawing procedure layer_set_update_proc(s_canvas_layer, canvas_update_proc); // Add to Window layer_add_child(window_get_root_layer(window), s_canvas_layer);

From now on, every time the [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") needs to be redrawn (for example, if other layer geometry changes), the [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc") will be called to allow the developer to draw it. It can also be explicitly marked for redrawing at the next opportunity:

// Redraw this as soon as possible layer_mark_dirty(s_canvas_layer);

## Drawing Primitive Shapes

The [`Graphics Context`](/docs/c/Graphics/Graphics_Context/ "Graphics Context") API allows drawing and filling of lines, rectangles, circles, and arbitrary paths. For each of these, the colors of the output can be set using the appropriate function:

// Set the line color graphics_context_set_stroke_color(ctx, GColorRed); // Set the fill color graphics_context_set_fill_color(ctx, GColorBlue);

In addition, the stroke width and antialiasing mode can also be changed:

// Set the stroke width (must be an odd integer value) graphics_context_set_stroke_width(ctx, 5); // Disable antialiasing (enabled by default where available) graphics_context_set_antialiased(ctx, false);

### Lines

Drawing a simple line requires only the start and end positions, expressed as [`GPoint`](/docs/c/Graphics/Graphics_Types/#GPoint "GPoint") values:

GPoint start = GPoint(10, 10); GPoint end = GPoint(40, 60); // Draw a line graphics_draw_line(ctx, start, end);

### Rectangles

Drawing a rectangle requires a bounding [`GRect`](/docs/c/Graphics/Graphics_Types/#GRect "GRect"), as well as other parameters if it is to be filled:

GRect rect_bounds = GRect(10, 10, 40, 60); // Draw a rectangle graphics_draw_rect(ctx, rect_bounds); // Fill a rectangle with rounded corners int corner_radius = 10; graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

It is also possible to draw a rounded unfilled rectangle:

// Draw outline of a rounded rectangle graphics_draw_round_rect(ctx, rect_bounds, corner_radius);

### Circles

Drawing a circle requries its center [`GPoint`](/docs/c/Graphics/Graphics_Types/#GPoint "GPoint") and radius:

GPoint center = GPoint(25, 25); uint16_t radius = 50; // Draw the outline of a circle graphics_draw_circle(ctx, center, radius); // Fill a circle graphics_fill_circle(ctx, center, radius);

In addition, it is possble to draw and fill arcs. In these cases, the [`GOvalScaleMode`](/docs/c/Graphics/Drawing_Primitives/#GOvalScaleMode "GOvalScaleMode") determines how the shape is adjusted to fill the rectangle, and the cartesian angle values are transformed to preserve accuracy:

int32_t angle_start = DEG_TO_TRIGANGLE(0); int32_t angle_end = DEG_TO_TRIGANGLE(45); // Draw an arc graphics_draw_arc(ctx, rect_bounds, GOvalScaleModeFitCircle, angle_start, angle_end);

Lastly, a filled circle with a sector removed can also be drawn in a similar manner. The value of `inset_thickness` determines the inner inset size that is removed from the full circle:

uint16_t inset_thickness = 10; // Fill a radial section of a circle graphics_fill_radial(ctx, rect_bounds, GOvalScaleModeFitCircle, inset_thickness, angle_start, angle_end);

For more guidance on using round elements in apps, watch the presentation given at the 2015 Developer Retreat on [developing for Pebble Time Round](https://www.youtube.com/watch?v=3a1V4n9HDvY).

## Bitmaps

Manually drawing [`GBitmap`](/docs/c/Graphics/Graphics_Types/#GBitmap "GBitmap") images with the [`Graphics Context`](/docs/c/Graphics/Graphics_Context/ "Graphics Context") API is a simple task, and has much in common with the alternative approach of using a [`BitmapLayer`](/docs/c/User_Interface/Layers/BitmapLayer/ "BitmapLayer") (which provides additional convenience funcionality).

The first step is to load the image data from resources (read [*Images*](/guides/app-resources/images/) to learn how to include images in a Pebble project):

static GBitmap \*s_bitmap;

// Load the image data s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_EXAMPLE_IMAGE);

When the appropriate [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc") is called, draw the image inside the desired rectangle:

> Note: Unlike [`BitmapLayer`](/docs/c/User_Interface/Layers/BitmapLayer/ "BitmapLayer"), the image will be drawn relative to the [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer")'s origin, and not centered.

// Get the bounds of the image GRect bitmap_bounds = gbitmap_get_bounds(s_bitmap); // Set the compositing mode (GCompOpSet is required for transparency) graphics_context_set_compositing_mode(ctx, GCompOpSet); // Draw the image graphics_draw_bitmap_in_rect(ctx, s_bitmap, bitmap_bounds);

Once the image is no longer needed (i.e.: the app is exiting), free the data:

// Destroy the image data gbitmap_destroy(s_bitmap);

## Drawing Text

Similar to the [`TextLayer`](/docs/c/User_Interface/Layers/TextLayer/ "TextLayer") UI component, a [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc") can also be used to draw text. Advantages can include being able to draw in multiple fonts with only one [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer") and combining text with other drawing operations.

The first operation to perform inside the [`LayerUpdateProc`](/docs/c/User_Interface/Layers/#LayerUpdateProc "LayerUpdateProc") is to get or load the font to be used for drawing and set the text's color:

// Load the font GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD); // Set the color graphics_context_set_text_color(ctx, GColorBlack);

Next, determine the bounds that will guide the text's position and overflow behavior. This can either be the size of the [`Layer`](/docs/c/User_Interface/Layers/#Layer "Layer"), or a more precise bounds of the text itself. This information can be useful for drawing multiple text items after one another with automatic spacing.

char \*text = "Example test string for the Developer Website guide!"; // Determine a reduced bounding box GRect layer_bounds = layer_get_bounds(layer); GRect bounds = GRect(layer_bounds.origin.x, layer_bounds.origin.y, layer_bounds.size.w / 2, layer_bounds.size.h); // Calculate the size of the text to be drawn, with restricted space GSize text_size = graphics_text_layout_get_content_size(text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);

Finally, the text can be drawn into the appropriate bounding rectangle:

// Draw the text graphics_draw_text(ctx, text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

You need JavaScript enabled to read and post comments.

### Overview

- [Obtaining a Drawing Context](#obtaining-a-drawing-context)
- [Drawing Primitive Shapes](#drawing-primitive-shapes)
- [Lines](#lines)
- [Rectangles](#rectangles)
- [Circles](#circles)
- [Bitmaps](#bitmaps)
- [Drawing Text](#drawing-text)
