#include "test_hash.h"
#ifdef crypto_hash

#define SPEED
#define INCR_HASH <<= 1
// #define INCR_HASH += 1

#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

unsigned int __attribute__((noinline))
fn_hash_test(int *fn(), unsigned char *digest, const unsigned char *msg,
             unsigned long long mlen) {
    uint64_t oldcount, newcount;
    fn(digest, msg, mlen);
    oldcount = getcycles();
    oldcount = getcycles();
    fn(digest, msg, mlen);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

static void speed_test(unsigned char *digest, const unsigned char *msg,
                       unsigned long long mlen) {
    int i;
    unsigned int time;

    void *fn1 = &crypto_hash;
#ifdef crypto_hash_2
    void *fn2 = &crypto_hash_2;
#ifdef crypto_hash_3
    void *fn3 = &crypto_hash_3;
#ifdef crypto_hash_4
    void *fn4 = &crypto_hash_4;
#ifdef crypto_hash_5
    void *fn5 = &crypto_hash_5;
#ifdef crypto_hash_6
    void *fn6 = &crypto_hash_6;
#endif
#endif
#endif
#endif
#endif

    printf("%d\t\t", (unsigned int)mlen);

#define loop_speed(fnvar)                                                      \
    for (i = 0; i < 10; i++)                                                   \
        time = fn_hash_test(fnvar, digest, msg, mlen);                         \
    printf("%d\t", time);

    loop_speed(fn1)

#ifdef crypto_hash_2
        loop_speed(fn2)
#ifdef crypto_hash_3
            loop_speed(fn3)
#ifdef crypto_hash_4
                loop_speed(fn4)
#ifdef crypto_hash_5
                    loop_speed(fn5)
#ifdef crypto_hash_6
                        loop_speed(fn6)
#endif
#endif
#endif
#endif
#endif
                            printf("\n");
}

static void print_message(const unsigned char *msg, unsigned long long mlen,
                          int count) {
    printf("Count = %d\n", count);
    print_bstr("Msg = ", msg, mlen);
}

static void print_hash(unsigned char *digest, int count) {
    printf("%d MD = ", count);
    for (unsigned long long i = 0; i < CRYPTO_BYTES; i++)
        printf("%02X", digest[i]);
    printf("\n");
}

static int check_test(unsigned char *digest, unsigned char *digest2,
                      const unsigned char *msg, unsigned long long mlen,
                      int count) {

    init_buffer(digest, CRYPTO_BYTES);
    crypto_hash(digest, msg, mlen);

#define hash_test(fnvar)                                                       \
    init_buffer(digest2, CRYPTO_BYTES);                                        \
    fnvar(digest2, msg, mlen);                                                 \
    if (memcmp(digest, digest2, CRYPTO_BYTES)) {                               \
        printf("\n");                                                          \
        printf(STRINGIFY(fnvar) " did not match\n");                           \
        print_hash(digest, count);                                             \
        print_hash(digest2, count);                                            \
        return KAT_CRYPTO_FAILURE;                                             \
    }

#ifndef crypto_hash_2
    print_hash(digest, count);
#else
    hash_test(crypto_hash_2);
#endif
#ifdef crypto_hash_3
    hash_test(crypto_hash_3);
#endif
#ifdef crypto_hash_4
    hash_test(crypto_hash_4);
#endif
#ifdef crypto_hash_5
    hash_test(crypto_hash_5);
#endif
#ifdef crypto_hash_6
    hash_test(crypto_hash_6);
#endif

    return KAT_SUCCESS;
}

void test_crypto_hash() {
    unsigned char msg[MAX_MESSAGE_LENGTH_HASH];
    unsigned char digest[CRYPTO_BYTES];
#ifndef NO_CHECK
    int ret = 0;
    int count = 1;
    unsigned char digest2[CRYPTO_BYTES];
#endif

    printf("#################################\n");
    init_buffer(msg, MAX_MESSAGE_LENGTH_HASH);

#ifndef NO_CHECK
    for (unsigned long long mlen = 0;
         mlen <= MAX_MESSAGE_LENGTH_HASH && ret == 0; mlen++) {
        ret = check_test(digest, digest2, msg, mlen, count++);
    }

    if (ret != KAT_SUCCESS) {
        printf("test vector generation failed with code %d\n", ret);
    } else {
        printf("test vector crypto_hash checked.\n");
    }
#endif

#ifdef SPEED
    printf("---------------------------------\n");
    printf("1: " STRINGIFY(crypto_hash) "\n");
#ifdef crypto_hash_2
    printf("2: " STRINGIFY(crypto_hash_2) "\n");
#ifdef crypto_hash_3
    printf("3: " STRINGIFY(crypto_hash_3) "\n");
#ifdef crypto_hash_4
    printf("4: " STRINGIFY(crypto_hash_4) "\n");
#ifdef crypto_hash_5
    printf("5: " STRINGIFY(crypto_hash_5) "\n");
#ifdef crypto_hash_6
    printf("6: " STRINGIFY(crypto_hash_6) "\n");
#endif
#endif
#endif
#endif
#endif

    printf("---------------------------------\n");
    printf("length (Bytes) \t1");
#ifdef crypto_hash_2
    printf("\t2");
#ifdef crypto_hash_3
    printf("\t3");
#ifdef crypto_hash_4
    printf("\t4");
#ifdef crypto_hash_5
    printf("\t5");
#ifdef crypto_hash_6
    printf("\t6");
#endif
#endif
#endif
#endif
#endif
    printf("\n");
    speed_test(digest, msg, 0);
    for (unsigned long long mlen = 1; mlen <= MAX_MESSAGE_LENGTH_HASH;
         mlen INCR_HASH) {
        speed_test(digest, msg, mlen);
    }
#endif
}

#endif
