#include "../../kat.h"
#include "../endian.h"
#include "permutations.h"

#define RATE (64 / 8)
#define PA_ROUNDS 12
#define PB_ROUNDS 6
#define IV                                                                     \
    ((u64)(8 * (CRYPTO_KEYBYTES)) << 56 | (u64)(8 * (RATE)) << 48 |            \
     (u64)(PA_ROUNDS) << 40 | (u64)(PB_ROUNDS) << 32)

int crypto_aead_encrypt_bit_interleaving_32(
    unsigned char *c, unsigned long long *clen, const unsigned char *m,
    unsigned long long mlen, const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec, const unsigned char *npub,
    const unsigned char *k) {
    u32_2 K0, K1, N0, N1;
    u32_2 x0, x1, x2, x3, x4;
    u32_2 t0, t1, t2, t3, t4;
    u64 tmp0, tmp1;
    u32 i;
    (void)nsec;

    // set ciphertext size
    *clen = mlen + CRYPTO_ABYTES;

    // load key and nonce
    to_bit_interleaving(K0, U64BIG(*(u64 *)k));
    to_bit_interleaving(K1, U64BIG(*(u64 *)(k + 8)));
    to_bit_interleaving(N0, U64BIG(*(u64 *)npub));
    to_bit_interleaving(N1, U64BIG(*(u64 *)(npub + 8)));

    // initialization
    to_bit_interleaving(x0, IV);
    x1.o = K0.o;
    x1.e = K0.e;
    x2.e = K1.e;
    x2.o = K1.o;
    x3.e = N0.e;
    x3.o = N0.o;
    x4.e = N1.e;
    x4.o = N1.o;
    P12();
    x3.e ^= K0.e;
    x3.o ^= K0.o;
    x4.e ^= K1.e;
    x4.o ^= K1.o;

    // process associated data
    if (adlen) {
        while (adlen >= RATE) {
            to_bit_interleaving(t0, U64BIG(*(u64 *)ad));
            x0.e ^= t0.e;
            x0.o ^= t0.o;
            P6();
            adlen -= RATE;
            ad += RATE;
        }
        tmp0 = 0;
        for (i = 0; i < adlen; ++i, ++ad)
            tmp0 |= INS_BYTE64(*ad, i);
        tmp0 |= INS_BYTE64(0x80, adlen);
        to_bit_interleaving(t0, tmp0);
        x0.e ^= t0.e;
        x0.o ^= t0.o;
        P6();
    }
    x4.e ^= 1;

    // process plaintext
    while (mlen >= RATE) {
        to_bit_interleaving(t0, U64BIG(*(u64 *)m));
        x0.e ^= t0.e;
        x0.o ^= t0.o;
        from_bit_interleaving(tmp0, x0);
        *(u64 *)c = U64BIG(tmp0);
        P6();
        mlen -= RATE;
        m += RATE;
        c += RATE;
    }
    tmp0 = 0;
    for (i = 0; i < mlen; ++i, ++m)
        tmp0 |= INS_BYTE64(*m, i);
    tmp0 |= INS_BYTE64(0x80, mlen);
    to_bit_interleaving(t0, tmp0);
    x0.e ^= t0.e;
    x0.o ^= t0.o;
    from_bit_interleaving(tmp0, x0);
    for (i = 0; i < mlen; ++i, ++c)
        *c = EXT_BYTE64(tmp0, i);

    // finalization
    x1.e ^= K0.e;
    x1.o ^= K0.o;
    x2.e ^= K1.e;
    x2.o ^= K1.o;
    P12();
    x3.e ^= K0.e;
    x3.o ^= K0.o;
    x4.e ^= K1.e;
    x4.o ^= K1.o;

    // set tag
    from_bit_interleaving(tmp0, x3);
    for (i = 0; i < 8; ++i, ++c) {
        *c = EXT_BYTE64(tmp0, i);
    }
    from_bit_interleaving(tmp1, x4);
    for (i = 0; i < 8; ++i, ++c) {
        *c = EXT_BYTE64(tmp1, i);
    }

    return 0;
}
