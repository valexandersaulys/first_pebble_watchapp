#include <pebble.h>

static Window *s_main_window;   // This will be a static pointer to the Window object
static TextLayer *s_time_layer; // This will hold the text for the watchface


/* To create an instance of Window to assign to the above pointer, we 
   create two pointers to pointer to it. */

static void main_window_load(Window *window) {
  // First the time TextLayer is created
  s_time_layer = text_layer_create(GRect(0,55,144,50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");

  // Then we so some playing around with the text face
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Then we add it as a child layer to the root (default) layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {

}


static void init() {
  // First we create the main Window element and assign it to the pointer
  s_main_window = window_create();

  // Then we set handlers to manage the elements within the Window
  window_set_window_handlers( s_main_window, (WindowHandlers) {
	  .load = main_window_load,
	  .unload = main_window_unload
  });

  // Then we tell the Pebble to show the window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // We inform the pebble what to do when ending the app
  // So we say to destroy the window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
