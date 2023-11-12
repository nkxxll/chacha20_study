#include "chacha20.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_state(uint32_t state[16]);
void test_one_round(int verbose);
void get_expected_test_one_round(uint32_t expected_state[16]);
void test_full_run(int verbose);
void assert_eq_state(uint32_t state[16], uint32_t expected_state[16]);

int main(int argc, char *argv[]) {
  int verbose = 1;
  test_one_round(0);
  test_full_run(verbose);

  return EXIT_SUCCESS;
}

void test_one_round(int verbose) {
  uint32_t state[16];
  uint32_t expected_state[16];
  get_expected_test_one_round(expected_state);

  get_state(state);
  quad_round(state, 2, 7, 8, 13);
  if (verbose) {
    print_state(state);
    print_state(expected_state);
  }

  assert_eq_state(state, expected_state);
  printf("TEST_ONE_ROUND SUCCESS\n");
}

void get_expected_test_one_round(uint32_t expected_state[16]) {
  printf("### TEST ONE ROUND ###\n");
  char *char_ex_state[16] = {"879531e0", "c5ecf37d", "bdb886dc", "c9a62f8a",
                             "44c20ef3", "3390af7f", "d9fc690b", "cfacafd2",
                             "e46bea80", "b00a5631", "974c541a", "359e9963",
                             "5c971061", "ccc07c79", "2098d9d6", "91dbd320"};
  for (int i = 0; i < 16; i++) {
    expected_state[i] = strtoul(char_ex_state[i], NULL, 16);
  }
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
  for (int i = 0; i < 16; i++) {
    state[i] =  strtoul(init_state_char[i], NULL, 16);
    expected_state[i] = strtoul(expected_state_char[i], NULL, 16);
  }

  if (verbose) {
    printf("state before\n");
    print_state(state);
    printf("expected\n");
    print_state(expected_state);
    printf("--- applying full round 10 times ---\n");
  }

  for (int i = 0; i < 10; i++) {
    full_round(state);
  }

  if (verbose) {
    printf("state\n");
    print_state(state);
    printf("expected\n");
    print_state(expected_state);
  }
  assert_eq_state(state, expected_state);
  printf("TEST SUCCESS\n");
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
