#include <pebble.h>

static Window *s_main_window;   // This will be a static pointer to the Window object
static TextLayer *s_time_layer; // This will hold the text for the watchface
static GFont s_time_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

/* We create an event service that can access the current time by creating a function
   that listens to the time changing. This can be every second, minute, or hour with 
   expected battery costs if it listens a lot */

static void update_time() {
  // First we need to get a tm struture
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Then we create a long=lived buffer
  static char buffer[] = "00:00";

  // Finally we write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
	// We use the 24 hour format if this option is clicked
	strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
	// And the 12 hour time if thats clicked
	strftime( buffer, sizeof("00:00"), "%I:%M", tick_time );
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

/* To create an instance of Window to assign to the above pointer, we 
   create two pointers to pointer to it. */

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(5, 52, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");

  // Then we add it as a child layer to the root (default) layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);

  // After its changed, we destroy the TextLayer
  text_layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
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

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
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
