#include "detail_window.h"

#define ANIM_DURATION_MS 250

static Window *s_window;
static Layer *s_content;
static DetailSpec s_spec;
static bool s_closing;

static void prv_content_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  if (s_spec.draw) {
    s_spec.draw(ctx, bounds, s_spec.context);
  }
}

static void prv_anim_stopped(Animation *animation, bool finished, void *context) {
  animation_destroy(animation);
  if (context) {
    // Non-NULL context marks the closing animation - now safe to actually
    // tear the window down (no default slide-out, we already animated).
    window_stack_remove(s_window, false);
  }
}

static void prv_animate(GRect from, GRect to, bool closing) {
  layer_set_frame(s_content, from);
  PropertyAnimation *prop_anim = property_animation_create_layer_frame(s_content, &from, &to);
  Animation *anim = property_animation_get_animation(prop_anim);
  animation_set_duration(anim, ANIM_DURATION_MS);
  animation_set_curve(anim, AnimationCurveEaseOut);
  animation_set_handlers(anim, (AnimationHandlers){ .stopped = prv_anim_stopped },
                          closing ? (void *)1 : NULL);
  animation_schedule(anim);
}

static void prv_close(void) {
  if (s_closing) {
    return;
  }
  s_closing = true;
  GRect full = layer_get_bounds(window_get_root_layer(s_window));
  prv_animate(full, s_spec.from_rect, true);
}

static void prv_back_click(ClickRecognizerRef recognizer, void *context) {
  prv_close();
}

static void prv_up_click(ClickRecognizerRef recognizer, void *context) {
  if (s_spec.click) {
    s_spec.click(BUTTON_ID_UP, s_spec.context);
  }
}

static void prv_down_click(ClickRecognizerRef recognizer, void *context) {
  if (s_spec.click) {
    s_spec.click(BUTTON_ID_DOWN, s_spec.context);
  }
}

static void prv_select_click(ClickRecognizerRef recognizer, void *context) {
  if (s_spec.click) {
    s_spec.click(BUTTON_ID_SELECT, s_spec.context);
  }
}

static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, prv_back_click);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 150, prv_up_click);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 150, prv_down_click);
  window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click);
}

static void prv_window_appear(Window *window) {
  (void)window;
  if (s_spec.appear) {
    s_spec.appear(s_spec.context);
  }
}

static void prv_window_disappear(Window *window) {
  (void)window;
  if (s_spec.disappear) {
    s_spec.disappear(s_spec.context);
  }
}

static void prv_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root);
  s_content = layer_create(s_spec.from_rect);
  layer_set_update_proc(s_content, prv_content_update_proc);
  layer_add_child(root, s_content);
  window_set_click_config_provider(window, prv_click_config_provider);
  s_closing = false;
  prv_animate(s_spec.from_rect, bounds, false);
}

static void prv_window_unload(Window *window) {
  layer_destroy(s_content);
  s_content = NULL;
  // detail_window_push creates a Window on every open; free it here so
  // open/close cycles do not leak (aplite heap is ~8 KB).
  window_destroy(window);
  s_window = NULL;
}

void detail_window_push(const DetailSpec *spec) {
  s_spec = *spec;
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = prv_window_load,
    .unload = prv_window_unload,
    .appear = prv_window_appear,
    .disappear = prv_window_disappear,
  });
  window_stack_push(s_window, false);
}

void detail_window_mark_dirty(void) {
  if (s_content) {
    layer_mark_dirty(s_content);
  }
}
