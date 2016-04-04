#include <pebble.h>
#include "main.h"
#include "error_window.h"

TextLayer *oops_layer, *message_layer;

char* error_messages[10] = { 
	"Invalid API key, please email the developer!", // Invalid API key
	"Couldn't connect to TransLink", // Database connection error
	"That doesn't seem to be a stop number", // Invalid stop number
	"Couldn't seem to find that stop", // Stop not found
	"Unknown error, please email the developer!", // Unknown get estimates error
	"Invalid route", // UInvalid route
	"No arrivals found for this stop. \n \n Stuck? \n Call #TAXI for a cab", // No stop estimates found
	"Invalid time frame", // Invalid time frame
	"Invalid count", // Invalid count
	"Timed out \n Check your Bluetooth/Internet connection!"
};

void size_error_message() {
	GRect bounds = layer_get_bounds(window_get_root_layer(error_window));
	
	GSize message_size = text_layer_get_content_size(message_layer);
	GSize oops_size = text_layer_get_content_size(oops_layer);
	
	layer_set_frame(text_layer_get_layer(message_layer), GRect(0, bounds.size.h / 2 - (message_size.h / 2), bounds.size.w, message_size.h + 3));
	
	GRect message_grect = layer_get_frame(text_layer_get_layer(message_layer));
	
	layer_set_frame(text_layer_get_layer(oops_layer), GRect(0, message_grect.origin.y - oops_size.h, bounds.size.w, oops_size.h + 3));
}

void error_window_load(Window *window) {
	window_set_background_color(window, GColorRed);
	
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	
	oops_layer = text_layer_create(GRect(0, 3, bounds.size.w, bounds.size.h));
	text_layer_set_font(oops_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(oops_layer, GTextAlignmentCenter);
	text_layer_set_background_color(oops_layer, GColorClear);
	text_layer_set_text_color(oops_layer, GColorWhite);
	text_layer_set_text(oops_layer, "Uh oh...");
	
	message_layer = text_layer_create(GRect(0, bounds.size.h / 2, bounds.size.w, bounds.size.h));
	text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
	text_layer_set_text_alignment(message_layer, GTextAlignmentCenter);
	text_layer_set_background_color(message_layer, GColorClear);
	text_layer_set_text_color(message_layer, GColorWhite);
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(oops_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(message_layer));
}

void error_window_unload(Window *window) {
	
}