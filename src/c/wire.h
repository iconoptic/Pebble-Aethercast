#pragma once

#include <pebble.h>

// PebbleKit JS Numbers always arrive as a 4-byte int32 tuple regardless of
// the wire table's "logical" width (int16/uint8/etc) - see
// docs/vendor/pebble-comm-sending-receiving.md. Narrow explicitly on read.
static inline int32_t wire_read_int32(DictionaryIterator *iter, uint32_t key, int32_t fallback) {
  Tuple *t = dict_find(iter, key);
  return t ? t->value->int32 : fallback;
}

// Copies a fixed-length byte tuple into `dst`; returns false (leaving `dst`
// untouched) if the key is missing or its length doesn't match exactly.
static inline bool wire_read_bytes(DictionaryIterator *iter, uint32_t key, void *dst, size_t len) {
  Tuple *t = dict_find(iter, key);
  if (!t || t->length != len) {
    return false;
  }
  memcpy(dst, t->value->data, len);
  return true;
}
