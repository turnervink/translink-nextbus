#include <pebble.h>
#include "main.h"
#include "bus_window.h"

void bus_window_load(Window *window) {
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	window_set_background_color(window, GColorVividCerulean);
	
	route_number_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(route_number_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_background_color(route_number_layer, GColorClear);
	text_layer_set_text_alignment(route_number_layer, GTextAlignmentCenter);
	text_layer_set_text_color(route_number_layer, GColorWhite);
	
	route_name_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(route_name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(route_name_layer, GColorClear);
	text_layer_set_overflow_mode(route_name_layer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(route_name_layer, GTextAlignmentCenter);
	text_layer_set_text_color(route_name_layer, GColorWhite);
	
	arrives_in_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
	text_layer_set_font(arrives_in_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
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
	text_layer_set_font(minutes_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_background_color(minutes_text_layer, GColorClear);
	text_layer_set_text_alignment(minutes_text_layer, GTextAlignmentCenter);
	text_layer_set_text_color(minutes_text_layer, GColorWhite);
	text_layer_set_text(minutes_text_layer, "minutes");
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(route_number_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(route_name_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(arrival_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(arrives_in_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(minutes_text_layer));
}

void bus_window_unload(Window *window) {
	
}