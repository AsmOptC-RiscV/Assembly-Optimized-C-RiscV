#include "test_perm.h"

unsigned int __attribute__((noinline))
fn_test1(void fn(const uint32_t *), uint32_t *x) {
    uint64_t oldcount, newcount;
    fn(x);
    oldcount = getcycles();
    fn(x);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

static void check_test() {

    uint32_t lanes[50] = {0};
    uint32_t lanes2[50] = {0};

    // Generate round keys
    // Ko code
    keccakf1600(lanes);
    keccakf1600_C(lanes2);

    // Print ciphertext
    // Print all round keys
    if (memcmp(lanes, lanes2, 50 * 4)) {
        printf("out:\n");
        for (unsigned int i = 0; i < 50; ++i) {
            printf("lanes[%2d] = %08x", i, (unsigned int)lanes[i]);
            printf("  lanes2[%2d] = %08x", i, (unsigned int)lanes2[i]);
            printf("\n");
        }
    } else {
        printf("Permutation is correct.\n");
    }
    printf("---------------------------\n");
}

static void speed_test() {
    uint32_t lanes[50] = {0};

    int i;
    unsigned int time1, time2;
    void *ks_1 = &keccakf1600;
    void *ks_2 = &keccakf1600_C;

    for (i = 0; i < 20; i++)
        time1 = fn_test1(ks_1, lanes);
    for (i = 0; i < 20; i++)
        time2 = fn_test1(ks_2, lanes);
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
