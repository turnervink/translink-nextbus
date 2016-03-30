#pragma once
#include <pebble.h>

#define KEY_STOP_NUMBER 0

int stop_number;

void init_appmessage();

void inbox_received_handler(DictionaryIterator *iter, void *context);
void inbox_dropped_callback(AppMessageResult reason, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
void outbox_sent_callback(DictionaryIterator *iterator, void *context);