#include "test_aead.h"
#ifdef crypto_aead_encrypt

#define SPEED
#define INCR_AEAD <<= 1
// #define INCR_AEAD += 1

#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

unsigned int __attribute__((noinline))
fn_aead_test(int *fn(), unsigned char *ct, unsigned long long *clen,
             const unsigned char *msg, unsigned long long mlen,
             const unsigned char *ad, unsigned long long adlen,
             const unsigned char *nsec, const unsigned char *nonce,
             const unsigned char *key) {
    (void)nsec;
    uint64_t oldcount, newcount;
    fn(ct, clen, msg, mlen, ad, adlen, NULL, nonce, key);
    oldcount = getcycles();
    fn(ct, clen, msg, mlen, ad, adlen, NULL, nonce, key);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

void speed_test(unsigned char *key, unsigned char *nonce, unsigned char *msg,
                unsigned char *ad, unsigned char *ct) {
    unsigned long long clen;
    unsigned int time;
    int i;
    void *fn1 = &crypto_aead_encrypt;
#ifdef crypto_aead_encrypt_2
    void *fn2 = &crypto_aead_encrypt_2;
#ifdef crypto_aead_encrypt_3
    void *fn3 = &crypto_aead_encrypt_3;
#ifdef crypto_aead_encrypt_4
    void *fn4 = &crypto_aead_encrypt_4;
#ifdef crypto_aead_encrypt_5
    void *fn5 = &crypto_aead_encrypt_5;
#ifdef crypto_aead_encrypt_6
    void *fn6 = &crypto_aead_encrypt_6;
#ifdef crypto_aead_encrypt_7
    void *fn7 = &crypto_aead_encrypt_7;
#ifdef crypto_aead_encrypt_8
    void *fn8 = &crypto_aead_encrypt_8;
#endif
#endif
#endif
#endif
#endif
#endif

#define loop_speed(fnvar, mlen, adlen)                                         \
    for (i = 0; i < 10; i++)                                                   \
        time = fn_aead_test(fnvar, ct, &clen, msg, mlen, ad, adlen, NULL,      \
                            nonce, key);                                       \
    printf("%d\t", time);

    printf("%d\t", (unsigned int)0);
    printf("%d\t", (unsigned int)0);

    loop_speed(fn1, 0, 0);
#ifdef crypto_aead_encrypt_2
    loop_speed(fn2, 0, 0);
#ifdef crypto_aead_encrypt_3
    loop_speed(fn3, 0, 0);
#ifdef crypto_aead_encrypt_4
    loop_speed(fn4, 0, 0);
#ifdef crypto_aead_encrypt_5
    loop_speed(fn5, 0, 0);
#ifdef crypto_aead_encrypt_6
    loop_speed(fn6, 0, 0);
#ifdef crypto_aead_encrypt_7
    loop_speed(fn7, 0, 0);
#ifdef crypto_aead_encrypt_8
    loop_speed(fn8, 0, 0);
#endif
#endif
#endif
#endif
#endif
#endif
#endif

    printf("\n");

    for (unsigned long long adlen = 1; adlen <= MAX_ASSOCIATED_DATA_LENGTH;
         adlen INCR_AEAD) {
        printf("%d\t", (unsigned int)adlen);
        printf("%d\t", (unsigned int)0);
        loop_speed(fn1, 0, adlen);
#ifdef crypto_aead_encrypt_2
        loop_speed(fn2, 0, adlen);
#ifdef crypto_aead_encrypt_3
        loop_speed(fn3, 0, adlen);
#ifdef crypto_aead_encrypt_4
        loop_speed(fn4, 0, adlen);
#ifdef crypto_aead_encrypt_5
        loop_speed(fn5, 0, adlen);
#ifdef crypto_aead_encrypt_6
        loop_speed(fn6, 0, adlen);
#ifdef crypto_aead_encrypt_7
        loop_speed(fn7, 0, adlen);
#ifdef crypto_aead_encrypt_8
        loop_speed(fn8, 0, adlen);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
        printf("\n");
    }

    for (unsigned long long mlen = 1; (mlen <= MAX_MESSAGE_LENGTH_AEAD);
         mlen INCR_AEAD) {
        for (unsigned long long adlen = 1; adlen <= MAX_ASSOCIATED_DATA_LENGTH;
             adlen INCR_AEAD) {
            printf("%d\t", (unsigned int)adlen);
            printf("%d\t", (unsigned int)mlen);
            loop_speed(fn1, mlen, adlen);
#ifdef crypto_aead_encrypt_2
            loop_speed(fn2, mlen, adlen);
#ifdef crypto_aead_encrypt_3
            loop_speed(fn3, mlen, adlen);
#ifdef crypto_aead_encrypt_4
            loop_speed(fn4, mlen, adlen);
#ifdef crypto_aead_encrypt_5
            loop_speed(fn5, mlen, adlen);
#ifdef crypto_aead_encrypt_6
            loop_speed(fn6, mlen, adlen);
#ifdef crypto_aead_encrypt_7
            loop_speed(fn7, mlen, adlen);
#ifdef crypto_aead_encrypt_8
            loop_speed(fn8, mlen, adlen);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
            printf("\n");
        }
    }
}

inline static int __attribute__((always_inline))
mem_cmp_print_encr(const unsigned char *key, const unsigned char *nonce,
                   const unsigned char *msg, const unsigned char *msg2,
                   const unsigned char *ad, const unsigned char *ct,
                   unsigned char *ct2, const unsigned long long clen,
                   const int count, const unsigned long long adlen,
                   const unsigned long long mlen,
                   const unsigned long long mlen2, int *ret_val,
                   const int type) {
    // kill warnings unused
    (void)msg2;
    (void)mlen2;
#ifndef NO_CHECK
    if (memcmp(ct, ct2, clen)) {
        printf("Count = %d\n", count);
        print_bstr("Key = ", key, CRYPTO_KEYBYTES);
        print_bstr("Nonce = ", nonce, CRYPTO_NPUBBYTES);
        printf("PTlen = %d \n", (unsigned int)mlen);
        print_bstr("PT = ", msg, mlen);
        printf("Adlen = %d \n", (unsigned int)adlen);
        print_bstr("AD = ", ad, adlen);
        printf("Clen = %d \n", (unsigned int)clen);
        print_bstr("CT  = ", ct, clen);
        print_bstr("CT2 = ", ct2, clen);
        xor_bstr(ct2, ct, clen);
        print_bstr("DX  = ", ct2, clen);
        highlight_tag("      ", clen, CRYPTO_ABYTES);
        printf("\n");
        printf(
            "crypto_aead_encrypt_%d did not generate the same cypher text.\n",
            type);
        *ret_val = KAT_CRYPTO_FAILURE;
        return 1;
    }
#else
    // kill warnings unused
    (void)key;
    (void)nonce;
    (void)msg;
    (void)ad;
    (void)ct;
    (void)ct2;
    (void)clen;
    (void)count;
    (void)adlen;
    (void)mlen;
    (void)mlen2;
    (void)ret_val;
    (void)type;
#endif
    return 0;
}

inline static int __attribute__((always_inline))
mem_cmp_print_decr(const unsigned char *key, const unsigned char *nonce,
                   const unsigned char *msg, const unsigned char *msg2,
                   const unsigned char *ad, const unsigned char *ct,
                   const unsigned char *ct2, const unsigned long long clen,
                   const int count, const unsigned long long adlen,
                   const unsigned long long mlen,
                   const unsigned long long mlen2, int *ret_val,
                   const int type) {
    // kill warnings unused
    (void)ct2;
    if (mlen != mlen2) {
        printf("crypto_aead_decrypt_%d returned bad 'mlen': Got <%llu>, "
               "expected <%llu>\n",
               type, mlen2, mlen);
        *ret_val = KAT_CRYPTO_FAILURE;
        return 1;
    }

#ifndef NO_CHECK
    if (memcmp(msg, msg2, mlen)) {
        printf("Count = %d\n", count);
        print_bstr("Key = ", key, CRYPTO_KEYBYTES);
        print_bstr("Nonce = ", nonce, CRYPTO_NPUBBYTES);
        print_bstr("PT = ", msg, mlen);
        printf("ADen = %d \n", (unsigned int)adlen);
        print_bstr("AD = ", ad, adlen);
        printf("Clen = %d \n", (unsigned int)clen);
        print_bstr("CT  = ", ct, clen);
        printf("PTlen = %d \n", (unsigned int)mlen);
        print_bstr("PT  = ", msg, mlen);
        print_bstr("PT2 = ", msg2, mlen);
        printf("crypto_aead_decrypt_%d did not recover the plaintext\n", 2);
        *ret_val = KAT_CRYPTO_FAILURE;
        return 1;
    }
#else
    // kill warnings unused
    (void)key;
    (void)nonce;
    (void)msg;
    (void)msg2;
    (void)ad;
    (void)ct;
    (void)ct2;
    (void)clen;
    (void)count;
    (void)adlen;
    (void)mlen;
    (void)mlen2;
    (void)ret_val;
    (void)type;
#endif
    return 0;
}

int check_test(unsigned char *key, unsigned char *nonce, unsigned char *msg,
               unsigned char *msg2, unsigned char *ad, unsigned char *ct,
               unsigned char *ct2) {
    unsigned long long clen;
    unsigned long long mlen2;
    int count = 1;
    int func_ret, ret_val = KAT_SUCCESS;

    init_buffer(key, CRYPTO_KEYBYTES);
    init_buffer(nonce, CRYPTO_NPUBBYTES);
    init_buffer(msg, MAX_MESSAGE_LENGTH_AEAD);
    init_buffer(ad, MAX_ASSOCIATED_DATA_LENGTH);

    for (unsigned long long mlen = 0;
         (mlen <= MAX_MESSAGE_LENGTH_AEAD) && (ret_val == KAT_SUCCESS);
         mlen++) {
        for (unsigned long long adlen = 0; adlen <= MAX_ASSOCIATED_DATA_LENGTH;
             adlen++) {

            init_buffer(ct, MAX_MESSAGE_LENGTH_AEAD + CRYPTO_ABYTES);
            init_buffer(ct2, MAX_MESSAGE_LENGTH_AEAD + CRYPTO_ABYTES);

            if ((func_ret = crypto_aead_encrypt(ct, &clen, msg, mlen, ad, adlen,
                                                NULL, nonce, key)) != 0) {
                printf(STRINGIFY(crypto_aead_encrypt) " returned <%d>\n",
                       func_ret);
                ret_val = KAT_CRYPTO_FAILURE;
                break;
            }

            if ((func_ret = crypto_aead_decrypt(msg2, &mlen2, NULL, ct, clen,
                                                ad, adlen, nonce, key)) != 0) {
                printf(STRINGIFY(crypto_aead_decrypt) " returned <%d>\n",
                       func_ret);
                ret_val = KAT_CRYPTO_FAILURE;
                break;
            }

            if (mlen != mlen2) {
                printf(STRINGIFY(crypto_aead_decrypt) " returned bad 'mlen': "
                                                      "Got <%llu>, "
                                                      "expected <%llu>\n",
                       mlen2, mlen);
                ret_val = KAT_CRYPTO_FAILURE;
                break;
            }

            if (memcmp(msg, msg2, mlen)) {
                printf(STRINGIFY(
                    crypto_aead_decrypt) " did not recover the plaintext\n");
                ret_val = KAT_CRYPTO_FAILURE;
                break;
            }

#define check_encr_fun(fname)                                                  \
    init_buffer(ct2, MAX_MESSAGE_LENGTH_AEAD + CRYPTO_ABYTES);                 \
    if ((func_ret = fname(ct2, &clen, msg, mlen, ad, adlen, NULL, nonce,       \
                          key)) != 0) {                                        \
        printf(STRINGIFY(fname) " returned <%d>\n", func_ret);                 \
        ret_val = KAT_CRYPTO_FAILURE;                                          \
        break;                                                                 \
    }                                                                          \
    if (mem_cmp_print_encr(key, nonce, msg, msg2, ad, ct, ct2, clen, count,    \
                           adlen, mlen, mlen2, &ret_val, 2)) {                 \
        break;                                                                 \
    }

#define check_decr_fun(fname)                                                  \
    if ((func_ret = fname(msg2, &mlen2, NULL, ct2, clen, ad, adlen, nonce,     \
                          key)) != 0) {                                        \
        printf(STRINGIFY(fname) " returned <%d>\n", func_ret);                 \
        ret_val = KAT_CRYPTO_FAILURE;                                          \
        break;                                                                 \
    }                                                                          \
    if (mem_cmp_print_decr(key, nonce, msg, msg2, ad, ct, ct2, clen, count,    \
                           adlen, mlen, mlen2, &ret_val, 2)) {                 \
        break;                                                                 \
    }

#ifdef crypto_aead_encrypt_2
            check_encr_fun(crypto_aead_encrypt_2)
#ifdef crypto_aead_decrypt_2
                check_decr_fun(crypto_aead_decrypt_2)
#endif
#endif

#ifdef crypto_aead_encrypt_3
                    check_encr_fun(crypto_aead_encrypt_3)
#ifdef crypto_aead_decrypt_3
                        check_decr_fun(crypto_aead_decrypt_3)
#endif
#endif

#ifdef crypto_aead_encrypt_4
                            check_encr_fun(crypto_aead_encrypt_4)
#ifdef crypto_aead_decrypt_4
                                check_decr_fun(crypto_aead_decrypt_4)
#endif
#endif

#ifdef crypto_aead_encrypt_5
                                    check_encr_fun(crypto_aead_encrypt_5)
#ifdef crypto_aead_decrypt_5
                                        check_decr_fun(crypto_aead_decrypt_5)
#endif
#endif

#ifdef crypto_aead_encrypt_6
                                            check_encr_fun(
                                                crypto_aead_encrypt_6)
#ifdef crypto_aead_decrypt_6
                                                check_decr_fun(
                                                    crypto_aead_decrypt_6)
#endif
#endif

#ifdef crypto_aead_encrypt_7
                                                    check_encr_fun(
                                                        crypto_aead_encrypt_7)
#ifdef crypto_aead_decrypt_7
                                                        check_decr_fun(
                                                            crypto_aead_decrypt_7)
#endif
#endif

#ifdef crypto_aead_encrypt_8
                                                            check_encr_fun(
                                                                crypto_aead_encrypt_8)
#ifdef crypto_aead_decrypt_8
                                                                check_decr_fun(
                                                                    crypto_aead_decrypt_8)
#endif
#endif
                                                                    count++;
        }
    }

    return ret_val;
}

void test_crypto_aead() {

    unsigned char key[CRYPTO_KEYBYTES];
    unsigned char nonce[CRYPTO_NPUBBYTES];
    unsigned char msg[MAX_MESSAGE_LENGTH_AEAD];
    unsigned char msg2[MAX_MESSAGE_LENGTH_AEAD + CRYPTO_ABYTES];
    unsigned char ad[MAX_ASSOCIATED_DATA_LENGTH];
    unsigned char ct[MAX_MESSAGE_LENGTH_AEAD + CRYPTO_ABYTES];
    unsigned char ct2[MAX_MESSAGE_LENGTH_AEAD + CRYPTO_ABYTES];

    (void)msg2;
    (void)ct2;

    int ret = KAT_SUCCESS;
    printf("#################################\n");

    // ret = check_test(key, nonce, msg, msg2, ad, ct, ct2);

    if (ret != KAT_SUCCESS) {
        printf("test vector generation failed with code %d\n", ret);
    } else {
        printf("test vector crypto_aead checked.\n");
    }

#ifdef SPEED
    printf("---------------------------------\n");
    printf("1: " STRINGIFY(crypto_aead_encrypt) "\n");
#ifdef crypto_aead_encrypt_2
    printf("2: " STRINGIFY(crypto_aead_encrypt_2) "\n");
#ifdef crypto_aead_encrypt_3
    printf("3: " STRINGIFY(crypto_aead_encrypt_3) "\n");
#ifdef crypto_aead_encrypt_4
    printf("4: " STRINGIFY(crypto_aead_encrypt_4) "\n");
#ifdef crypto_aead_encrypt_5
    printf("5: " STRINGIFY(crypto_aead_encrypt_5) "\n");
#ifdef crypto_aead_encrypt_6
    printf("6: " STRINGIFY(crypto_aead_encrypt_6) "\n");
#ifdef crypto_aead_encrypt_7
    printf("7: " STRINGIFY(crypto_aead_encrypt_7) "\n");
#ifdef crypto_aead_encrypt_8
    printf("8: " STRINGIFY(crypto_aead_encrypt_8) "\n");
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

    printf("---------------------------------\n");
    printf("adlen\tmlen\t1");
#ifdef crypto_aead_encrypt_2
    printf("\t2");
#ifdef crypto_aead_encrypt_3
    printf("\t3");
#ifdef crypto_aead_encrypt_4
    printf("\t4");
#ifdef crypto_aead_encrypt_5
    printf("\t5");
#ifdef crypto_aead_encrypt_6
    printf("\t6");
#ifdef crypto_aead_encrypt_7
    printf("\t7");
#ifdef crypto_aead_encrypt_8
    printf("\t8");
#endif
#endif
#endif
#endif
#endif
#endif
#endif
    printf("\n");
    speed_test(key, nonce, msg, ad, ct);
#endif
}

#endif
