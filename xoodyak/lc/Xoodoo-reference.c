/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

/* #define VERBOSE_LEVEL    0 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Xoodoo.h"

/* ---------------------------------------------------------------- */

void Xoodoo_lc_StaticInitialize( void )
{
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_Initialize(void *state)
{
    memset(state, 0, NLANES*sizeof(tXoodooLane));
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_AddByte(void *state, unsigned char byte, unsigned int offset)
{
    assert(offset < NLANES*sizeof(tXoodooLane));
    ((unsigned char *)state)[offset] ^= byte;
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int i;

    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    for(i=0; i<length; i++)
        ((unsigned char *)state)[offset+i] ^= data[i];
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    memcpy((unsigned char*)state+offset, data, length);
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    assert(byteCount <= NLANES*sizeof(tXoodooLane));
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    memcpy(data, (unsigned char*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void Xoodoo_lc_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    unsigned int i;

    assert(offset < NLANES*sizeof(tXoodooLane));
    assert(offset+length <= NLANES*sizeof(tXoodooLane));
    for(i=0; i<length; i++)
        output[i] = input[i] ^ ((unsigned char *)state)[offset+i];
}

/* ---------------------------------------------------------------- */

#if defined(VERBOSE_LEVEL)

static void Dump(char * text, tXoodooLane * a, unsigned int level)
{
    if (level == VERBOSE_LEVEL) {
    #if 0
        printf("%-8.8s ", text);
        printf("%u %u %u %u - ", a[0+0], a[0+1], a[0+2], a[0+3] );
        printf("%u %u %u %u - ", a[4+0], a[4+1], a[4+2], a[4+3] );
        printf("%u %u %u %u\n", a[8+0], a[8+1], a[8+2], a[8+3] );
        if ((level == 2) && !strcmp(text, "Rho-east"))
            printf("\n");
    #elif 0
        printf("%-8.8s ", text);
        printf("%08x %08x %08x %08x - ", a[0+0], a[0+1], a[0+2], a[0+3] );
        printf("%08x %08x %08x %08x - ", a[4+0], a[4+1], a[4+2], a[4+3] );
        printf("%08x %08x %08x %08x\n", a[8+0], a[8+1], a[8+2], a[8+3] );
    #else
        printf("%s\n", text);
        printf("a00 %08x, a01 %08x, a02 %08x, a03 %08x\n", a[0+0], a[0+1], a[0+2], a[0+3] );
        printf("a10 %08x, a11 %08x, a12 %08x, a13 %08x\n", a[4+0], a[4+1], a[4+2], a[4+3] );
        printf("a20 %08x, a21 %08x, a22 %08x, a23 %08x\n\n", a[8+0], a[8+1], a[8+2], a[8+3] );
    #endif
    }
}

#else

#define Dump(text, a, level)

#endif


static void fromBytesToWords(tXoodooLane *stateAsWords, const unsigned char *state)
{
    unsigned int i, j;

    for(i=0; i<NLANES; i++) {
        stateAsWords[i] = 0;
        for(j=0; j<sizeof(tXoodooLane); j++)
            stateAsWords[i] |= (tXoodooLane)(state[i*sizeof(tXoodooLane)+j]) << (8*j);
    }
}

static void fromWordsToBytes(unsigned char *state, const tXoodooLane *stateAsWords)
{
    unsigned int i, j;

    for(i=0; i<NLANES; i++)
        for(j=0; j<sizeof(tXoodooLane); j++)
            state[i*sizeof(tXoodooLane)+j] = (stateAsWords[i] >> (8*j)) & 0xFF;
}


#define ROTL32(a, offset) ((a << offset) ^ (a >> (32-offset)))

#define Theta_Rho_West_Iota(__rc)           \
    t1 = x03 ^ x13 ^ x23;                   \
    t2 = x00 ^ x10 ^ x20;                   \
    t1 = ROTL32(t1, 5) ^ ROTL32(t1, 14);    \
    t2 = ROTL32(t2, 5) ^ ROTL32(t2, 14);    \
    x00 ^= t1;                              \
    x10 ^= t1;                              \
    x20 ^= t1;                              \
    t1 = x01 ^ x11 ^ x21;                   \
    t1 = ROTL32(t1, 5) ^ ROTL32(t1, 14);    \
    x01 ^= t2;                              \
    x11 ^= t2;                              \
    x21 ^= t2;                              \
    t2 = x02 ^ x12 ^ x22;                   \
    t2 = ROTL32(t2, 5) ^ ROTL32(t2, 14);    \
    x02 ^= t1;                              \
    x12 ^= t1;                              \
    x22 ^= t1;                              \
    x03 ^= t2;                              \
    x13 ^= t2;                              \
    x23 ^= t2;                              \
    t1 = x13;                               \
    x13 = x12;                              \
    x12 = x11;                              \
    x11 = x10;                              \
    x10 = t1;                               \
    x20 = ROTL32(x20, 11);                  \
    x21 = ROTL32(x21, 11);                  \
    x22 = ROTL32(x22, 11);                  \
    x23 = ROTL32(x23, 11);                  \
    x00 ^= __rc;


// for lane complementing
#define chi_pattern_2(c00, c10, c20) \
    c00 = c00 ^ (c10 & c20); \
    c10 = c10 ^ (c20 | c00); \
    c20 = c20 ^ (c00 & c10);

// for lane complementing
#define chi_pattern_4(c00, c10, c20) \
    c00 = c00 ^ (~c10 & c20); \
    c10 = c10 ^ (c20 | c00); \
    c20 = c20 ^ (c00 & ~c10);

#define Rho_East                            \
    x10 = ROTL32(x10, 1);                   \
    x11 = ROTL32(x11, 1);                   \
    x12 = ROTL32(x12, 1);                   \
    x13 = ROTL32(x13, 1);                   \
    t1 = ROTL32(x22, 8);                    \
    t2 = ROTL32(x23, 8);                    \
    x22 = ROTL32(x20, 8);                   \
    x23 = ROTL32(x21, 8);                   \
    x20 = t1;                               \
    x21 = t2;

// for lane complementing
#define Round_pattern_0                     \
    Theta_Rho_West_Iota(rc[round])          \
    chi_pattern_4(x00, x10, x20)            \
    chi_pattern_4(x01, x11, x21)            \
    chi_pattern_4(x02, x12, x22)            \
    chi_pattern_4(x03, x13, x23)            \
    Rho_East

// for lane complementing
#define Round_pattern_1                     \
    Theta_Rho_West_Iota(rc[round+1])        \
    chi_pattern_2(x00, x10, x20)            \
    chi_pattern_2(x01, x11, x21)            \
    chi_pattern_2(x02, x12, x22)            \
    chi_pattern_2(x03, x13, x23)            \
    Rho_East


void xoodoo_lc(uint32_t *state, uint8_t rounds)
{
    static uint16_t const rc[12] = {
        0x0058, 0x0038, 0x03C0, 0x00D0, 0x0120, 0x0014,
        0x0060, 0x002C, 0x0380, 0x00F0, 0x01A0, 0x0012
    };

    uint32_t x00, x01, x02, x03;
    uint32_t x10, x11, x12, x13;
    uint32_t x20, x21, x22, x23;
    uint32_t t1, t2;
    
    x00 = state[0+0];
    x01 = state[0+1];
    x02 = state[0+2];
    x03 = state[0+3];
    x10 = state[4+0];
    x11 = state[4+1];
    x12 = state[4+2];
    x13 = state[4+3];
    x20 = state[8+0];
    x21 = state[8+1];
    x22 = state[8+2];
    x23 = state[8+3];

    // for lane complementing
    // input mask
    x00 = ~x00;
    x01 = ~x01;
    x02 = ~x02;
    x03 = ~x03;

    for (uint8_t round=0; round<rounds; round+=2) {
        
        Round_pattern_0
        Round_pattern_1
        
    }

    // for lane complementing
    // output mask
    // rounds = 12
    x00 = ~x00;
    x01 = ~x01;
    x02 = ~x02;
    x03 = ~x03;

    state[0+0] = x00;
    state[0+1] = x01;
    state[0+2] = x02;
    state[0+3] = x03;
    state[4+0] = x10;
    state[4+1] = x11;
    state[4+2] = x12;
    state[4+3] = x13;
    state[8+0] = x20;
    state[8+1] = x21;
    state[8+2] = x22;
    state[8+3] = x23;
    
}



void Xoodoo_lc_Permute_Nrounds( void * state, uint32_t nr )
{
    tXoodooLane        a[NLANES];
    // unsigned int    i;

    fromBytesToWords(a, (const unsigned char *)state);

    xoodoo_lc(a, nr);

    fromWordsToBytes((unsigned char *)state, a);

}

void Xoodoo_lc_Permute_6rounds( uint32_t * state)
{
    Xoodoo_lc_Permute_Nrounds( state, 6 );
}

void Xoodoo_lc_Permute_12rounds( uint32_t * state)
{
    Xoodoo_lc_Permute_Nrounds( state, 12 );
}
