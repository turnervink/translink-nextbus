#include <pebble.h>
#include "main.h"
#include "messaging.h"
#include "error_window.h"

void inbox_received_handler(DictionaryIterator *iter, void *context) {
	bool error_occured = 0;
	APP_LOG(APP_LOG_LEVEL_INFO, "Received messagekeys");
	APP_LOG(APP_LOG_LEVEL_INFO, "Cancelling comm_timer");
	app_timer_cancel(comm_timer);

	Tuple *t = dict_read_first(iter);
	while(t) {
		int key = t->key - 10000;
		APP_LOG(APP_LOG_LEVEL_INFO, t->value->cstring);

		if (key == MsgKeyErrorCode) {
			APP_LOG(APP_LOG_LEVEL_INFO, "An error occured");
				error_occured = 1;

				window_stack_pop(false);
				window_stack_push(error_window, true);

				if (strcmp(t->value->cstring, "10001") == 0) {
					text_layer_set_text(message_layer, error_messages[0]);
				} else if (strcmp(t->value->cstring, "10002") == 0) {
					text_layer_set_text(message_layer, error_messages[1]);
				} else if (strcmp(t->value->cstring, "3001") == 0) {
					text_layer_set_text(message_layer, error_messages[2]);
				} else if (strcmp(t->value->cstring, "3002") == 0) {
					text_layer_set_text(message_layer, error_messages[3]);
				} else if (strcmp(t->value->cstring, "3003") == 0) {
					text_layer_set_text(message_layer, error_messages[4]);
				} else if (strcmp(t->value->cstring, "3004") == 0) {
					text_layer_set_text(message_layer, error_messages[5]);
				} else if (strcmp(t->value->cstring, "3005") == 0) {
					text_layer_set_text(message_layer, error_messages[6]);
				} else if (strcmp(t->value->cstring, "3006") == 0) {
					text_layer_set_text(message_layer, error_messages[7]);
				} else if (strcmp(t->value->cstring, "3007") == 0) {
					text_layer_set_text(message_layer, error_messages[8]);
				}

				size_error_message();
		}

		// TODO skip the array thing
		// Bus 0 info
		if (key == MsgKeyRoute0) {
			window_stack_pop(false);
			window_stack_push(bus_window, true);

			// bus0[0] = t->value->cstring;
			// text_layer_set_text(bus0route, t->value->cstring);
			static char route0buffer[16];
    	snprintf(route0buffer, sizeof(route0buffer), "%s", t->value->cstring);
			text_layer_set_text(bus0route, route0buffer);
		}

		if (key == MsgKeyName0) {
			// bus0[1] = t->value->cstring;
			// text_layer_set_text(bus0name, t->value->cstring);
			static char name0buffer[16];
    	snprintf(name0buffer, sizeof(name0buffer), "%s", t->value->cstring);
			text_layer_set_text(bus0name, name0buffer);
		}

		if (key == MsgKeyCountdown0) {
			// bus0[2] = t->value->cstring;
			// text_layer_set_text(bus0countdown, t->value->cstring);
			static char countdown0buffer[16];
    	snprintf(countdown0buffer, sizeof(countdown0buffer), "%s", t->value->cstring);
			text_layer_set_text(bus0countdown, countdown0buffer);
		}

		// Bus 1 info
		if (key == MsgKeyRoute1) {
			// bus1[0] = t->value->cstring;
			// text_layer_set_text(bus1route, t->value->cstring);
			static char route1buffer[16];
    	snprintf(route1buffer, sizeof(route1buffer), "%s", t->value->cstring);
			text_layer_set_text(bus1route, route1buffer);
		}

		if (key == MsgKeyName1) {
			// bus1[1] = t->value->cstring;
			// text_layer_set_text(bus1name, t->value->cstring);
			static char name1buffer[128];
    	snprintf(name1buffer, sizeof(name1buffer), "%s", t->value->cstring);
			text_layer_set_text(bus1name, name1buffer);
		}

		if (key == MsgKeyCountdown1) {
			// bus1[2] = t->value->cstring;
			// text_layer_set_text(bus1countdown, strcat(t->value->cstring, " minutes"));
			static char countdown1buffer[16];
    	snprintf(countdown1buffer, sizeof(countdown1buffer), "%s", t->value->cstring);
			text_layer_set_text(bus1countdown, strcat(countdown1buffer, " minutes"));
		}

		// Bus 2 info
		if (key == MsgKeyRoute2) {
			APP_LOG(APP_LOG_LEVEL_INFO, "Received route2");
			// bus2[0] = t->value->cstring;
			// text_layer_set_text(bus2route, bus2[0]);
			static char route2buffer[16];
    	snprintf(route2buffer, sizeof(route2buffer), "%s", t->value->cstring);
			text_layer_set_text(bus2route, route2buffer);
		}

		if (key == MsgKeyName2) {
			APP_LOG(APP_LOG_LEVEL_INFO, "Received name2");
			// bus2[1] = t->value->cstring;
			// text_layer_set_text(bus2name, bus2[1]);
			static char name2buffer[128];
    	snprintf(name2buffer, sizeof(name2buffer), "%s", t->value->cstring);
			text_layer_set_text(bus2name, name2buffer);
		}

		if (key == MsgKeyCountdown2) {
			APP_LOG(APP_LOG_LEVEL_INFO, "Received cd2");
			// bus0[2] = t->value->cstring;
			// text_layer_set_text(bus2countdown, strcat(bus2[2], " minutes"));
			static char countdown2buffer[16];
    	snprintf(countdown2buffer, sizeof(countdown2buffer), "%s", t->value->cstring);
			text_layer_set_text(bus2countdown, strcat(countdown2buffer, " minutes"));
		}

		t = dict_read_next(iter);
	}

	if (error_occured == 1) {
		APP_LOG(APP_LOG_LEVEL_INFO, "An error occured");
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "No error occured");
		size_layers();
	}

	// static char route_name_buffer[100];
	// static char route_number_buffer[12];
	// static char arrival_time_buffer[12];
	//
	// Tuple *route_name_tup = dict_find(iter, KEY_ROUTE_NAME);
	// Tuple *route_number_tup = dict_find(iter, KEY_ROUTE_NUMBER);
	// Tuple *arrival_time_tup = dict_find(iter, KEY_ARRIVAL_TIME);
	// Tuple *error_code_tup = dict_find(iter, KEY_ERROR_CODE);
	//
	// if (error_code_tup) {
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "An error occured");
	// 	error_occured = 1;
	//
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "Cancelling comm_timer");
	// 	app_timer_cancel(comm_timer);
	//
	// 	window_stack_pop(false);
	// 	window_stack_push(error_window, true);
	//
	// 	if (strcmp(error_code_tup->value->cstring, "10001") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[0]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "10002") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[1]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3001") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[2]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3002") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[3]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3003") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[4]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3004") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[5]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3005") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[6]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3006") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[7]);
	// 	} else if (strcmp(error_code_tup->value->cstring, "3007") == 0) {
	// 		text_layer_set_text(message_layer, error_messages[8]);
	// 	}
	//
	// 	size_error_message();
	// }
	//
	// if (route_name_tup) {
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "Route name received");
	//
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "Cancelling comm_timer");
	// 	app_timer_cancel(comm_timer);
	//
	// 	window_stack_pop(false);
	// 	window_stack_push(bus_window, true);
	//
	// 	snprintf(route_name_buffer, sizeof(route_name_buffer), "%s", route_name_tup->value->cstring);
	// 	text_layer_set_text(route_name_layer, route_name_buffer);
	// }
	//
	// if (route_number_tup) {
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "Route number received");
	// 	snprintf(route_number_buffer, sizeof(route_number_buffer), "%s", route_number_tup->value->cstring);
	// 	text_layer_set_text(route_number_layer, route_number_buffer);
	// }
	//
	// if (arrival_time_tup) {
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "Arrival time received");
	// 	snprintf(arrival_time_buffer, sizeof(arrival_time_buffer), "%d", (int)arrival_time_tup->value->int32);
	// 	text_layer_set_text(arrival_time_layer, arrival_time_buffer);
	// 	//text_layer_set_text(arrival_time_layer, "15");
	// }
	//
	// if (error_occured == 1) {
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "An error occured");
	// } else {
	// 	APP_LOG(APP_LOG_LEVEL_INFO, "No error occured");
	// 	size_layers();
	// }

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
