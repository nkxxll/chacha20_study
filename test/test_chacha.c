#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#define MUNIT_ENABLE_ASSERT_ALIASES
#include "test_chacha.h"
#include "chacha20.h"
#include "munit.h"
#include <stdint.h>

void *test_one_round_setup(const MunitParameter params[], void *user_data) {
  uint32_t *expected_state = malloc(sizeof(uint32_t[16]));
  char *char_ex_state[16] = {"879531e0", "c5ecf37d", "bdb886dc", "c9a62f8a",
                             "44c20ef3", "3390af7f", "d9fc690b", "cfacafd2",
                             "e46bea80", "b00a5631", "974c541a", "359e9963",
                             "5c971061", "ccc07c79", "2098d9d6", "91dbd320"};
  for (int i = 0; i < 16; i++) {
    expected_state[i] = strtoul(char_ex_state[i], NULL, 16);
  }

  return (void *)expected_state;
}

void test_tear_down(void *fixture) { free(fixture); }

MunitResult test_one_round(const MunitParameter params[], void *fixture) {
  uint32_t state[16];

  get_state(state);
  quad_round(state, 2, 7, 8, 13);
  assert_memory_equal(16, (void *)state, fixture);
  return MUNIT_OK;
}

void *test_full_round_setup(const MunitParameter params[], void *user_data) {
  uint32_t *expected_state = malloc(sizeof(uint32_t[16]));
  char *char_ex_state[16] = {"837778ab", "e238d763", "a67ae21e", "5950bb2f",
                             "c4f2d0c7", "fc62bb2f", "8fa018fc", "3f5ec7b7",
                             "335271c2", "f29489f3", "eabda8fc", "82e46ebd",
                             "d19c12b4", "b04e16de", "9e83d0cb", "4e3c50a2"};
  for (int i = 0; i < 16; i++) {
    expected_state[i] = strtoul(char_ex_state[i], NULL, 16);
  }

  return (void *)expected_state;
}

MunitResult test_full_round(const MunitParameter params[], void *fixture) {
  uint32_t state[16];
  char *init_state_char[16] = {"61707865", "3320646e", "79622d32", "6b206574",
                               "03020100", "07060504", "0b0a0908", "0f0e0d0c",
                               "13121110", "17161514", "1b1a1918", "1f1e1d1c",
                               "00000001", "09000000", "4a000000", "00000000"};
  for (int i = 0; i < 16; i++) {
    state[i] = strtoul(init_state_char[i], NULL, 16);
  }

  for (int i = 0; i < 10; i++) {
    full_round(state);
  }

  assert_memory_equal(16, (void *)state, fixture);
  return MUNIT_OK;
}

void *test_add_state_setup(const MunitParameter params[], void *user_data) {
  uint32_t *expected_state = malloc(sizeof(uint32_t[16]));
  char *char_ex_state[16] = {"e4e7f110", "15593bd1", "1fdd0f50", "c47120a3",
                             "c7f4d1c7", "0368c033", "9aaa2204", "4e6cd4c3",
                             "466482d2", "09aa9f07", "05d7c214", "a2028bd9",
                             "d19c12b5", "b94e16de", "e883d0cb", "4e3c50a2"};

  for (int i = 0; i < 16; i++) {
    expected_state[i] = strtoul(char_ex_state[i], NULL, 16);
  }

  return (void *)expected_state;
}

MunitResult test_add_state(const MunitParameter params[], void *fixture) {

  uint32_t state[16];
  uint32_t init_state[16];
  char *init_state_char[16] = {"61707865", "3320646e", "79622d32", "6b206574",
                               "03020100", "07060504", "0b0a0908", "0f0e0d0c",
                               "13121110", "17161514", "1b1a1918", "1f1e1d1c",
                               "00000001", "09000000", "4a000000", "00000000"};
  for (int i = 0; i < 16; i++) {
    state[i] = strtoul(init_state_char[i], NULL, 16);
  }

  memcpy(init_state, state, sizeof(uint32_t) * 16);
  for (int i = 0; i < 10; i++) {

    full_round(state);
  }

  add_state(16, state, init_state);

  assert_memory_equal(16, (void *)state, fixture);
  return MUNIT_OK;
}

void *test_serialize_setup(const MunitParameter params[], void *user_data) {
  uint32_t *expected_state = malloc(sizeof(uint32_t[16]));

  char *char_ex_state[16] = {"10f1e7e4", "d13b5915", "500fdd1f", "a32071c4",
                             "c7d1f4c7", "33c06803", "0422aa9a", "c3d46c4e",
                             "d2826446", "079faa09", "14c2d705", "d98b02a2",
                             "b5129cd1", "de164eb9", "cbd083e8", "a2503c4e"};

  for (int i = 0; i < 16; i++) {
    expected_state[i] = strtoul(char_ex_state[i], NULL, 16);
  }

  return (void *)expected_state;
}

MunitResult test_serialize(const MunitParameter params[], void *fixture) {
  uint32_t state[16];
  uint32_t init_state[16];
  char *init_state_char[16] = {"61707865", "3320646e", "79622d32", "6b206574",
                               "03020100", "07060504", "0b0a0908", "0f0e0d0c",
                               "13121110", "17161514", "1b1a1918", "1f1e1d1c",
                               "00000001", "09000000", "4a000000", "00000000"};

  for (int i = 0; i < 16; i++) {
    state[i] = strtoul(init_state_char[i], NULL, 16);
  }

  memcpy(init_state, state, sizeof(uint32_t) * 16);

  for (int i = 0; i < 10; i++) {
    full_round(state);
  }

  add_state(16, state, init_state);
  serialize(state);

  assert_memory_equal(16, (void *)state, fixture);
  return MUNIT_OK;
}

#pragma clang diagnostic pop
