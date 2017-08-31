#pragma once
#include <pebble.h>

#define KEY_STOP_NUMBER 0
#define KEY_ROUTE_NAME 1
#define KEY_ROUTE_NUMBER 2
#define KEY_ARRIVAL_TIME 3
#define KEY_ERROR_CODE 4
#define KEY_TEST 5

typedef enum {
  MsgKeyStopNumber,
  MsgKeyErrorCode,
  MsgKeyRoute0,
  MsgKeyName0,
  MsgKeyCountdown0,
  MsgKeyDummy,

  MsgKeyCount
} CfgKey;

int stop_number;
char* bus0[3];
char* bus1[3];
char* bus2[3];

void init_appmessage();

void inbox_received_handler(DictionaryIterator *iter, void *context);
void inbox_dropped_callback(AppMessageResult reason, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
void outbox_sent_callback(DictionaryIterator *iterator, void *context);
