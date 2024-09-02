#include <string.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#define MUNIT_ENABLE_ASSERT_ALIASES
#include "chacha20.h"
#include "munit.h"
#include "test_chacha.h"
#include <stdint.h>

const uint32_t constants[4] = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};

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

void *test_encryption_setup(const MunitParameter params[], void *user_data) {
  char expected[] = {
      0x6e, 0x2e, 0x35, 0x9a, 0x25, 0x68, 0xf9, 0x80, 0x41, 0xba, 0x07, 0x28,
      0xdd, 0x0d, 0x69, 0x81, 0xe9, 0x7e, 0x7a, 0xec, 0x1d, 0x43, 0x60, 0xc2,
      0x0a, 0x27, 0xaf, 0xcc, 0xfd, 0x9f, 0xae, 0x0b, 0xf9, 0x1b, 0x65, 0xc5,
      0x52, 0x47, 0x33, 0xab, 0x8f, 0x59, 0x3d, 0xab, 0xcd, 0x62, 0xb3, 0x57,
      0x16, 0x39, 0xd6, 0x24, 0xe6, 0x51, 0x52, 0xab, 0x8f, 0x53, 0x0c, 0x35,
      0x9f, 0x08, 0x61, 0xd8, 0x07, 0xca, 0x0d, 0xbf, 0x50, 0x0d, 0x6a, 0x61,
      0x56, 0xa3, 0x8e, 0x08, 0x8a, 0x22, 0xb6, 0x5e, 0x52, 0xbc, 0x51, 0x4d,
      0x16, 0xcc, 0xf8, 0x06, 0x81, 0x8c, 0xe9, 0x1a, 0xb7, 0x79, 0x37, 0x36,
      0x5a, 0xf9, 0x0b, 0xbf, 0x74, 0xa3, 0x5b, 0xe6, 0xb4, 0x0b, 0x8e, 0xed,
      0xf2, 0x78, 0x5e, 0x42, 0x87, 0x4d,
  };
  void *expected_state = malloc(strlen(expected) * sizeof(char));
  return (void *)expected_state;
}

MunitResult test_encryption(const MunitParameter params[], void *fixture) {
  uint32_t key[8] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
                     0x10111213, 0x14151617, 0x18191a1b, 0x1c1d1e1f};

  uint32_t nonce[3] = {0x00000000, 0x0000004a, 0x00000000};
  uint32_t counter[1] = {1};
  char plaintext[] = {
      (char)0x4c, (char)0x61, (char)0x64, (char)0x69, (char)0x65, (char)0x73,
      (char)0x20, (char)0x61, (char)0x6e, (char)0x64, (char)0x20, (char)0x47,
      (char)0x65, (char)0x6e, (char)0x74, (char)0x6c, (char)0x65, (char)0x6d,
      (char)0x65, (char)0x6e, (char)0x20, (char)0x6f, (char)0x66, (char)0x20,
      (char)0x74, (char)0x68, (char)0x65, (char)0x20, (char)0x63, (char)0x6c,
      (char)0x61, (char)0x73, (char)0x73, (char)0x20, (char)0x6f, (char)0x66,
      (char)0x20, (char)0x27, (char)0x39, (char)0x39, (char)0x3a, (char)0x20,
      (char)0x49, (char)0x66, (char)0x20, (char)0x49, (char)0x20, (char)0x63,
      (char)0x6f, (char)0x75, (char)0x6c, (char)0x64, (char)0x20, (char)0x6f,
      (char)0x66, (char)0x66, (char)0x65, (char)0x72, (char)0x20, (char)0x79,
      (char)0x6f, (char)0x75, (char)0x20, (char)0x6f, (char)0x6e, (char)0x6c,
      (char)0x79, (char)0x20, (char)0x6f, (char)0x6e, (char)0x65, (char)0x20,
      (char)0x74, (char)0x69, (char)0x70, (char)0x20, (char)0x66, (char)0x6f,
      (char)0x72, (char)0x20, (char)0x74, (char)0x68, (char)0x65, (char)0x20,
      (char)0x66, (char)0x75, (char)0x74, (char)0x75, (char)0x72, (char)0x65,
      (char)0x2c, (char)0x20, (char)0x73, (char)0x75, (char)0x6e, (char)0x73,
      (char)0x63, (char)0x72, (char)0x65, (char)0x65, (char)0x6e, (char)0x20,
      (char)0x77, (char)0x6f, (char)0x75, (char)0x6c, (char)0x64, (char)0x20,
      (char)0x62, (char)0x65, (char)0x20, (char)0x69, (char)0x74, (char)0x2e,
  };
  encrypt(constants, key, counter, nonce, plaintext);
  assert_memory_equal(strlen(plaintext) * sizeof(char), plaintext, fixture);
  return MUNIT_OK;
}

#pragma clang diagnostic pop
