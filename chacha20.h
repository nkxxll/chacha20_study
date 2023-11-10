#pragma once
#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>
#include <stdlib.h>

void quad_round(uint32_t *state, int a, int b, int c, int d);
void add_state(size_t size, uint32_t dest_state[size],
               uint32_t src_state[size]);
void serialize(uint32_t *state);
void get_state(uint32_t state[16]);
int big_to_little(int number);

#endif // !CHACHA20_H
