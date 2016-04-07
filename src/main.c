#include <pebble.h>
#include "messaging.h"
#include "main.h"
#include "bus_window.h"
#include "error_window.h"

Window *main_window, *loading_window, *bus_window, *error_window;
static TextLayer *slot_one, *slot_two, *slot_three, *slot_four, *slot_five, *loading_text_layer, *instr_text_layer;
TextLayer *route_name_layer, *route_number_layer, *arrival_time_layer, *arrives_in_layer, *minutes_text_layer;
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

void timeout_callback() {
	window_stack_pop(false);
	window_stack_push(error_window, true); 
	
	text_layer_set_text(message_layer, error_messages[9]);
	
	size_error_message();
}

static void increment_slot(bool inc_up) {
	int slot_value = SLOTS[current_slot];
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "Slot %d is %d", current_slot, slot_value);
	#endif
	
	if (slot_value == 9 && inc_up == true) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Setting slot to 0");
		#endif
		slot_value = 0;
	}	else if (slot_value == 0 && inc_up == false) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Setting slot to 9");
		#endif
		slot_value = 9;
	} else if (inc_up == true) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Moving slot up");
		#endif
		slot_value = slot_value + 1;
	} else if (inc_up == false) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Moving slot down");
		#endif
		slot_value = slot_value - 1;
	}
	
	SLOTS[current_slot] = slot_value;
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "slot_value is %d", slot_value);
	APP_LOG(APP_LOG_LEVEL_INFO, "Stored slot is %d", SLOTS[current_slot]);
	#endif
	
	
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
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "Up button clicked!");
	#endif
	increment_slot(true);
}

static void down_click(ClickRecognizerRef recognizer, void *context) {
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "Down button clicked!");
	#endif
	increment_slot(false);
}

static void update_indicator() {
	text_layer_set_background_color(slot_one, GColorWhite);
	text_layer_set_background_color(slot_two, GColorWhite);
	text_layer_set_background_color(slot_three, GColorWhite);
	text_layer_set_background_color(slot_four, GColorWhite);
	text_layer_set_background_color(slot_five, GColorWhite);
	
	if (current_slot == 0) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 0");
		#endif
		text_layer_set_background_color(slot_one, GColorVividCerulean);
	} else if (current_slot == 1) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 1");
		#endif
		text_layer_set_background_color(slot_two, GColorVividCerulean);
	} else if (current_slot == 2) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 2");
		#endif
		text_layer_set_background_color(slot_three, GColorVividCerulean);
	} else if (current_slot == 3) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 3");
		#endif
		text_layer_set_background_color(slot_four, GColorVividCerulean);
	} else if (current_slot == 4) {
		#ifdef SHOW_LOGS
		APP_LOG(APP_LOG_LEVEL_INFO, "Indicating slot 4");
		#endif
		text_layer_set_background_color(slot_five, GColorVividCerulean);
	}
}

static void back_click(ClickRecognizerRef recognizer, void *context) {
	if (current_slot == 0) {
		window_stack_pop(false);
	} else {
		current_slot = current_slot - 1;
	}
	
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "Current slot is %d", current_slot);
	#endif
	update_indicator();
}

static void next_click(ClickRecognizerRef recognizer, void *context) {
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "Select button clicked!");
	#endif
	if (current_slot == 4) {
		stop_number = 10000 * SLOTS[0] + 1000 * SLOTS[1] + 100 * SLOTS[2] + 10 * SLOTS[3] + SLOTS[4];
		
		APP_LOG(APP_LOG_LEVEL_INFO, "Stop number: %d", stop_number);
		
		if (stop_number < 10000) {
			window_stack_pop(false);
			window_stack_push(error_window, true);
			
			text_layer_set_text(message_layer, error_messages[2]);
			size_error_message();
		} else {
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
	} else {
		current_slot = current_slot + 1;
	}
	#ifdef SHOW_LOGS
	APP_LOG(APP_LOG_LEVEL_INFO, "Current slot is %d", current_slot);
	#endif
	update_indicator();
}


static void click_config_provider(void *context) {
	ButtonId next = BUTTON_ID_SELECT;
	ButtonId back = BUTTON_ID_BACK;
	ButtonId up = BUTTON_ID_UP;
	ButtonId down = BUTTON_ID_DOWN;
	
	window_single_click_subscribe(up, up_click);
	window_single_click_subscribe(down, down_click);
	window_single_click_subscribe(next, next_click);
	window_single_click_subscribe(back, back_click);
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

static void main_window_load(Window *window) {
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	
	slot_one = text_layer_create(GRect(0, 0, 144, 168));
	text_layer_set_font(slot_one, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	text_layer_set_text_alignment(slot_one, GTextAlignmentCenter);
	
	slot_two = text_layer_create(GRect(10, 0, 144, 168));
	text_layer_set_font(slot_two, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	text_layer_set_text_alignment(slot_two, GTextAlignmentCenter);
	
	slot_three = text_layer_create(GRect(20, 0, 144, 168));
	text_layer_set_font(slot_three, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	text_layer_set_text_alignment(slot_three, GTextAlignmentCenter);
	
	slot_four = text_layer_create(GRect(30, 0, 144, 168));
	text_layer_set_font(slot_four, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	text_layer_set_text_alignment(slot_four, GTextAlignmentCenter);
	
	slot_five = text_layer_create(GRect(40, 0, 144, 168));
	text_layer_set_font(slot_five, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
	text_layer_set_text_alignment(slot_five, GTextAlignmentCenter);
	
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
	int centre = bounds.size.w / 2 - (slot_size.w / 2);

	layer_set_frame(text_layer_get_layer(slot_one), GRect(centre - (slot_size.w * 2), bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h + 9));
	layer_set_frame(text_layer_get_layer(slot_two), GRect(centre - slot_size.w, bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h + 9));
	layer_set_frame(text_layer_get_layer(slot_three), GRect(centre, bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h + 9));
	layer_set_frame(text_layer_get_layer(slot_four), GRect(centre + slot_size.w, bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h + 9));
	layer_set_frame(text_layer_get_layer(slot_five), GRect(centre + (slot_size.w * 2), bounds.size.h / 2 - (slot_size.h / 2), slot_size.w, slot_size.h + 9));
	
	update_indicator();
	
	
	instr_text_layer = text_layer_create(GRect(0, 40, bounds.size.w, 30));
	text_layer_set_font(instr_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text_alignment(instr_text_layer, GTextAlignmentCenter);
	text_layer_set_background_color(instr_text_layer, GColorClear);
	text_layer_set_text(instr_text_layer, "Enter a stop number");
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(instr_text_layer));
}

static void main_window_unload(Window *window) {
	
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