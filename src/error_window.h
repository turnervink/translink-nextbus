#pragma once
#include <pebble.h>

TextLayer *oops_layer, *message_layer;

char* error_messages[10];

void error_window_load(Window *window);
void error_window_unload(Window *window);
void size_error_message();