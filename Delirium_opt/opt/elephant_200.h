#include "../kat.h"
#include "crypto_aead.h"
#include "interleaving.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keccak.h"
#include <stdint.h>

#ifndef ELEPHANT_200
#define ELEPHANT_200

#define BLOCK_SIZE 25
typedef unsigned long long SIZE;

void lfsr_step(uint8_t *output, uint8_t *input);

// void get_c_block_4(uint32_t *output, const uint32_t *c, uint32_t clen, uint32_t i);

// void get_ad_block_4(uint32_t *output, const uint32_t *ad, uint32_t adlen, uint32_t i);

#endif
