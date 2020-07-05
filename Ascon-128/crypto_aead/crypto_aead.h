#define params_encr                                                            \
    unsigned char *c, unsigned long long *clen, const unsigned char *m,        \
        unsigned long long mlen, const unsigned char *ad,                      \
        unsigned long long adlen, const unsigned char *nsec,                   \
        const unsigned char *npub, const unsigned char *k

#define params_decr                                                            \
    unsigned char *m, unsigned long long *mlen, unsigned char *nsec,           \
        const unsigned char *c, unsigned long long clen,                       \
        const unsigned char *ad, unsigned long long adlen,                     \
        const unsigned char *npub, const unsigned char *k

int crypto_aead_encrypt_ref(params_encr);
int crypto_aead_decrypt_ref(params_decr);

int crypto_aead_encrypt_ref_asm(params_encr);
int crypto_aead_decrypt_ref_asm(params_decr);

int crypto_aead_encrypt_ref_c(params_encr);
int crypto_aead_decrypt_ref_c(params_decr);

int crypto_aead_encrypt_opt_C6(params_encr);
int crypto_aead_decrypt_opt_C6(params_decr);

int crypto_aead_encrypt_opt64_C2_unrolled(params_encr);
int crypto_aead_decrypt_opt64_C2_unrolled(params_decr);

int crypto_aead_encrypt_bit_interleaving_32(params_encr);
int crypto_aead_decrypt_bit_interleaving_32(params_decr);

int crypto_aead_encrypt_opt64_asm(params_encr);
int crypto_aead_decrypt_opt64_asm(params_decr);

int crypto_aead_encrypt_opt64_C_unrolled(params_encr);
int crypto_aead_decrypt_opt64_C_unrolled(params_decr);
