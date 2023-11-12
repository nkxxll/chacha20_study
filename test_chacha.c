#include "chacha20.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERBOSE 1

void test_full_run(int verbose);
void test_one_round(int verbose);
void test_addstate(int verbose);
void test_serialize(int verbose);
void state_from_char_arr(uint32_t state[16], char *state_char[16]);
void print_state(uint32_t state[16]);
void assert_eq_state(uint32_t state[16], uint32_t expected_state[16]);

int main(int argc, char *argv[]) {
  test_one_round(VERBOSE);
  test_full_run(VERBOSE);
  test_addstate(VERBOSE);
  test_serialize(VERBOSE);

  return EXIT_SUCCESS;
}

void test_one_round(int verbose) {
  printf("### TEST ONE ROUND ###\n");
  uint32_t state[16];
  uint32_t expected_state[16];
  char *char_ex_state[16] = {"879531e0", "c5ecf37d", "bdb886dc", "c9a62f8a",
                             "44c20ef3", "3390af7f", "d9fc690b", "cfacafd2",
                             "e46bea80", "b00a5631", "974c541a", "359e9963",
                             "5c971061", "ccc07c79", "2098d9d6", "91dbd320"};

  state_from_char_arr(expected_state, char_ex_state);

  get_state(state);
  quad_round(state, 2, 7, 8, 13);
  if (verbose) {
    printf("[+] state\n");
    print_state(state);
    printf("[+] expected state\n");
    print_state(expected_state);
  }

  assert_eq_state(state, expected_state);
  printf("TEST_ONE_ROUND SUCCESS\n");
}

void test_full_run(int verbose) {
  printf("### TEST_FULL_ROUND ###\n");

  uint32_t state[16];
  uint32_t expected_state[16];

  char *init_state_char[16] = {"61707865", "3320646e", "79622d32", "6b206574",
                               "03020100", "07060504", "0b0a0908", "0f0e0d0c",
                               "13121110", "17161514", "1b1a1918", "1f1e1d1c",
                               "00000001", "09000000", "4a000000", "00000000"};
  char *expected_state_char[16] = {
      "837778ab", "e238d763", "a67ae21e", "5950bb2f", "c4f2d0c7", "fc62bb2f",
      "8fa018fc", "3f5ec7b7", "335271c2", "f29489f3", "eabda8fc", "82e46ebd",
      "d19c12b4", "b04e16de", "9e83d0cb", "4e3c50a2"};
  // state before
  state_from_char_arr(state, init_state_char);
  state_from_char_arr(expected_state, expected_state_char);

  if (verbose) {
    printf("[+] state before\n");
    print_state(state);
    printf("[+] expected state\n");
    print_state(expected_state);
    printf("--- applying full round 10 times ---\n");
  }

  for (int i = 0; i < 10; i++) {
    full_round(state);
  }

  if (verbose) {
    printf("[+] state\n");
    print_state(state);
    printf("[+] expected state\n");
    print_state(expected_state);
  }
  assert_eq_state(state, expected_state);
  printf("TEST SUCCESS\n");
}

void test_addstate(int verbose) {
  printf("### TEST_ADDSTATE ###\n");

  uint32_t state[16];
  uint32_t expected_state[16];

  char *init_state_char[16] = {"61707865", "3320646e", "79622d32", "6b206574",
                               "03020100", "07060504", "0b0a0908", "0f0e0d0c",
                               "13121110", "17161514", "1b1a1918", "1f1e1d1c",
                               "00000001", "09000000", "4a000000", "00000000"};
  char *expected_state_char[16] = {
      "e4e7f110", "15593bd1", "1fdd0f50", "c47120a3", "c7f4d1c7", "0368c033",
      "9aaa2204", "4e6cd4c3", "466482d2", "09aa9f07", "05d7c214", "a2028bd9",
      "d19c12b5", "b94e16de", "e883d0cb", "4e3c50a2"};

  state_from_char_arr(state, init_state_char);
  state_from_char_arr(expected_state, expected_state_char);
  uint32_t init_state[16];
  memcpy(init_state, state, sizeof(uint32_t) * 16);
  for (int i = 0; i < 10; i++) {
    full_round(state);
  }

  add_state(16, state, init_state);

  if (verbose) {
    printf("[+] state\n");
    print_state(state);
    printf("[+] expected state\n");
    print_state(expected_state);
  }

  assert_eq_state(state, expected_state);

  printf("TEST SUCCESS\n");
}

void test_serialize(int verbose) {
  printf("### TEST_SERIALIZE ###\n");

  uint32_t state[16];
  uint32_t expected_state[16];

  char *init_state_char[16] = {"61707865", "3320646e", "79622d32", "6b206574",
                               "03020100", "07060504", "0b0a0908", "0f0e0d0c",
                               "13121110", "17161514", "1b1a1918", "1f1e1d1c",
                               "00000001", "09000000", "4a000000", "00000000"};
  char *expected_state_char[16] = {
      "10f1e7e4", "d13b5915", "500fdd1f", "a32071c4", "c7d1f4c7", "33c06803",
      "0422aa9a", "c3d46c4e", "d2826446", "079faa09", "14c2d705", "d98b02a2",
      "b5129cd1", "de164eb9", "cbd083e8", "a2503c4e",
  };

  state_from_char_arr(state, init_state_char);
  state_from_char_arr(expected_state, expected_state_char);
  uint32_t init_state[16];
  memcpy(init_state, state, sizeof(uint32_t) * 16);
  for (int i = 0; i < 10; i++) {
    full_round(state);
  }

  add_state(16, state, init_state);
  serialize(state);

  if (verbose) {
    printf("[+] state\n");
    print_state(state);
    printf("[+] expected state\n");
    print_state(expected_state);
  }

  assert_eq_state(state, expected_state);

  printf("TEST SUCCESS\n");
}

void state_from_char_arr(uint32_t state[16], char *state_char[16]) {
  for (int i = 0; i < 16; i++) {
    state[i] = strtoul(state_char[i], NULL, 16);
  }
}

void assert_eq_state(uint32_t state[16], uint32_t expected_state[16]) {
  for (int i = 0; i < 16; i++) {
    assert(state[i] == expected_state[i]);
  }
}

void print_state(uint32_t state[16]) {
  for (int i = 0; i < 16; i++) {
    printf("%x ", state[i]);
    if ((i + 1) % 4 == 0)
      printf("\n");
  }
}
