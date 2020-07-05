#include "ascon.h"

// This function computes boolean formulas of the substitution layer in
// parallel on the registers s0 through s4, using t0, t1 and t2 as
// temporary registers. This function must be run twice with different
// state registers, because each 64-bit state word is split in two
// 32-bit machine words. Note that after this function, the state words
// end up in different registers. In order to make space to move the
// state words back into position, the final result is stored in
// register r0 instead.

inline static void __attribute__((always_inline))
sbox(uint32_t *s0, uint32_t *s1, uint32_t *s2, uint32_t *s3, uint32_t *s4,
     uint32_t *r0) {
    uint32_t t0, t1, t2;
    t0 = *s1 ^ *s2;
    t1 = *s0 ^ *s4;
    t2 = *s3 ^ *s4;

    *s4 = ((~*s4) | *s3) ^ t0;
    *s3 = ((*s3 ^ *s1) | t0) ^ t1;
    *s2 = ((*s2 ^ t1) | *s1) ^ t2;
    *s1 = (*s1 & ~t1) ^ t2;
    *s0 = *s0 | t2;
    *r0 = *s0 ^ t0;
};

// The xorror function applies the rotates and bitwise additions of the
// linear diffusion layer to a single state word. Because of the
// limitations of assembly functions, it takes many arguments: dl and dh
// are the registers for storing the low and high part of the result.
// sl, sh, sl0, sh0, sl1 and sh1 are the source registers for the state
// word, without rotation, with rotation r0 and with rotation r1
// respectively. Because 32-bit shift instructions can only shift by 31
// at most, representing rotate amounts above 32 is done by subtracting
// 32 and swapping the respective source registers. This is why the
// source registers must be supplied three times.
inline static void __attribute__((always_inline))
xorror(uint32_t *dl, uint32_t *dh, const uint32_t *sl, const uint32_t *sh,
       const uint32_t *sl0, const uint32_t *sh0, const int r0,
       const uint32_t *sl1, const uint32_t *sh1, const int r1) {
    uint32_t t0, t1;
    t0 = *sl0 << (32 - r0);
    t0 ^= *sh0 >> r0;
    t0 ^= *sl1 << (32 - r1);
    t0 ^= *sh1 >> r1;

    t1 = *sh0 << (32 - r0);
    t1 ^= *sl0 >> r0;
    t1 ^= *sh1 << (32 - r1);
    t1 ^= *sl1 >> r1;

    *dl = *sl ^ t1;
    *dh = *sh ^ t0;
}

inline static void __attribute__((always_inline))
round_ascon(uint32_t *x0, uint32_t *x1, uint32_t *x2, uint32_t *x3,
            uint32_t *x4, uint32_t *y0, uint32_t *y1, uint32_t *y2,
            uint32_t *y3, uint32_t *y4, uint32_t *t3, uint32_t *t4,
            int *round_constant) {
    // 	Addition of round constant
    *x2 ^= *round_constant;
    // 	Substitution layer
    sbox(x0, x1, x2, x3, x4, t3);
    sbox(y0, y1, y2, y3, y4, t4);
    // 	Linear diffusion layer
    xorror(x0, y0, x2, y2, x2, y2, 19, x2, y2, 28);
    xorror(x2, y2, x4, y4, x4, y4, 1, x4, y4, 6);
    xorror(x4, y4, x1, y1, x1, y1, 7, y1, x1, 9);
    xorror(x1, y1, x3, y3, y3, x3, 29, y3, x3, 7);
    xorror(x3, y3, t3, t4, t3, t4, 10, t3, t4, 17);

    // 	Compute next round constant
    *round_constant -= 15;
}

// This is a translation of the optimizations made in asm.S.

void ascon_opt_2(state *p, int round_constant) {
    uint32_t x0, x1, x2, x3, x4;
    uint32_t y0, y1, y2, y3, y4;
    uint32_t t3, t4;

    uint32_t *state_32 = (uint32_t *)p;
    // x0-x4 lower halves of state
    // y0-y4 upper halves of state

    x0 = state_32[0];
    y0 = state_32[1];
    x1 = state_32[2];
    y1 = state_32[3];
    x2 = state_32[4];
    y2 = state_32[5];
    x3 = state_32[6];
    y3 = state_32[7];
    x4 = state_32[8];
    y4 = state_32[9];

    // The main permutation loop that loops over all rounds in a
    // permutation, defined in terms of the sbox and xorror function. t5
    // contains the round constant, x0 through x4 contain the least
    // significant halves of the five state words, y0 through y4 contain
    // the most significant halves of the five state words. t0 through t4
    // are used as temporary registers and a5 contains the round constant
    // where the loop should end, 0x3c.
    do {
        round_ascon(&x0, &x1, &x2, &x3, &x4, &y0, &y1, &y2, &y3, &y4, &t3, &t4,
                    &round_constant);
    } while (60 != round_constant);

    state_32[0] = x0;
    state_32[1] = y0;
    state_32[2] = x1;
    state_32[3] = y1;
    state_32[4] = x2;
    state_32[5] = y2;
    state_32[6] = x3;
    state_32[7] = y3;
    state_32[8] = x4;
    state_32[9] = y4;
};