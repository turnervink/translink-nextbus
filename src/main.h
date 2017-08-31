#pragma once
#include <pebble.h>

//#define SHOW_LOGS

Window *bus_window, *error_window;
TextLayer *route_name_layer, *route_number_layer, *arrival_time_layer, *arrives_in_layer, *minutes_text_layer;
TextLayer *bus0route, *bus0name, *bus0countdown;
TextLayer *bus1route, *bus1name, *bus1countdown;
TextLayer *bus2route, *bus2name, *bus2countdown;
TextLayer *min_txt_layer;
AppTimer *comm_timer;

int stop_number;

void size_layers();
void timeout_callback();
