#include "chacha20.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_state(uint32_t state[16]);
void test_one_round(int verbose);

int main(int argc, char *argv[]) {
  int verbose = 0;
  if (strcmp(argv[1], "-v")) {
    verbose = 1;
  }
  test_one_round(verbose);

  return EXIT_SUCCESS;
}

void test_one_round(int verbose) {
  uint32_t state[16];
  uint32_t expected_state[16];

  get_state(state);
  quad_round(state, 2, 7, 8, 13);
  if (verbose)
    print_state(state);

  for (int i = 0; i < 16; i++) {
    assert(state[i] == expected_state[i]);
  }
  printf("TEST_ONE_ROUND SUCCESS\n");
}

void print_state(uint32_t state[16]) {
  for (int i = 0; i < 16; i++) {
    printf("%x ", state[i]);
    if ((i + 1) % 4 == 0)
      printf("\n");
  }
}
