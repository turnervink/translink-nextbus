#include <pebble.h>

void inbox_received_handler(DictionaryIterator *iter, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Received appmessage");
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