#include "../../Permutation/ascon.h"
#include "../../kat.h"
#include "../crypto_aead.h"

#define RATE (64 / 8)
#define PA_ROUNDS 12
#define PB_ROUNDS 6
#define IV                                                                     \
    ((u64)(8 * (CRYPTO_KEYBYTES)) << 56 | (u64)(8 * (RATE)) << 48 |            \
     (u64)(PA_ROUNDS) << 40 | (u64)(PB_ROUNDS) << 32)

#define P12(s) (ascon_inlined((s), 0xf0))
#define P6(s) (ascon_inlined((s), 0x96))

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
ascon_inlined(state *p, int round_constant) {
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

    do {
        // 	Addition of round constant
        x2 ^= round_constant;
        // 	Substitution layer
        sbox(&x0, &x1, &x2, &x3, &x4, &t3);
        sbox(&y0, &y1, &y2, &y3, &y4, &t4);
        // 	Linear diffusion layer
        xorror(&x0, &y0, &x2, &y2, &x2, &y2, 19, &x2, &y2, 28);
        xorror(&x2, &y2, &x4, &y4, &x4, &y4, 1, &x4, &y4, 6);
        xorror(&x4, &y4, &x1, &y1, &x1, &y1, 7, &y1, &x1, 9);
        xorror(&x1, &y1, &x3, &y3, &y3, &x3, 29, &y3, &x3, 7);
        xorror(&x3, &y3, &t3, &t4, &t3, &t4, 10, &t3, &t4, 17);

        // 	Compute next round constant
        round_constant -= 15;
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

int crypto_aead_encrypt_ref_c(unsigned char *c, unsigned long long *clen,
                              const unsigned char *m, unsigned long long mlen,
                              const unsigned char *ad, unsigned long long adlen,
                              const unsigned char *nsec,
                              const unsigned char *npub,
                              const unsigned char *k) {
    const u64 K0 = BYTES_TO_U64(k, 8);
    const u64 K1 = BYTES_TO_U64(k + 8, 8);
    const u64 N0 = BYTES_TO_U64(npub, 8);
    const u64 N1 = BYTES_TO_U64(npub + 8, 8);
    state s;
    (void)nsec;

    // set ciphertext size
    *clen = mlen + CRYPTO_ABYTES;

    // initialization
    s.x0 = IV;
    s.x1 = K0;
    s.x2 = K1;
    s.x3 = N0;
    s.x4 = N1;
    P12(&s);
    s.x3 ^= K0;
    s.x4 ^= K1;

    // process associated data
    if (adlen) {
        while (adlen >= RATE) {
            s.x0 ^= BYTES_TO_U64(ad, 8);
            P6(&s);
            adlen -= RATE;
            ad += RATE;
        }
        s.x0 ^= BYTES_TO_U64(ad, adlen);
        s.x0 ^= 0x80ull << (56 - 8 * adlen);
        P6(&s);
    }
    s.x4 ^= 1;

    // process plaintext
    while (mlen >= RATE) {
        s.x0 ^= BYTES_TO_U64(m, 8);
        U64_TO_BYTES(c, s.x0, 8);
        P6(&s);
        mlen -= RATE;
        m += RATE;
        c += RATE;
    }
    s.x0 ^= BYTES_TO_U64(m, mlen);
    s.x0 ^= 0x80ull << (56 - 8 * mlen);
    U64_TO_BYTES(c, s.x0, mlen);
    c += mlen;

    // finalization
    s.x1 ^= K0;
    s.x2 ^= K1;
    P12(&s);
    s.x3 ^= K0;
    s.x4 ^= K1;

    // set tag
    U64_TO_BYTES(c, s.x3, 8);
    U64_TO_BYTES(c + 8, s.x4, 8);

    return 0;
}

int crypto_aead_decrypt_ref_c(unsigned char *m, unsigned long long *mlen,
                              unsigned char *nsec, const unsigned char *c,
                              unsigned long long clen, const unsigned char *ad,
                              unsigned long long adlen,
                              const unsigned char *npub,
                              const unsigned char *k) {
    if (clen < CRYPTO_ABYTES) {
        *mlen = 0;
        return -1;
    }

    const u64 K0 = BYTES_TO_U64(k, 8);
    const u64 K1 = BYTES_TO_U64(k + 8, 8);
    const u64 N0 = BYTES_TO_U64(npub, 8);
    const u64 N1 = BYTES_TO_U64(npub + 8, 8);
    state s;
    u64 c0;
    (void)nsec;

    // set plaintext size
    *mlen = clen - CRYPTO_ABYTES;

    // initialization
    s.x0 = IV;
    s.x1 = K0;
    s.x2 = K1;
    s.x3 = N0;
    s.x4 = N1;
    P12(&s);
    s.x3 ^= K0;
    s.x4 ^= K1;

    // process associated data
    if (adlen) {
        while (adlen >= RATE) {
            s.x0 ^= BYTES_TO_U64(ad, 8);
            P6(&s);
            adlen -= RATE;
            ad += RATE;
        }
        s.x0 ^= BYTES_TO_U64(ad, adlen);
        s.x0 ^= 0x80ull << (56 - 8 * adlen);
        P6(&s);
    }
    s.x4 ^= 1;

    // process plaintext
    clen -= CRYPTO_ABYTES;
    while (clen >= RATE) {
        c0 = BYTES_TO_U64(c, 8);
        U64_TO_BYTES(m, s.x0 ^ c0, 8);
        s.x0 = c0;
        P6(&s);
        clen -= RATE;
        m += RATE;
        c += RATE;
    }
    c0 = BYTES_TO_U64(c, clen);
    U64_TO_BYTES(m, s.x0 ^ c0, clen);
    s.x0 &= ~BYTE_MASK(clen);
    s.x0 |= c0;
    s.x0 ^= 0x80ull << (56 - 8 * clen);
    c += clen;

    // finalization
    s.x1 ^= K0;
    s.x2 ^= K1;
    P12(&s);
    s.x3 ^= K0;
    s.x4 ^= K1;

    // verify tag (should be constant time, check compiler output)
    if (((s.x3 ^ BYTES_TO_U64(c, 8)) | (s.x4 ^ BYTES_TO_U64(c + 8, 8))) != 0) {
        *mlen = 0;
        return -1;
    }

    return 0;
}
