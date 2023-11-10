#include "chacha20.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void get_state(uint32_t state[16]) {
  // get the state to the chacha function in one way or another
  // TODO: add the test state here
  char *char_states[16] = {"879531e0", "c5ecf37d", "516461b1", "c9a62f8a",
                           "44c20ef3", "3390af7f", "d9fc690b", "2a5f714c",
                           "53372767", "b00a5631", "974c541a", "359e9963",
                           "5c971061", "3d631689", "2098d9d6", "91dbd320"};
  for (int i = 0; i < 16; i++) {
    state[i] = strtoul(char_states[i], NULL, 16);
  }
}

void quad_round(uint32_t *state, int a, int b, int c, int d) {
  // 1.  a += b; d ^= a; d <<<= 16;
  // 2.  c += d; b ^= c; b <<<= 12;
  // 3.  a += b; d ^= a; d <<<= 8;
  // 4.  c += d; b ^= c; b <<<= 7;
  state[a] += state[b];
  state[d] ^= state[a];
  state[d] = (state[d] << 16) | (state[d] >> (32 - 16));
  state[c] += state[d];
  state[b] ^= state[c];
  state[b] = (state[b] << 12) | (state[b] >> (32 - 12));
  state[a] += state[b];
  state[d] ^= state[a];
  state[d] = (state[d] << 8) | (state[d] >> (32 - 8));
  state[c] += state[d];
  state[b] ^= state[c];
  state[b] = (state[b] << 7) | (state[b] >> (32 - 7));
}

void add_state(size_t size, uint32_t dest_state[size],
               uint32_t src_state[size]) {
  for (int i = 0; i < size; i++) {
    dest_state[i] += src_state[i];
  }
}

void serialize(uint32_t *state) {
  for (int i = 0; i < 16; i++) {
    state[i] = big_to_little(state[i]);
  }
}

int big_to_little(int number) {
  return ((number >> 24) & 0xff) | ((number << 8) & 0xff0000) |
         ((number >> 8) & 0xff00) | ((number << 24) & 0xff000000);
}
