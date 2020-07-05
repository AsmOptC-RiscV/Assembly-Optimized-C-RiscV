#include "../../Permutation/ascon.h"
#include "../../kat.h"
#include "../crypto_aead.h"

#define RATE (64 / 8)
#define PA_ROUNDS 12
#define PB_ROUNDS 6
#define IV                                                                     \
    ((u64)(8 * (CRYPTO_KEYBYTES)) << 56 | (u64)(8 * (RATE)) << 48 |            \
     (u64)(PA_ROUNDS) << 40 | (u64)(PB_ROUNDS) << 32)

#define P12(s) (ascon_ref((s), 0xf0))
#define P8(s) (ascon_ref((s), 0xb4))
#define P6(s) (ascon_ref((s), 0x96))

int crypto_aead_encrypt_ref(unsigned char *c, unsigned long long *clen,
                            const unsigned char *m, unsigned long long mlen,
                            const unsigned char *ad, unsigned long long adlen,
                            const unsigned char *nsec,
                            const unsigned char *npub, const unsigned char *k) {
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

int crypto_aead_decrypt_ref(unsigned char *m, unsigned long long *mlen,
                            unsigned char *nsec, const unsigned char *c,
                            unsigned long long clen, const unsigned char *ad,
                            unsigned long long adlen, const unsigned char *npub,
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
