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

inline static void __attribute__((always_inline)) gimli_c_opt(uint32_t *state) {
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

inline static void __attribute__((always_inline))
gimli_c_opt_lg(uint32_t *x0, uint32_t *x1, uint32_t *x2, uint32_t *x3,
               uint32_t *y0, uint32_t *y1, uint32_t *y2, uint32_t *y3,
               uint32_t *z0, uint32_t *z1, uint32_t *z2, uint32_t *z3) {
    uint32_t t0, t1, cst, round, c2;
    cst = 0x9e377900;
    c2 = 2;
    for (round = 24; round > 0; --round) {
        spbox(x0, y0, z0, &t0, &t1);
        spbox(x1, y1, z1, &t0, &t1);
        spbox(x2, y2, z2, &t0, &t1);
        spbox(x3, y3, z3, &t0, &t1);

        t0 = round & 3;
        if (t0 == c2) { // big swap: pattern ..S...S...S. etc.
            *x0 = *x0 ^ *x2;
            *x2 = *x0 ^ *x2;
            *x0 = *x0 ^ *x2;
            *x1 = *x1 ^ *x3;
            *x3 = *x1 ^ *x3;
            *x1 = *x1 ^ *x3;
        }

        if (t0 == 0) {
            // small swap: pattern s...s...s... etc.
            // add constant: pattern c...c...c... etc.
            *x0 = *x0 ^ *x1;
            *x1 = *x0 ^ *x1;
            *x0 = *x0 ^ *x1;
            *x2 = *x2 ^ *x3;
            *x3 = *x2 ^ *x3;
            *x2 = *x2 ^ *x3;
            t0 = round + cst;
            *x0 = *x0 ^ t0;
        }
    }
}

int crypto_aead_encrypt_c_opt(unsigned char *c, unsigned long long *clen,
                              const unsigned char *m, unsigned long long mlen,
                              const unsigned char *ad, unsigned long long adlen,
                              const unsigned char *nsec,
                              const unsigned char *npub,
                              const unsigned char *k) {
    (void)nsec;
    uint32_t state[12];
    unsigned long long i;

    state[0] = ((uint32_t *)npub)[0];
    state[1] = ((uint32_t *)npub)[1];
    state[2] = ((uint32_t *)npub)[2];
    state[3] = ((uint32_t *)npub)[3];
    state[4] = ((uint32_t *)k)[0];
    state[5] = ((uint32_t *)k)[1];
    state[6] = ((uint32_t *)k)[2];
    state[7] = ((uint32_t *)k)[3];
    state[8] = ((uint32_t *)k)[4];
    state[9] = ((uint32_t *)k)[5];
    state[10] = ((uint32_t *)k)[6];
    state[11] = ((uint32_t *)k)[7];
    gimli_c_opt(state);
    *clen = mlen + 16;

    while (adlen >= 16) {
        state[0] ^= ((uint32_t *)ad)[0];
        state[1] ^= ((uint32_t *)ad)[1];
        state[2] ^= ((uint32_t *)ad)[2];
        state[3] ^= ((uint32_t *)ad)[3];
        gimli_c_opt(state);
        ad += 16;
        adlen -= 16;
    }

    for (i = 0; i < adlen; ++i)
        ((uint8_t *)state)[i] ^= ad[i];
    ((uint8_t *)state)[47] ^= 1;
    ((uint8_t *)state)[adlen] ^= 1;
    gimli_c_opt(state);

    while (mlen >= 16) {
        ((uint32_t *)c)[0] = state[0] ^= ((uint32_t *)m)[0];
        ((uint32_t *)c)[1] = state[1] ^= ((uint32_t *)m)[1];
        ((uint32_t *)c)[2] = state[2] ^= ((uint32_t *)m)[2];
        ((uint32_t *)c)[3] = state[3] ^= ((uint32_t *)m)[3];
        gimli_c_opt(state);
        c += 16;
        m += 16;
        mlen -= 16;
    }

    for (i = 0; i < mlen; ++i)
        c[i] = ((uint8_t *)state)[i] ^= m[i];
    c += mlen;
    ((uint8_t *)state)[47] ^= 1;
    ((uint8_t *)state)[mlen] ^= 1;
    gimli_c_opt(state);

    for (i = 0; i < 16; ++i)
        c[i] = ((uint8_t *)state)[i];

    return 0;
}

int crypto_aead_decrypt_c_opt(unsigned char *m, unsigned long long *mlen,
                              unsigned char *nsec, const unsigned char *c,
                              unsigned long long clen, const unsigned char *ad,
                              unsigned long long adlen,
                              const unsigned char *npub,
                              const unsigned char *k) {
    (void)nsec;
    uint32_t state_32[12];
    uint8_t *const state = (uint8_t *)state_32;
    uint32_t result;
    unsigned long long i;
    unsigned long long tlen;

    if (clen < 16)
        return -1;
    *mlen = tlen = clen - 16;

    state_32[0] = ((uint32_t *)npub)[0];
    state_32[1] = ((uint32_t *)npub)[1];
    state_32[2] = ((uint32_t *)npub)[2];
    state_32[3] = ((uint32_t *)npub)[3];
    state_32[4] = ((uint32_t *)k)[0];
    state_32[5] = ((uint32_t *)k)[1];
    state_32[6] = ((uint32_t *)k)[2];
    state_32[7] = ((uint32_t *)k)[3];
    state_32[8] = ((uint32_t *)k)[4];
    state_32[9] = ((uint32_t *)k)[5];
    state_32[10] = ((uint32_t *)k)[6];
    state_32[11] = ((uint32_t *)k)[7];
    gimli_c_opt(state_32);

    while (adlen >= 16) {
        state_32[0] ^= ((uint32_t *)ad)[0];
        state_32[1] ^= ((uint32_t *)ad)[1];
        state_32[2] ^= ((uint32_t *)ad)[2];
        state_32[3] ^= ((uint32_t *)ad)[3];
        gimli_c_opt(state_32);
        ad += 16;
        adlen -= 16;
    }

    for (i = 0; i < adlen; ++i)
        state[i] ^= ad[i];
    state[adlen] ^= 1;
    state[47] ^= 1;
    gimli_c_opt(state_32);

    while (tlen >= 16) {
        for (i = 0; i < 16; ++i)
            m[i] = state[i] ^ c[i];
        for (i = 0; i < 16; ++i)
            state[i] = c[i];
        gimli_c_opt(state_32);
        c += 16;
        m += 16;
        tlen -= 16;
    }

    for (i = 0; i < tlen; ++i)
        m[i] = state[i] ^ c[i];
    for (i = 0; i < tlen; ++i)
        state[i] = c[i];
    c += tlen;
    m += tlen;
    state[tlen] ^= 1;
    state[47] ^= 1;
    gimli_c_opt(state_32);

    result = 0;
    for (i = 0; i < 16; ++i)
        result |= c[i] ^ state[i];
    result -= 1;
    result = ((int32_t)result) >> 16;

    tlen = *mlen;
    m -= tlen;
    for (i = 0; i < tlen; ++i)
        m[i] &= result;

    return ~result;
}
