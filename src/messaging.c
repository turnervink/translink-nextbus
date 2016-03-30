#include <pebble.h>
#include "messaging.h"
#include "main.h"

void inbox_received_handler(DictionaryIterator *iter, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Received appmessage");
	
	static char route_name_buffer[100];
	static char route_number_buffer[12];
	static char arrival_time_buffer[12];
	
	Tuple *route_name_tup = dict_find(iter, KEY_ROUTE_NAME);
	Tuple *route_number_tup = dict_find(iter, KEY_ROUTE_NUMBER);
	Tuple *arrival_time_tup = dict_find(iter, KEY_ARRIVAL_TIME);
	
	if (route_name_tup) {
		window_stack_pop(false);
		window_stack_push(bus_window, true);
		
		snprintf(route_name_buffer, sizeof(route_name_buffer), "%s", route_name_tup->value->cstring);
		text_layer_set_text(route_name_layer, route_name_buffer);
	}
	
	if (route_number_tup) {
		snprintf(route_number_buffer, sizeof(route_number_buffer), "%s", route_number_tup->value->cstring);
		text_layer_set_text(route_number_layer, route_number_buffer);
	}
	
	if (arrival_time_tup) {
		snprintf(arrival_time_buffer, sizeof(arrival_time_buffer), "%d", (int)arrival_time_tup->value->int32);
		text_layer_set_text(arrival_time_layer, arrival_time_buffer);
	}
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped, %d", reason);
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void init_appmessage() {
	app_message_register_inbox_received(inbox_received_handler);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	
	// Create buffers based on what we are sending/receiving
	int buffer_in = dict_calc_buffer_size(15, sizeof(char), sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(char), sizeof(int8_t), sizeof(int8_t), sizeof(int8_t), sizeof(int8_t), sizeof(char), sizeof(char), sizeof(char), sizeof(int8_t), sizeof(char), sizeof(char)) + 10;
	int buffer_out = dict_calc_buffer_size(1, sizeof(int32_t));
	APP_LOG(APP_LOG_LEVEL_INFO, "buffer_in is %d", buffer_in);
	APP_LOG(APP_LOG_LEVEL_INFO, "buffer_out is %d", buffer_out);
	app_message_open(buffer_in, buffer_out);
}