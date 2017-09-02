#include <pebble.h>
#include "main.h"
#include "bus_window.h"

ScrollLayer *scroll_layer;
ContentIndicator *indicator;
Layer *up_indicator_layer, *down_indicator_layer;


static void up_click(ClickRecognizerRef recognizer, void *context) {}

static void down_click(ClickRecognizerRef recognizer, void *context) {}

static void next_click(ClickRecognizerRef recognizer, void *context) {
	// Begin dictionary
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	// Add a key-value pair
	dict_write_uint32(iter, MESSAGE_KEY_MsgKeyStopNumber, stop_number);

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

	layer_add_child(window_get_root_layer(window), scroll_layer_get_layer(scroll_layer));
	scroll_layer_set_click_config_onto_window(scroll_layer, window);
	layer_add_child(window_get_root_layer(window), up_indicator_layer);
  layer_add_child(window_get_root_layer(window), down_indicator_layer);

	// route_number_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	// text_layer_set_font(route_number_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	// text_layer_set_background_color(route_number_layer, GColorClear);
	// text_layer_set_text_alignment(route_number_layer, GTextAlignmentCenter);
	// text_layer_set_text_color(route_number_layer, GColorWhite);
	//
	// route_name_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	// text_layer_set_font(route_name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	// text_layer_set_background_color(route_name_layer, GColorClear);
	// text_layer_set_overflow_mode(route_name_layer, GTextOverflowModeWordWrap);
	// text_layer_set_text_alignment(route_name_layer, GTextAlignmentCenter);
	// text_layer_set_text_color(route_name_layer, GColorWhite);
	//
	// arrives_in_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	// text_layer_set_font(arrives_in_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	// text_layer_set_background_color(arrives_in_layer, GColorClear);
	// text_layer_set_text_alignment(arrives_in_layer, GTextAlignmentCenter);
	// text_layer_set_text_color(arrives_in_layer, GColorWhite);
	// text_layer_set_text(arrives_in_layer, "arrives in");
	//
	// arrival_time_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	// text_layer_set_font(arrival_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
	// text_layer_set_background_color(arrival_time_layer, GColorClear);
	// text_layer_set_text_alignment(arrival_time_layer, GTextAlignmentCenter);
	// text_layer_set_text_color(arrival_time_layer, GColorWhite);
	//
	// minutes_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	// text_layer_set_font(minutes_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	// text_layer_set_background_color(minutes_text_layer, GColorClear);
	// text_layer_set_text_alignment(minutes_text_layer, GTextAlignmentCenter);
	// text_layer_set_text_color(minutes_text_layer, GColorWhite);
	// text_layer_set_text(minutes_text_layer, "minutes");
	//
	// scroll_layer_add_child(scroll_layer, text_layer_get_layer(route_number_layer));
	// scroll_layer_add_child(scroll_layer, text_layer_get_layer(route_name_layer));
	// scroll_layer_add_child(scroll_layer, text_layer_get_layer(arrival_time_layer));
	// scroll_layer_add_child(scroll_layer, text_layer_get_layer(arrives_in_layer));
	// scroll_layer_add_child(scroll_layer, text_layer_get_layer(minutes_text_layer));
	// layer_add_child(window_get_root_layer(window), scroll_layer_get_layer(scroll_layer));
	// layer_add_child(window_get_root_layer(window), up_indicator_layer);
  // layer_add_child(window_get_root_layer(window), down_indicator_layer);
	// scroll_layer_set_content_size(scroll_layer, GSize(bounds.size.w, bounds.size.h * 2));
	// scroll_layer_set_click_config_onto_window(scroll_layer, window);

	// Bus 0
	bus0route = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus0route, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_background_color(bus0route, GColorClear);
	text_layer_set_text_alignment(bus0route, GTextAlignmentCenter);
	text_layer_set_text_color(bus0route, GColorWhite);
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus0route));

	bus0name = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus0name, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_background_color(bus0name, GColorClear);
	text_layer_set_text_alignment(bus0name, GTextAlignmentCenter);
	text_layer_set_text_color(bus0name, GColorWhite);
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus0name));

	bus0countdown = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus0countdown, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
	text_layer_set_background_color(bus0countdown, GColorClear);
	text_layer_set_text_alignment(bus0countdown, GTextAlignmentCenter);
	text_layer_set_text_color(bus0countdown, GColorWhite);
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus0countdown));

	min_txt_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(min_txt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_background_color(min_txt_layer, GColorClear);
	text_layer_set_text_alignment(min_txt_layer, GTextAlignmentCenter);
	text_layer_set_text_color(min_txt_layer, GColorWhite);
	text_layer_set_text(min_txt_layer, "minutes");
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(min_txt_layer));

	// Bus 1
	bus1route = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus1route, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(bus1route, GColorClear);
	text_layer_set_text_color(bus1route, GColorWhite);
	text_layer_set_text_alignment(bus1route, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus1route));

	bus1name = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus1name, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_background_color(bus1name, GColorClear);
	text_layer_set_text_color(bus1name, GColorWhite);
	text_layer_set_text_alignment(bus1name, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus1name));

	bus1countdown = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus1countdown, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(bus1countdown, GColorClear);
	text_layer_set_text_color(bus1countdown, GColorWhite);
	text_layer_set_text_alignment(bus1countdown, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus1countdown));

	// Bus 2
	bus2route = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus2route, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(bus2route, GColorClear);
	text_layer_set_text_color(bus2route, GColorWhite);
	text_layer_set_text_alignment(bus2route, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus2route));

	bus2name = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus2name, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_background_color(bus2name, GColorClear);
	text_layer_set_text_color(bus2name, GColorWhite);
	text_layer_set_text_alignment(bus2name, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus2name));

	bus2countdown = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(bus2countdown, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(bus2countdown, GColorClear);
	text_layer_set_text_color(bus2countdown, GColorWhite);
	text_layer_set_text_alignment(bus2countdown, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(bus2countdown));


}

void bus_window_unload(Window *window) {

}
