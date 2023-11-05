#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void quad_round(uint32_t *state, int a, int b, int c, int d);
void add_state(uint32_t *dest_state, uint32_t *src_state);
void serialize(uint32_t *state);

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
  fgets(buf, 128, stdin);
  for (int i = 0; i < 16; i++) {
    int start_idx = i * 8;
    int end_idx = (i + 1) * 8;
    char hex_str[8];
    // so was in der art oder so
    strncpy(hex_str, buf + i * 8, 8);
    state[i] = strtoul(hex_str, NULL, 16);
  }
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

  add_state(state, init_state);
  serialize(state);

  return EXIT_SUCCESS;
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