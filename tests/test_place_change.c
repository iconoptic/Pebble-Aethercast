// Host tests for the place-change mask and the 3 h delta fallback.
// Run via `make -C tests && ./tests/run`.
//
// Byte table shared with tests/trail.test.js (buildMask). Little-endian,
// bit i in byte i>>3. A change at that index and nowhere else:
//   1  -> [2, 0, 0, 0, 0]
//   7  -> [128, 0, 0, 0, 0]
//   8  -> [0, 1, 0, 0, 0]
//   14 -> [0, 64, 0, 0, 0]
//   35 -> [0, 0, 0, 0, 8]
// tools/fake_payload.js preset "trail" sends the index-14 row.

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/c/lib/scale.h"

static void prv_set_only(uint8_t mask[5], int idx) {
  memset(mask, 0, 5);
  mask[idx >> 3] = (uint8_t)(1u << (idx & 7));
}

static void test_bits_round_trip(void) {
  static const int kIdx[] = {0, 1, 7, 8, 14, 35};
  for (int n = 0; n < (int)(sizeof(kIdx) / sizeof(kIdx[0])); n++) {
    uint8_t mask[5];
    prv_set_only(mask, kIdx[n]);
    for (int i = 0; i < SCALE_N_SAMPLES; i++) {
      bool on = scale_place_change_bit(mask, i);
      assert(on == (i == kIdx[n]));
    }
  }
}

static void test_bit_edges(void) {
  uint8_t mask[5] = {0xff, 0xff, 0xff, 0xff, 0xff};
  assert(!scale_place_change_bit(NULL, 14));
  assert(!scale_place_change_bit(mask, -1));
  assert(!scale_place_change_bit(mask, SCALE_N_SAMPLES));
  assert(!scale_place_change_bit(mask, 200));
}

static void test_trail_preset_mask(void) {
  // Exact bytes from tools/fake_payload.js preset "trail".
  const uint8_t mask[5] = {0, 64, 0, 0, 0};
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    assert(scale_place_change_bit(mask, i) == (i == 14));
  }
}

static void test_effective_delta3(void) {
  int16_t series[SCALE_N_SAMPLES];
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    series[i] = (int16_t)(1000 + i);
  }

  assert(scale_effective_delta3(series, 24, true, 0) == 0);
  assert(scale_trend_delta3(series, 24) != 0);
  assert(scale_effective_delta3(series, 24, true, -13) == -13);

  assert(scale_effective_delta3(series, 24, false, 99) == scale_trend_delta3(series, 24));

  // 200 must be clamped to the last sample before any read. Forgetting the
  // clamp makes scale_trend_delta3 return 0 instead of series[35]-series[32].
  int16_t clamped = (int16_t)(series[SCALE_N_SAMPLES - 1] - series[SCALE_N_SAMPLES - 1 - 3]);
  assert(clamped != 0);
  assert(scale_effective_delta3(series, 200, false, 99) == clamped);
  assert(scale_effective_delta3(series, 200, false, 99) ==
         scale_trend_delta3(series, (uint8_t)(SCALE_N_SAMPLES - 1)));
}

void test_place_change_run(void) {
  test_bits_round_trip();
  test_bit_edges();
  test_trail_preset_mask();
  test_effective_delta3();
  printf("place-change host tests passed\n");
}
