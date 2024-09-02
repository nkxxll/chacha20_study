#pragma once
#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>
#include <stdlib.h>

void full_round(uint32_t state[16]);
void quad_round(uint32_t *state, int a, int b, int c, int d);
void add_state(size_t size, uint32_t dest_state[size],
               uint32_t src_state[size]);
void serialize(uint32_t *state);
void get_state(uint32_t state[16]);
int big_to_little(int number);

char *encrypt(uint32_t constants[4], uint32_t key[8], uint32_t counter[1],
              uint32_t nonce[3], char *plaintext);

#endif // !CHACHA20_H
