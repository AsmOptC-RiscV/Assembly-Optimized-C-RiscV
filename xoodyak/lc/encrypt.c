/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "crypto_aead.h"
#include "api.h"
#include "Xoodyak.h"
#include <string.h>

#if !defined(CRYPTO_KEYBYTES)
    #define CRYPTO_KEYBYTES     16
#endif
#if !defined(CRYPTO_NPUBBYTES)
    #define CRYPTO_NPUBBYTES    16
#endif

#define    TAGLEN        16

int crypto_aead_encrypt_lc(
    unsigned char *c, unsigned long long *clen,
    const unsigned char *m, unsigned long long mlen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k)
{
    Xoodyak_lc_Instance    instance;

    (void)nsec;

    Xoodyak_lc_Initialize(&instance, k, CRYPTO_KEYBYTES, NULL, 0, NULL, 0);
    Xoodyak_lc_Absorb(&instance, npub, CRYPTO_NPUBBYTES);
    Xoodyak_lc_Absorb(&instance, ad, (size_t)adlen);
    Xoodyak_lc_Encrypt(&instance, m, c, (size_t)mlen);
    Xoodyak_lc_Squeeze(&instance, c + mlen, TAGLEN);
    *clen = mlen + TAGLEN;
    #if 0
    {
        unsigned int i;
        for (i = 0; i < *clen; ++i )
        {
            printf("\\x%02x", c[i] );
        }
        printf("\n");
    }
    #endif
    return 0;
}

int crypto_aead_decrypt_lc(
    unsigned char *m, unsigned long long *mlen,
    unsigned char *nsec,
    const unsigned char *c, unsigned long long clen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *npub,
    const unsigned char *k)
{
    Xoodyak_lc_Instance    instance;
    unsigned char        tag[TAGLEN];
    unsigned long long    mlen_;

    (void)nsec;

    *mlen = 0;
    if (clen < TAGLEN) {
        return -1;
    }
    mlen_ = clen - TAGLEN;
    Xoodyak_lc_Initialize(&instance, k, CRYPTO_KEYBYTES, NULL, 0, NULL, 0);
    Xoodyak_lc_Absorb(&instance, npub, CRYPTO_NPUBBYTES);
    Xoodyak_lc_Absorb(&instance, ad, (size_t)adlen);
    Xoodyak_lc_Decrypt(&instance, c, m, (size_t)mlen_);
    Xoodyak_lc_Squeeze(&instance, tag, TAGLEN);
    if (memcmp(tag, c + mlen_, TAGLEN) != 0) {
        memset(m, 0, (size_t)mlen_);
        return -1;
    }
    *mlen = mlen_;
    return 0;
}
