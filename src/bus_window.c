#include <pebble.h>
#include "main.h"
#include "bus_window.h"

ScrollLayer *scroll_layer;
ContentIndicator *indicator;
Layer *up_indicator_layer, *down_indicator_layer;

static void up_click(ClickRecognizerRef recognizer, void *context) {

}

static void down_click(ClickRecognizerRef recognizer, void *context) {

}

static void next_click(ClickRecognizerRef recognizer, void *context) {
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

void bus_click_config_provider(void *context) {
	ButtonId next = BUTTON_ID_SELECT;
	ButtonId up = BUTTON_ID_UP;
	ButtonId down = BUTTON_ID_DOWN;

	window_single_click_subscribe(up, up_click);
	window_single_click_subscribe(down, down_click);
	window_single_click_subscribe(next, next_click);
}

void bus_window_load(Window *window) {
	vibes_double_pulse();

	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	window_set_background_color(window, PBL_IF_BW_ELSE(GColorBlack, GColorVividCerulean));

	scroll_layer = scroll_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	scroll_layer_set_shadow_hidden(scroll_layer, true);

	indicator = scroll_layer_get_content_indicator(scroll_layer);
  up_indicator_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y,
                                      bounds.size.w, STATUS_BAR_LAYER_HEIGHT));
  down_indicator_layer = layer_create(GRect(0, bounds.size.h - STATUS_BAR_LAYER_HEIGHT,
                                        bounds.size.w, STATUS_BAR_LAYER_HEIGHT));

	const ContentIndicatorConfig up_config = (ContentIndicatorConfig) {
    .layer = up_indicator_layer,
    .times_out = false,
    .alignment = GAlignCenter,
    .colors = {
      .foreground = GColorWhite,
      .background = PBL_IF_BW_ELSE(GColorBlack, GColorVividCerulean)
    }
  };
  content_indicator_configure_direction(indicator, ContentIndicatorDirectionUp,
                                        &up_config);

  const ContentIndicatorConfig down_config = (ContentIndicatorConfig) {
    .layer = down_indicator_layer,
    .times_out = false,
    .alignment = GAlignCenter,
    .colors = {
      .foreground = GColorWhite,
      .background = PBL_IF_BW_ELSE(GColorBlack, GColorVividCerulean)
    }
  };
  content_indicator_configure_direction(indicator, ContentIndicatorDirectionDown,
                                        &down_config);

	route_number_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(route_number_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_background_color(route_number_layer, GColorClear);
	text_layer_set_text_alignment(route_number_layer, GTextAlignmentCenter);
	text_layer_set_text_color(route_number_layer, GColorWhite);

	route_name_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(route_name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_background_color(route_name_layer, GColorClear);
	text_layer_set_overflow_mode(route_name_layer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(route_name_layer, GTextAlignmentCenter);
	text_layer_set_text_color(route_name_layer, GColorWhite);

	arrives_in_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(arrives_in_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_background_color(arrives_in_layer, GColorClear);
	text_layer_set_text_alignment(arrives_in_layer, GTextAlignmentCenter);
	text_layer_set_text_color(arrives_in_layer, GColorWhite);
	text_layer_set_text(arrives_in_layer, "arrives in");

	arrival_time_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(arrival_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
	text_layer_set_background_color(arrival_time_layer, GColorClear);
	text_layer_set_text_alignment(arrival_time_layer, GTextAlignmentCenter);
	text_layer_set_text_color(arrival_time_layer, GColorWhite);

	minutes_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(minutes_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_background_color(minutes_text_layer, GColorClear);
	text_layer_set_text_alignment(minutes_text_layer, GTextAlignmentCenter);
	text_layer_set_text_color(minutes_text_layer, GColorWhite);
	text_layer_set_text(minutes_text_layer, "minutes");

	// layer_add_child(window_get_root_layer(window), text_layer_get_layer(route_number_layer));
	// layer_add_child(window_get_root_layer(window), text_layer_get_layer(route_name_layer));
	// layer_add_child(window_get_root_layer(window), text_layer_get_layer(arrival_time_layer));
	// layer_add_child(window_get_root_layer(window), text_layer_get_layer(arrives_in_layer));
	// layer_add_child(window_get_root_layer(window), text_layer_get_layer(minutes_text_layer));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(route_number_layer));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(route_name_layer));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(arrival_time_layer));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(arrives_in_layer));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(minutes_text_layer));
	layer_add_child(window_get_root_layer(window), scroll_layer_get_layer(scroll_layer));
	layer_add_child(window_get_root_layer(window), up_indicator_layer);
  layer_add_child(window_get_root_layer(window), down_indicator_layer);
	scroll_layer_set_content_size(scroll_layer, GSize(bounds.size.w, bounds.size.h * 2));
	scroll_layer_set_click_config_onto_window(scroll_layer, window);
}

void bus_window_unload(Window *window) {

}
