#include "gimli.h"

inline static void __attribute__((always_inline))
ror(uint32_t *o0, uint32_t *i0, int bits, uint32_t *r0, uint32_t *r1) {
    *r0 = *i0 >> bits;
    *r1 = *i0 << (32 - bits);
    *o0 = *r0 | *r1;
}

inline static void __attribute__((always_inline))
spbox(uint32_t *x, uint32_t *y, uint32_t *z, uint32_t *u, uint32_t *v) {
    /* START SPBOX */    /* u = rotate x <<< 24 */
    ror(u, x, 8, u, x);  /* v = rotate y <<< 9 */
    ror(v, y, 23, v, y); /* compute z (post swap: x) */
    *x = *u & *v;
    *x = *x << 3;
    *x = *x ^ *v;
    *x = *x ^ *z; /* compute y */
    *y = *u | *z;
    *y = *y << 1;
    *y = *y ^ *u;
    *y = *y ^ *v; /* compute x (post swap: z) */
    *v = *v & *z;
    *v = *v << 2;
    *v = *u ^ *v;
    *z = *z << 1;
    *z = *z ^ *v; /* END SPBOX */
}

void gimli_c_opt(uint32_t *state) {
    uint32_t x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3, t0, t1, cst, round,
        c2;

    x0 = state[0];
    x1 = state[1];
    x2 = state[2];
    x3 = state[3];
    y0 = state[4];
    y1 = state[5];
    y2 = state[6];
    y3 = state[7];
    z0 = state[8];
    z1 = state[9];
    z2 = state[10];
    z3 = state[11];

    cst = 0x9e377900;
    c2 = 2;
    for (round = 24; round > 0; --round) {
        spbox(&x0, &y0, &z0, &t0, &t1);
        spbox(&x1, &y1, &z1, &t0, &t1);
        spbox(&x2, &y2, &z2, &t0, &t1);
        spbox(&x3, &y3, &z3, &t0, &t1);

        t0 = round & 3;
        if (t0 == c2) { // big swap: pattern ..S...S...S. etc.
            x0 = x0 ^ x2;
            x2 = x0 ^ x2;
            x0 = x0 ^ x2;
            x1 = x1 ^ x3;
            x3 = x1 ^ x3;
            x1 = x1 ^ x3;
        }

        if (t0 == 0) {
            // small swap: pattern s...s...s... etc.
            // add constant: pattern c...c...c... etc.
            x0 = x0 ^ x1;
            x1 = x0 ^ x1;
            x0 = x0 ^ x1;
            x2 = x2 ^ x3;
            x3 = x2 ^ x3;
            x2 = x2 ^ x3;
            t0 = round + cst;
            x0 = x0 ^ t0;
        }
    }

    state[0] = x0;
    state[1] = x1;
    state[2] = x2;
    state[3] = x3;
    state[4] = y0;
    state[5] = y1;
    state[6] = y2;
    state[7] = y3;
    state[8] = z0;
    state[9] = z1;
    state[10] = z2;
    state[11] = z3;
}
