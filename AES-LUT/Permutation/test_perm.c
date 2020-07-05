#include "test_perm.h"

unsigned int __attribute__((noinline))
fn_test2(void fn(const uint8_t *, uint8_t *), const uint8_t *x, uint8_t *y) {
    uint64_t oldcount, newcount;
    fn(x, y);
    oldcount = getcycles();
    fn(x, y);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

unsigned int __attribute__((noinline))
fn_test3(void fn(const uint8_t *, const uint8_t *, uint8_t *), const uint8_t *x,
         const uint8_t *y, uint8_t *z) {
    uint64_t oldcount, newcount;
    fn(x, y, z);
    oldcount = getcycles();
    fn(x, y, z);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

static void check_test() {

    const uint8_t key[16] = {4, 5, 6, 7, 4, 5, 6, 8, 4, 5, 6, 9, 4, 5, 6, 10};
    uint8_t in[16] = {0, 0, 0, 0, 1, 2, 3, 1, 2, 4, 1, 2, 5, 1, 2, 6};
    uint8_t out[16] = {0};
    int i, j;

    uint8_t rk[11 * 16] = {0}, rk2[11 * 16] = {0}, rk3[11 * 16] = {0};

    // Generate round keys
    // Ko code
    memcpy(rk, key, 16);
    AES_128_keyschedule(key, rk + 16);
    // optimized C
    memcpy(rk2, key, 16);
    AES_128_keyschedule_C(key, rk2 + 16);
    // ref code
    memcpy(rk3, key, 16);
    int nround = rijndaelKeySetupEnc((u32 *)rk3, key, 128);

    // Print all round keys
    int same = 1;
    for (i = 0; i < 11 * 4; ++i) {
        for (j = 0; j < 4; ++j) {
            same &= (rk3[i * 4 + (3 - j)] == rk[i * 4 + j]);
            same &= (rk3[i * 4 + (3 - j)] == rk2[i * 4 + j]);
        }
    }
    if (!same) {
        for (i = 0; i < 11 * 4; ++i) {
            printf("rk[%2d]: ", i);
            for (j = 0; j < 4; ++j) {
                printf("%02x", rk[i * 4 + j]);
            }
            printf("  rk2[%2d]: ", i);
            for (j = 0; j < 4; ++j) {
                printf("%02x", rk2[i * 4 + j]);
            }
            printf("  rk3[%2d]: ", i);
            for (j = 3; j >= 0; --j) {
                printf("%02x", rk3[i * 4 + j]);
            }
            printf("\n");
            if ((i & 3) == 3) {
                printf("\n");
            }
        }
    } else {
        printf("Round keys are the correct.\n");
    }

    printf("---------------------------\n");

    // encrypt plaintext - Ko code
    AES_128_encrypt(rk, in, out);
    // Print ciphertext
    printf("out ASM: ");
    for (i = 0; i < 16; ++i) {
        printf("%02x", out[i]);
        if ((i & 3) == 3)
            printf(" ");
    }
    printf("\n");

    // clear output just to be sure
    for (i = 0; i < 16; i++) {
        out[i] = 0;
    }

    // encrypt plaintext - Optimized C
    AES_128_encrypt_C(rk2, in, out);
    // Print ciphertext
    printf("out C:   ");
    for (i = 0; i < 16; ++i) {
        printf("%02x", out[i]);
        if ((i & 3) == 3)
            printf(" ");
    }
    printf("\n");

    // clear output just to be sure
    for (i = 0; i < 16; i++) {
        out[i] = 0;
    }

    // encrypt plaintext - Reference C
    rijndaelEncrypt((u32 *)rk3, nround, in, out);
    // Print ciphertext
    printf("out Ref: ");
    for (i = 0; i < 16; ++i) {
        printf("%02x", out[i]);
        if ((i & 3) == 3)
            printf(" ");
    }
    printf("\n");
}

static void speed_test() {
    const uint8_t key[16] = {4, 5, 6, 7, 4, 5, 6, 8, 4, 5, 6, 9, 4, 5, 6, 10};
    uint8_t in[16] = {0, 0, 0, 0, 1, 2, 3, 1, 2, 4, 1, 2, 5, 1, 2, 6};
    uint8_t out[16];

    uint8_t rk[11 * 16] = {0};

    memcpy(rk, key, 16);

    int i;
    unsigned int time1, time2, time3;
    void *ks_1 = &AES_128_keyschedule;
    void *ks_2 = &AES_128_keyschedule_C;

    for (i = 0; i < 40; i++)
        time1 = fn_test2(ks_1, key, rk + 16);
    for (i = 0; i < 40; i++)
        time2 = fn_test2(ks_2, key, rk + 16);
    printf("---------------------------\n");
    printf("ASM         took %d cycles.\n", time1);
    printf("---------------------------\n");
    printf("Optimized C took %d cycles.\n", time2);
    printf("######################\n");

    // Print all round keys (to avoid optimization at compile time)

    void *enc_1 = &AES_128_encrypt;
    void *enc_2 = &AES_128_encrypt_C;

    for (i = 0; i < 40; i++)
        time1 = fn_test3(enc_1, rk, in, out);

    // printf("out: ");
    // for (i = 0; i < 16; ++i) {
    //     printf("%02x", out[i]);
    // }
    // printf("\n");

    for (i = 0; i < 40; i++)
        time2 = fn_test3(enc_2, rk, in, out);

    printf("---------------------------\n");
    printf("ASM         took %d cycles.\n", time1);
    printf("---------------------------\n");
    printf("Optimized C took %d cycles.\n", time2);
}

void test_perm() {
    printf("######################\n");
    check_test();
    printf("######################\n");
    speed_test();
}
