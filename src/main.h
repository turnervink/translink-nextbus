#pragma once
#include <pebble.h>

//#define SHOW_LOGS

Window *bus_window, *error_window;
TextLayer *route_name_layer, *route_number_layer, *arrival_time_layer, *arrives_in_layer, *minutes_text_layer;
AppTimer *comm_timer;

int stop_number;

void size_layers();
void timeout_callback();