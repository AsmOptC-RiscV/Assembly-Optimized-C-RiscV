#include "test_perm.h"

#define NUM_BLOCKS 128 // would test 256 but Ko's code crashes :p
#define INPUT_LENGTH (NUM_BLOCKS * 16)
#define OUTPUT_LENGTH (((INPUT_LENGTH + 32) / 32) * 32)

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

unsigned int __attribute__((noinline))
fn_test4(void fn(param const *, const uint8_t *, uint8_t *, uint32_t),
         param const *x, const uint8_t *y, uint8_t *z, uint32_t t) {
    uint64_t oldcount, newcount;
    fn(x, y, z, t);
    oldcount = getcycles();
    fn(x, y, z, t);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

// we need this in order to compute ctr for the Ref code
inline static void __attribute__((always_inline)) revbytes(uint32_t *b0) {
    // original is more complex but this order makes more sense.
    // may need optimizations.
    uint32_t r0 = *b0 & 0xff;
    *b0 = *b0 >> 8;
    uint32_t r1 = *b0 & 0xff;
    *b0 = *b0 >> 8;
    uint32_t r2 = *b0 & 0xff;
    *b0 = *b0 >> 8;
    r0 = r0 << 8;
    r0 = (r0 | r1) << 8;
    r0 = (r0 | r2) << 8;
    *b0 = *b0 | r0;
}

static void check_test() {

    uint8_t nonce[12] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
                         0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb};
    uint8_t ctr[4] = {0xfc, 0xfd, 0xfe, 0xff};
    uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                       0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t in[INPUT_LENGTH] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a, 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 0x30, 0xc8, 0x1c, 0x46,
        0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef};
    uint8_t out[OUTPUT_LENGTH] = {0};
    uint8_t out2[OUTPUT_LENGTH] = {0};
    uint8_t out3[OUTPUT_LENGTH] = {0};
    param p, p2, p3;

    int i, j;

    // Generate round keys
    // Ko code
    memcpy(p.ctr, ctr, 4);
    memcpy(p.nonce, nonce, 12);
    AES_128_keyschedule(key, p.rk);

    memcpy(p2.ctr, ctr, 4);
    memcpy(p2.nonce, nonce, 12);
    AES_128_keyschedule_C(key, p2.rk);

    // ref code
    uint8_t *out3_temp = out3;
    uint8_t *in_temp = in;
    j = 0, i = 0;
    while (i < INPUT_LENGTH) {
        memcpy(p3.ctr, ctr, 4);

        revbytes((uint32_t *)p3.ctr);
        *(uint32_t *)p3.ctr += j;
        revbytes((uint32_t *)p3.ctr);
        memcpy(p3.nonce, nonce, 12);
        int nround = rijndaelKeySetupEnc((u32 *)p3.rk, key, 128);
        rijndaelEncrypt((u32 *)p3.rk, nround, p3.nonce, out3_temp);
        for (int k = 0; k < 16; k++) {
            *out3_temp ^= *in_temp;
            out3_temp += 1;
            in_temp += 1;
        }
        i += 16;
        j += 1;
        printf("\n");
    }

    // Print all round keys
    if (memcmp(p.rk, p2.rk, 2 * 11 * 16)) {
        for (i = 0; i < 2 * 11 * 4; ++i) {
            printf("p.rk[%2d]: ", i);
            for (j = 0; j < 4; ++j) {
                printf("%02x", p.rk[i * 4 + j]);
            }
            printf("  p2.rk[%2d]: ", i);
            for (j = 0; j < 4; ++j) {
                printf("%02x", p2.rk[i * 4 + j]);
            }
            printf("\n");
        }
    } else {
        printf("Round keys are the correct.\n");
    }
    printf("---------------------------\n");

    AES_128_encrypt_ctr(&p, in, out, INPUT_LENGTH);
    AES_128_encrypt_ctr_C(&p2, in, out2, INPUT_LENGTH);

    if (memcmp(out, out2, INPUT_LENGTH)) {
        printf("In:        ");
        for (i = 0; i < INPUT_LENGTH; ++i) {
            printf("%02x", in[i]);
        }
        printf("\n");
        printf("ASM out:   ");
        for (i = 0; i < INPUT_LENGTH; ++i) {
            printf("%02x", out[i]);
            if ((i & 3) == 3) {
                printf(" ");
            }
        }
        printf("\n");
        printf("C-Opt out: ");
        for (i = 0; i < INPUT_LENGTH; ++i) {
            printf("%02x", out2[i]);
            if ((i & 3) == 3) {
                printf(" ");
            }
        }
        printf("\n");
        printf("Ref   out: ");
        for (i = 0; i < INPUT_LENGTH; ++i) {
            printf("%02x", out3[i]);
            if ((i & 3) == 3) {
                printf(" ");
            }
        }
        printf("\n");
    } else {
        printf("Encryptions are the correct.\n");
    }
}

static void speed_test() {
    uint8_t nonce[12] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
                         0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb};
    uint8_t ctr[4] = {0xfc, 0xfd, 0xfe, 0xff};
    uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                       0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t in[INPUT_LENGTH] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a, 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 0x30, 0xc8, 0x1c, 0x46,
        0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef};
    uint8_t out[OUTPUT_LENGTH];
    param p, p2;

    int i;

    // Generate round keys
    // Ko code
    memcpy(p.ctr, ctr, 4);
    memcpy(p.nonce, nonce, 12);
    memcpy(p2.ctr, ctr, 4);
    memcpy(p2.nonce, nonce, 12);

    unsigned int time1, time2;
    void *ks_1 = &AES_128_keyschedule;
    void *ks_2 = &AES_128_keyschedule_C;

    for (i = 0; i < 20; i++)
        time1 = fn_test2(ks_1, key, p.rk);
    for (i = 0; i < 20; i++)
        time2 = fn_test2(ks_2, key, p2.rk);
    printf("---------------------------\n");
    printf("ASM         took %d cycles.\n", time1);
    printf("---------------------------\n");
    printf("Optimized C took %d cycles.\n", time2);

    // Print all round keys (to avoid optimization at compile time)

    void *enc_1 = &AES_128_encrypt_ctr;
    void *enc_2 = &AES_128_encrypt_ctr_C;

    for (i = 0; i < 30; i++)
        time1 = fn_test4(enc_1, &p, in, out, INPUT_LENGTH);

    for (i = 0; i < 30; i++)
        time2 = fn_test4(enc_2, &p, in, out, INPUT_LENGTH);

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
