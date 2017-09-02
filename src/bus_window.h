#pragma once
#include <pebble.h>

ScrollLayer *scroll_layer;

void bus_window_load(Window *window);
void bus_window_unload(Window *window);
void bus_click_config_provider(void *context);
