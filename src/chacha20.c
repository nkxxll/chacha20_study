#include "chacha20.h"
#include "munit.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* creates a state and allocates the necessary memory for it */
uint32_t *create_state(uint32_t constants[4], uint32_t key[8],
                       uint32_t counter[1], uint32_t nonce[3]) {
  uint32_t *state = malloc(sizeof(uint32_t[16]));
  // todo: write a macro for this for performance
  for (int i = 0; i < 4; i++) {
    state[i] = constants[i];
  }
  for (int i = 4; i < 12; i++) {
    state[i] = key[i];
  }
  state[13] = counter[0];
  for (int i = 14; i < 16; i++) {
    state[i] = nonce[i];
  }
  return state;
}

void quad_round(uint32_t *state, int a, int b, int c, int d) {
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

/* This function performs a full (horizontal & diagonal) chacha20 round */
void full_round(uint32_t state[16]) {
  quad_round(state, 0, 4, 8, 12);
  quad_round(state, 1, 5, 9, 13);
  quad_round(state, 2, 6, 10, 14);
  quad_round(state, 3, 7, 11, 15);
  quad_round(state, 0, 5, 10, 15);
  quad_round(state, 1, 6, 11, 12);
  quad_round(state, 2, 7, 8, 13);
  quad_round(state, 3, 4, 9, 14);
}

void add_state(size_t size, uint32_t dest_state[size],
               uint32_t src_state[size]) {
  for (unsigned long i = 0; i < size; i++) {
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

char *key_stream_to_char(uint32_t key_stream[16]) {
  uint32_t first = 0xff000000;
  uint32_t second = 0x00ff0000;
  uint32_t third = 0x0000ff00;
  uint32_t fourth = 0x00000ff0;
  char *char_key_stream = malloc(sizeof(char[64]));
  for (int i = 0; i < 16; i++) {
    char_key_stream[4 * i] = key_stream[i] & first;
    char_key_stream[4 * i + 1] = key_stream[i] & second;
    char_key_stream[4 * i + 2] = key_stream[i] & third;
    char_key_stream[4 * i + 3] = key_stream[i] & fourth;
  }
  return char_key_stream;
}

/*chacha20_block(key, counter, nonce):*/
/*  state = constants | key | counter | nonce*/
/*  working_state = state*/
/*  for i=1 upto 10*/
/*     inner_block(working_state)*/
/*     end*/
/*  state += working_state*/
/*  return serialize(state)*/
/*  end*/
uint32_t *block(uint32_t constants[4], uint32_t key[8], uint32_t counter[1],
                uint32_t nonce[3]) {
  uint32_t *state = create_state(constants, key, counter, nonce);
  uint32_t working_state[16];
  memcpy(working_state, state, sizeof(uint32_t[16]));
  for (int i = 0; i < 10; i++) {
    full_round(working_state);
  }
  add_state(16, state, working_state);
  serialize(state);
  return state;
}

/*chacha20_encrypt(key, counter, nonce, plaintext):*/
/*  for j = 0 upto floor(len(plaintext)/64)-1*/
/*     key_stream = chacha20_block(key, counter+j, nonce)*/
/*     block = plaintext[(j*64)..(j*64+63)]*/
/*     encrypted_message +=  block ^ key_stream*/
/*     end*/
/*  if ((len(plaintext) % 64) != 0)*/
/*     j = floor(len(plaintext)/64)*/
/*     key_stream = chacha20_block(key, counter+j, nonce)*/
/*     block = plaintext[(j*64)..len(plaintext)-1]*/
/*     encrypted_message += (block^key_stream)[0..len(plaintext)%64]*/
/*     end*/
/*  return encrypted_message*/
/*  end*/
char *encrypt(uint32_t constants[4], uint32_t key[8], uint32_t counter[1],
              uint32_t nonce[3], char *plaintext) {
  size_t plaintext_len = strlen(plaintext);
  unsigned long i = 0;
  for (; i < (plaintext_len / 64) - 1; i++) {
    uint32_t *key_stream = block(constants, key, counter + i, nonce);
    for (int a = 0; a < 16; a++) {
      fprintf(stderr, "%x", key_stream[a]);
    }
    char *char_key_stream = key_stream_to_char(key_stream);
    free(key_stream);
    for (int j = 0; j < 64; j++) {
      plaintext[i * 64 + j] ^= char_key_stream[j];
    }
    free(char_key_stream);
  }
  if ((plaintext_len % 64) != 0) {
    uint32_t *key_stream = block(constants, key, counter + i, nonce);
    char *char_key_stream = key_stream_to_char(key_stream);
    free(key_stream);
    for (unsigned long j = 0; j < plaintext_len % 64; j++) {
      plaintext[i * 64 + j] ^= char_key_stream[j];
    }
    free(char_key_stream);
  }
  return plaintext;
}
