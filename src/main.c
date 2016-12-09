#include <pebble.h>
#include "main.h"
#include "messaging.h"
#include "bus_window.h"
#include "error_window.h"
#include "pin_window.h"

Window *loading_window, *bus_window, *error_window;
static TextLayer *loading_text_layer;
static Layer *loading_icon_layer;
static GBitmap *loading_icon_bitmap;
AppTimer *comm_timer;
int i, mult;

static void pin_complete_callback(PIN pin, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Pin was %d %d %d %d %d", pin.digits[0], pin.digits[1], pin.digits[2], pin.digits[3], pin.digits[4]);
  pin_window_pop((PinWindow*)context, true);

	stop_number = 10000 * pin.digits[0] + 1000 * pin.digits[1] + 100 * pin.digits[2] + 10 * pin.digits[3] + pin.digits[4];
	APP_LOG(APP_LOG_LEVEL_INFO, "Stop number: %d", stop_number);

	window_stack_pop(false);
	window_stack_push(loading_window, true);

	// Begin dictionary
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	// Add a key-value pair
	dict_write_uint32(iter, 0, stop_number);

	// Send the message!
	app_message_outbox_send();

	APP_LOG(APP_LOG_LEVEL_INFO, "Starting comm_timer");
	comm_timer = app_timer_register(30000, timeout_callback, NULL);
}

void timeout_callback() {
	window_stack_pop(false);
	window_stack_push(error_window, true);

	text_layer_set_text(message_layer, error_messages[9]);

	size_error_message();
}

static void loading_update_proc(Layer *layer, GContext *ctx) {
	graphics_context_set_compositing_mode(ctx, GCompOpSet);
	graphics_draw_bitmap_in_rect(ctx, loading_icon_bitmap, gbitmap_get_bounds(loading_icon_bitmap));
}

void size_layers() {
	GRect bounds = layer_get_bounds(window_get_root_layer(bus_window));
	GSize route_number_size = text_layer_get_content_size(route_number_layer);
	GSize route_name_size = text_layer_get_content_size(route_name_layer);
	GSize arrival_time_size = text_layer_get_content_size(arrival_time_layer);
	GSize arrives_in_size = text_layer_get_content_size(arrives_in_layer);
	GSize minutes_text_size = text_layer_get_content_size(minutes_text_layer);

	layer_set_frame(text_layer_get_layer(route_number_layer), GRect(0, 3, bounds.size.w, route_number_size.h));
	GRect route_number_grect = layer_get_frame(text_layer_get_layer(route_number_layer));

	layer_set_frame(text_layer_get_layer(route_name_layer), GRect(0, route_number_grect.origin.y + route_number_size.h - 3, bounds.size.w, route_name_size.h));
	GRect route_name_grect = layer_get_frame(text_layer_get_layer(route_name_layer));

	layer_set_frame(text_layer_get_layer(arrives_in_layer), GRect(0, route_name_grect.origin.y + route_name_grect.size.h + 10, bounds.size.w, arrives_in_size.h));
	GRect arrives_in_grect = layer_get_frame(text_layer_get_layer(arrives_in_layer));

	layer_set_frame(text_layer_get_layer(arrival_time_layer), GRect(0, arrives_in_grect.origin.y + arrives_in_size.h - 3, bounds.size.w, arrival_time_size.h));
	GRect arrival_time_grect = layer_get_frame(text_layer_get_layer(arrival_time_layer));

	layer_set_frame(text_layer_get_layer(minutes_text_layer), GRect(0, arrival_time_grect.origin.y + arrival_time_size.h + 5, bounds.size.w, minutes_text_size.w));
}

static void loading_window_load(Window *window) {
	window_set_background_color(window, PBL_IF_BW_ELSE(GColorBlack, GColorVividCerulean));

	loading_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_STOP_SIGN);

	GRect bounds = layer_get_bounds(window_get_root_layer(window)); // Window size
	GRect icon = gbitmap_get_bounds(loading_icon_bitmap); // Icon size

	loading_icon_layer = layer_create(GRect(((bounds.size.w / 2) - (icon.size.w / 2) + 5), 10, bounds.size.w, bounds.size.h)); // Position icon 10px from, centered
	layer_set_update_proc(loading_icon_layer, loading_update_proc);

	layer_add_child(window_get_root_layer(window), loading_icon_layer);


	loading_text_layer = text_layer_create(GRect(0, (icon.size.h + 15), bounds.size.w, bounds.size.h));
	text_layer_set_background_color(loading_text_layer, GColorClear);
	text_layer_set_text_alignment(loading_text_layer, GTextAlignmentCenter);
	text_layer_set_text(loading_text_layer, "Getting next bus");
	text_layer_set_text_color(loading_text_layer, GColorWhite);

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(loading_text_layer));
}

static void loading_window_unload(Window *window) {

}

static void init() {
	PinWindow *pin_window = pin_window_create((PinWindowCallbacks) {
		.pin_complete = pin_complete_callback
	});

	loading_window = window_create();

	window_set_window_handlers(loading_window, (WindowHandlers) {
		.load = loading_window_load,
		.unload = loading_window_unload
	});

	error_window = window_create();

	window_set_window_handlers(error_window, (WindowHandlers) {
		.load = error_window_load,
		.unload = error_window_unload
	});

	window_set_click_config_provider(error_window, error_click_config_provider);

	bus_window = window_create();

	window_set_window_handlers(bus_window, (WindowHandlers) {
		.load = bus_window_load,
		.unload = bus_window_unload
	});

	window_set_click_config_provider(bus_window, bus_click_config_provider);

	pin_window_push(pin_window, true);

	init_appmessage();
}

static void deinit() {

}

int main(void) {
	init();
  app_event_loop();
 	deinit();
}
