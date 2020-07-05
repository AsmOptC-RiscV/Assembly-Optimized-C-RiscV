#pragma once

#include <stdint.h>
#include <string.h>

int crypto_aead_encrypt(unsigned char *c, unsigned long long *clen,
                        const unsigned char *m, unsigned long long mlen,
                        const unsigned char *ad, unsigned long long adlen,
                        const unsigned char *nsec, const unsigned char *npub,
                        const unsigned char *k);

int crypto_aead_encrypt_asm(unsigned char *c, unsigned long long *clen,
                            const unsigned char *m, unsigned long long mlen,
                            const unsigned char *ad, unsigned long long adlen,
                            const unsigned char *nsec,
                            const unsigned char *npub, const unsigned char *k);

int crypto_aead_encrypt_c_opt(unsigned char *c, unsigned long long *clen,
                              const unsigned char *m, unsigned long long mlen,
                              const unsigned char *ad, unsigned long long adlen,
                              const unsigned char *nsec,
                              const unsigned char *npub,
                              const unsigned char *k);

int crypto_aead_encrypt_c_opt_unrolled(
    unsigned char *c, unsigned long long *clen, const unsigned char *m,
    unsigned long long mlen, const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec, const unsigned char *npub,
    const unsigned char *k);

int crypto_aead_decrypt(unsigned char *m, unsigned long long *mlen,
                        unsigned char *nsec, const unsigned char *c,
                        unsigned long long clen, const unsigned char *ad,
                        unsigned long long adlen, const unsigned char *npub,
                        const unsigned char *k);

int crypto_aead_decrypt_asm(unsigned char *m, unsigned long long *mlen,
                            unsigned char *nsec, const unsigned char *c,
                            unsigned long long clen, const unsigned char *ad,
                            unsigned long long adlen, const unsigned char *npub,
                            const unsigned char *k);

int crypto_aead_decrypt_c_opt(unsigned char *m, unsigned long long *mlen,
                              unsigned char *nsec, const unsigned char *c,
                              unsigned long long clen, const unsigned char *ad,
                              unsigned long long adlen,
                              const unsigned char *npub,
                              const unsigned char *k);
