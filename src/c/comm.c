#include "comm.h"
#include "model.h"

// Sized per docs/design/01-data-protocol.md: ~280 bytes of payload values
// (core + forecast fields) plus per-tuple dictionary overhead inbound, a
// single REQUEST byte outbound. Comfortably under 1024 with ~27 keys.
#define INBOX_SIZE 1024
#define OUTBOX_SIZE 128

#define REQUEST_RETRY_MS 3000
#define MAX_REQUEST_RETRIES 3

static int s_retries_left;

static void prv_send_request(void *data) {
  DictionaryIterator *iter;
  if (app_message_outbox_begin(&iter) != APP_MSG_OK) {
    return;
  }
  dict_write_uint8(iter, MESSAGE_KEY_REQUEST, 1);
  app_message_outbox_send();
}

static void prv_outbox_failed(DictionaryIterator *iter, AppMessageResult reason, void *context) {
  if (s_retries_left > 0) {
    s_retries_left--;
    app_timer_register(REQUEST_RETRY_MS, prv_send_request, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "comm: outbox failed, giving up (reason %d)", reason);
  }
}

static void prv_outbox_sent(DictionaryIterator *iter, void *context) {
  s_retries_left = 0;
}

static void prv_inbox_received(DictionaryIterator *iter, void *context) {
  if (dict_find(iter, MESSAGE_KEY_PKJS_READY)) {
    comm_request_refresh();
    return;
  }
  model_apply_inbox(iter);
}

static void prv_inbox_dropped(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "comm: inbox dropped (reason %d)", reason);
}

void comm_init(void) {
  app_message_register_inbox_received(prv_inbox_received);
  app_message_register_inbox_dropped(prv_inbox_dropped);
  app_message_register_outbox_failed(prv_outbox_failed);
  app_message_register_outbox_sent(prv_outbox_sent);
  app_message_open(INBOX_SIZE, OUTBOX_SIZE);
}

void comm_request_refresh(void) {
  s_retries_left = MAX_REQUEST_RETRIES;
  prv_send_request(NULL);
  model_note_request_sent();
}
