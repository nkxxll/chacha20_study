#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "chacha20.h"

int main(int argc, char *argv[]) {
  /*
   * ChaCha state:
   *
   *  0  1  2  3
   *  4  5  6  7
   *  8  9 10 11
   * 12 13 14 15
   *
   *  cccccccc  cccccccc  cccccccc  cccccccc
   *  kkkkkkkk  kkkkkkkk  kkkkkkkk  kkkkkkkk
   *  kkkkkkkk  kkkkkkkk  kkkkkkkk  kkkkkkkk
   *  bbbbbbbb  nnnnnnnn  nnnnnnnn  nnnnnnnn
   */
  // read in 16 32 bit unsigned hex strings and convert to uint_32
  uint32_t state[16];
  char buf[128];
  get_state(state);

  uint32_t init_state[16];
  // save the init state for later
  memcpy(init_state, state, 16 * sizeof(uint32_t));

  for (int i; i < 10; i++) {
    quad_round(state, 0, 4, 8, 12);
    quad_round(state, 1, 5, 9, 13);
    quad_round(state, 2, 6, 10, 14);
    quad_round(state, 3, 7, 11, 15);
    quad_round(state, 0, 5, 10, 14);
    quad_round(state, 1, 6, 11, 13);
    quad_round(state, 2, 7, 8, 13);
    quad_round(state, 3, 4, 9, 14);
  }

  // you don't realy need the size because the state is always 16 uint32_t long
  add_state(16, state, init_state);
  serialize(state);

  for (int i = 0; i < 16; i++) {
    printf("%x ", state[i]);
    if (i % 4 == 0)
      printf("\n");
  }

  return EXIT_SUCCESS;
}
