#include "../kat.h"
#include "crypto_aead_ref.h"
#include <string.h> 
#include <stdio.h>
#include "keccak_ref.h"

#ifndef ELEPHANT_200
#define ELEPHANT_200

#define BLOCK_SIZE 25
typedef unsigned char BYTE;
typedef unsigned long long SIZE;

void lfsr_step_ref(BYTE* output, BYTE* input);

void get_ad_block(BYTE* output, const BYTE* ad, SIZE adlen, const BYTE* npub, SIZE i);

void get_c_block(BYTE* output, const BYTE* c, SIZE clen, SIZE i);

#endif