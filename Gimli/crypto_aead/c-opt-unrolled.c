#include "crypto_aead.h"

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

inline static void __attribute__((always_inline))
gimli_c_opt_unrolled(uint32_t *x0, uint32_t *x1, uint32_t *x2, uint32_t *x3,
                     uint32_t *y0, uint32_t *y1, uint32_t *y2, uint32_t *y3,
                     uint32_t *z0, uint32_t *z1, uint32_t *z2, uint32_t *z3) {
    uint32_t t0, t1, cst, round;
    cst = 0x9e377900;
    for (round = cst + 24; round > cst; round -= 4) {
        spbox(x0, y0, z0, &t0, &t1);
        spbox(x1, y1, z1, &t0, &t1);
        spbox(x2, y2, z2, &t0, &t1);
        spbox(x3, y3, z3, &t0, &t1);

        // small swap: pattern s...s...s... etc.
        // add constant: pattern c...c...c... etc.
        *x1 = *x1 ^ round;

        spbox(x1, y0, z0, &t0, &t1);
        spbox(x0, y1, z1, &t0, &t1);
        spbox(x3, y2, z2, &t0, &t1);
        spbox(x2, y3, z3, &t0, &t1);

        spbox(x1, y0, z0, &t0, &t1);
        spbox(x0, y1, z1, &t0, &t1);
        spbox(x3, y2, z2, &t0, &t1);
        spbox(x2, y3, z3, &t0, &t1);

        // big swap: pattern ..S...S...S. etc.

        spbox(x3, y0, z0, &t0, &t1);
        spbox(x2, y1, z1, &t0, &t1);
        spbox(x1, y2, z2, &t0, &t1);
        spbox(x0, y3, z3, &t0, &t1);

        spbox(x3, y0, z0, &t0, &t1);
        spbox(x2, y1, z1, &t0, &t1);
        spbox(x1, y2, z2, &t0, &t1);
        spbox(x0, y3, z3, &t0, &t1);

        // small swap: pattern s...s...s... etc.
        // add constant: pattern c...c...c... etc.
        round -= 4;
        *x2 = *x2 ^ round;

        spbox(x2, y0, z0, &t0, &t1);
        spbox(x3, y1, z1, &t0, &t1);
        spbox(x0, y2, z2, &t0, &t1);
        spbox(x1, y3, z3, &t0, &t1);

        spbox(x2, y0, z0, &t0, &t1);
        spbox(x3, y1, z1, &t0, &t1);
        spbox(x0, y2, z2, &t0, &t1);
        spbox(x1, y3, z3, &t0, &t1);

        // big swap: pattern ..S...S...S. etc.

        spbox(x0, y0, z0, &t0, &t1);
        spbox(x1, y1, z1, &t0, &t1);
        spbox(x2, y2, z2, &t0, &t1);
        spbox(x3, y3, z3, &t0, &t1);
    }
}

int crypto_aead_encrypt_c_opt_unrolled(
    unsigned char *c, unsigned long long *clen, const unsigned char *m,
    unsigned long long mlen, const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec, const unsigned char *npub,
    const unsigned char *k) {
    (void)nsec;
    uint32_t state[4];
    const unsigned char *from = ad;
    unsigned long long from_len = adlen;
    unsigned long long i;
    uint32_t x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3;
    x0 = ((uint32_t *)npub)[0];
    x1 = ((uint32_t *)npub)[1];
    x2 = ((uint32_t *)npub)[2];
    x3 = ((uint32_t *)npub)[3];
    y0 = ((uint32_t *)k)[0];
    y1 = ((uint32_t *)k)[1];
    y2 = ((uint32_t *)k)[2];
    y3 = ((uint32_t *)k)[3];
    z0 = ((uint32_t *)k)[4];
    z1 = ((uint32_t *)k)[5];
    z2 = ((uint32_t *)k)[6];
    z3 = ((uint32_t *)k)[7];

    int phase = 2;
    // 2 absorb AD
    // 1 absorb M and encrypt into C
    // 0 generate tag.

    *clen = mlen + 16;

    while (phase > 0) {
        gimli_c_opt_unrolled(&x0, &x1, &x2, &x3, &y0, &y1, &y2, &y3, &z0, &z1,
                             &z2, &z3);

        if (from_len >= 16) {
            x0 ^= ((uint32_t *)from)[0];
            x1 ^= ((uint32_t *)from)[1];
            x2 ^= ((uint32_t *)from)[2];
            x3 ^= ((uint32_t *)from)[3];
            from += 16;
            from_len -= 16;
            if (phase == 1) {
                ((uint32_t *)c)[0] = x0;
                ((uint32_t *)c)[1] = x1;
                ((uint32_t *)c)[2] = x2;
                ((uint32_t *)c)[3] = x3;
                c += 16;
            }
        } else {
            state[0] = x0;
            state[1] = x1;
            state[2] = x2;
            state[3] = x3;
            for (i = 0; i < from_len; ++i)
                ((uint8_t *)state)[i] ^= from[i];
            ((uint8_t *)state)[from_len] ^= 1;
            x0 = state[0];
            x1 = state[1];
            x2 = state[2];
            x3 = state[3];
            z3 ^= 1 << 24;
            phase -= 1;
            if (phase > 0) {
                from = m;
                from_len = mlen;
            } else {
                for (i = 0; i < from_len; ++i)
                    c[i] = ((uint8_t *)state)[i];
                c += from_len;
            }
        }
    }
    gimli_c_opt_unrolled(&x0, &x1, &x2, &x3, &y0, &y1, &y2, &y3, &z0, &z1, &z2,
                         &z3);

    state[0] = x0;
    state[1] = x1;
    state[2] = x2;
    state[3] = x3;
    for (i = 0; i < 16; ++i)
        c[i] = ((uint8_t *)state)[i];

    return 0;
}
