#include <pebble.h>
#include "messaging.h"
#include "main.h"

Window *main_window, *loading_window, *bus_window;
static TextLayer *slot_one, *slot_two, *slot_three, *slot_four, *slot_five, *loading_text_layer;
TextLayer *route_name_layer, *route_number_layer, *arrival_time_layer;
static Layer *loading_icon_layer;
static GBitmap *loading_icon_bitmap;

static char slot_one_buffer[3], slot_two_buffer[3], slot_three_buffer[3], slot_four_buffer[3], slot_five_buffer[3];

static int SLOTS[5] = {
	0,
	0,
	0,
	0,
	0
};

static int current_slot = 0;
int stop_number;

static void increment_slot(bool inc_up) {
	int slot_value = SLOTS[current_slot];
	APP_LOG(APP_LOG_LEVEL_INFO, "Slot %d is %d", current_slot, slot_value);
	
	if (slot_value == 9 && inc_up == true) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Setting slot to 0");
		slot_value = 0;
	}	else if (slot_value == 0 && inc_up == false) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Setting slot to 9");
		slot_value = 9;
	} else if (inc_up == true) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Moving slot up");
		slot_value = slot_value + 1;
	} else if (inc_up == false) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Moving slot down");
		slot_value = slot_value - 1;
	}
	
	SLOTS[current_slot] = slot_value;
	APP_LOG(APP_LOG_LEVEL_INFO, "slot_value is %d", slot_value);
	APP_LOG(APP_LOG_LEVEL_INFO, "Stored slot is %d", SLOTS[current_slot]);
	
	
	if (current_slot == 0) {
		snprintf(slot_one_buffer, sizeof(slot_one_buffer), "%d", slot_value);
		text_layer_set_text(slot_one, slot_one_buffer);
	} else if (current_slot == 1) {
		snprintf(slot_two_buffer, sizeof(slot_two_buffer), "%d", slot_value);
		text_layer_set_text(slot_two, slot_two_buffer);
	} else if (current_slot == 2) {
		snprintf(slot_three_buffer, sizeof(slot_three_buffer), "%d", slot_value);
		text_layer_set_text(slot_three, slot_three_buffer);
	} else if (current_slot == 3) {
		snprintf(slot_four_buffer, sizeof(slot_four_buffer), "%d", slot_value);
		text_layer_set_text(slot_four, slot_four_buffer);
	} else if (current_slot == 4) {
		snprintf(slot_five_buffer, sizeof(slot_five_buffer), "%d", slot_value);
		text_layer_set_text(slot_five, slot_five_buffer);
	}
}

static void up_click(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Up button clicked!");
	increment_slot(true);
}

static void down_click(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Down button clicked!");
	increment_slot(false);
}

static void update_indicator() {
	text_layer_set_background_color(slot_one, GColorWhite);
	text_layer_set_background_color(slot_two, GColorWhite);
	text_layer_set_background_color(slot_three, GColorWhite);
	text_layer_set_background_color(slot_four, GColorWhite);
	text_layer_set_background_color(slot_five, GColorWhite);
	
	if (current_slot == 0) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 0");
		text_layer_set_background_color(slot_one, GColorVividCerulean);
	} else if (current_slot == 1) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 1");
		text_layer_set_background_color(slot_two, GColorVividCerulean);
	} else if (current_slot == 2) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 2");
		text_layer_set_background_color(slot_three, GColorVividCerulean);
	} else if (current_slot == 3) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 3");
		text_layer_set_background_color(slot_four, GColorVividCerulean);
	} else if (current_slot == 4) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 4");
		text_layer_set_background_color(slot_five, GColorVividCerulean);
	}
}

static void next_click(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Select button clicked!");
	if (current_slot == 4) {
		stop_number = 10000 * SLOTS[0] + 1000 * SLOTS[1] + 100 * SLOTS[2] + 10 * SLOTS[3] + SLOTS[4];
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
	} else {
		current_slot = current_slot + 1;
	}
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Current slot is %d", current_slot);
	update_indicator();
}


static void click_config_provider(void *context) {
	ButtonId next = BUTTON_ID_SELECT;
	ButtonId up = BUTTON_ID_UP;
	ButtonId down = BUTTON_ID_DOWN;
	
	window_single_click_subscribe(up, up_click);
	window_single_click_subscribe(down, down_click);
	window_single_click_subscribe(next, next_click);
}

static void loading_update_proc(Layer *layer, GContext *ctx) {
	graphics_context_set_compositing_mode(ctx, GCompOpSet);
	graphics_draw_bitmap_in_rect(ctx, loading_icon_bitmap, gbitmap_get_bounds(loading_icon_bitmap));
}

static void main_window_load(Window *window) {
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	
	slot_one = text_layer_create(GRect(0, 0, 144, 168));
	text_layer_set_font(slot_one, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	
	slot_two = text_layer_create(GRect(10, 0, 144, 168));
	text_layer_set_font(slot_two, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	
	slot_three = text_layer_create(GRect(20, 0, 144, 168));
	text_layer_set_font(slot_three, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	
	slot_four = text_layer_create(GRect(30, 0, 144, 168));
	text_layer_set_font(slot_four, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	
	slot_five = text_layer_create(GRect(40, 0, 144, 168));
	text_layer_set_font(slot_five, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	
	text_layer_set_text(slot_one, "0");
	text_layer_set_text(slot_two, "0");
	text_layer_set_text(slot_three, "0");
	text_layer_set_text(slot_four, "0");
	text_layer_set_text(slot_five, "0");
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(slot_one));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(slot_two));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(slot_three));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(slot_four));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(slot_five));
	
	GSize slot_size = text_layer_get_content_size(slot_one);
	GSize centre_slot = text_layer_get_content_size(slot_three);
	int centre = bounds.size.w / 2 - (slot_size.w / 2);
	int offset = 3;
	
	layer_set_frame(text_layer_get_layer(slot_one), GRect(centre - (slot_size.w * 2) - (offset * 2), bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h));
	layer_set_frame(text_layer_get_layer(slot_two), GRect(centre - slot_size.w - offset, bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h));
	layer_set_frame(text_layer_get_layer(slot_three), GRect(bounds.size.w / 2 - (slot_size.w / 2), bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h));
	layer_set_frame(text_layer_get_layer(slot_four), GRect(centre + slot_size.w + offset, bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h));
	layer_set_frame(text_layer_get_layer(slot_five), GRect(centre + (slot_size.w * 2) + (offset * 2), bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h));

	update_indicator();
}

static void main_window_unload(Window *window) {
	
}

static void loading_window_load(Window *window) {
	window_set_background_color(window, GColorVividCerulean);

	loading_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_STOP_SIGN);

	GRect bounds = layer_get_bounds(window_get_root_layer(window)); // Window size
	GRect icon = gbitmap_get_bounds(loading_icon_bitmap); // Icon size

	loading_icon_layer = layer_create(GRect(((bounds.size.w / 2) - (icon.size.w / 2) + 5), 10, bounds.size.w, bounds.size.h)); // Position icon 10px from, centered
	layer_set_update_proc(loading_icon_layer, loading_update_proc);

	layer_add_child(window_get_root_layer(window), loading_icon_layer);


	loading_text_layer = text_layer_create(GRect(0, (icon.size.h + 15), 144, 168));
	text_layer_set_background_color(loading_text_layer, GColorClear);
	text_layer_set_text_alignment(loading_text_layer, GTextAlignmentCenter);
	text_layer_set_text(loading_text_layer, "Finding stops");
	text_layer_set_text_color(loading_text_layer, GColorWhite);

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(loading_text_layer));
}

static void loading_window_unload(Window *window) {
	
}

static void bus_window_load(Window *window) {
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	window_set_background_color(window, GColorVividCerulean);
	
	route_name_layer = text_layer_create(GRect(0, 10, bounds.size.w, bounds.size.h));
	text_layer_set_background_color(route_name_layer, GColorClear);
	text_layer_set_text(route_name_layer, "Name");
	
	route_number_layer = text_layer_create(GRect(0, 30, bounds.size.w, bounds.size.h));
	text_layer_set_background_color(route_number_layer, GColorClear);
	text_layer_set_text(route_number_layer, "Number");
	
	arrival_time_layer = text_layer_create(GRect(0, 50, bounds.size.w, bounds.size.h));
	text_layer_set_background_color(arrival_time_layer, GColorClear);
	text_layer_set_text(arrival_time_layer, "00");
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(route_name_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(route_number_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(arrival_time_layer));
}

static void bus_window_unload(Window *window) {
	
}

static void init() {
	main_window = window_create();
	
	window_set_window_handlers(main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	window_set_click_config_provider(main_window, click_config_provider);
	
	loading_window = window_create();
	
	window_set_window_handlers(loading_window, (WindowHandlers) {
		.load = loading_window_load,
		.unload = loading_window_unload
	});
	
	bus_window = window_create(); 
	
	window_set_window_handlers(bus_window, (WindowHandlers) {
		.load = bus_window_load,
		.unload = bus_window_unload
	});
	
	window_stack_push(main_window, true);
	
	init_appmessage();
}

static void deinit() {

}

int main(void) {
	init();
  app_event_loop();
 	deinit();
}