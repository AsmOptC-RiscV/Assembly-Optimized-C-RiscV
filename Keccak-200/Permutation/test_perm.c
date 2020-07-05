#include "test_perm.h"

unsigned int __attribute__((noinline))
fn_test(void fn(uint32_t *state), uint32_t *x) {
    uint64_t oldcount, newcount;
    fn(x);
    oldcount = getcycles();
    fn(x);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

unsigned int __attribute__((noinline))
fn_test2(void fn(uint8_t *state), uint8_t *x) {
    uint64_t oldcount, newcount;
    fn(x);
    oldcount = getcycles();
    fn(x);
    fn(x + 25);
    fn(x + 50);
    fn(x + 75);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

static void interleave(uint32_t *out, const uint32_t *in) {
    uint8_t *i8 = (uint8_t *)in;
    interleave_4_blocks(out, i8, i8 + 25, i8 + 50, i8 + 75, 25);
}

static void deinterleave(uint32_t *out, const uint32_t *in) {
    uint8_t *i8 = (uint8_t *)out;
    deinterleave_uint32_to_blocks(i8, i8 + 25, i8 + 50, i8 + 75, in, 25);
}

static void check_test(uint32_t *x) {

    uint32_t y[25];
    printf("======================\n");
    printf("=====INTERLEAVING=====\n");
    printf("======================\n");
    reset_state(x);
    print_state(x);
    interleave(y, x);
    print_state(y);
    wipe_state(x);
    deinterleave(x, y);
    print_state(x);

    printf("======================\n");
    printf("======KECCAK-200======\n");
    printf("======================\n");
    reset_state(x);
    print_state(x);
    permutation((uint8_t *)x + 0);
    permutation(((uint8_t *)x) + 25);
    permutation(((uint8_t *)x) + 50);
    permutation(((uint8_t *)x) + 75);
    interleave(y, x);
    print_state(y);
    print_state(x);

    printf("======================\n");
    printf("=====KECCAK-200x4=====\n");
    printf("======================\n");
    reset_state(x);
    print_state(x);
    interleave(y, x);
    permutation_4(y);
    print_state(y);
    wipe_state(x);
    deinterleave(x, y);
    print_state(x);

    printf("======================\n");
    printf("===KECCAK-200x4-opt===\n");
    printf("======================\n");
    reset_state(x);
    print_state(x);
    interleave(y, x);
    permutation_4_c_opt(y);
    print_state(y);
    wipe_state(x);
    deinterleave(x, y);
    print_state(x);
}

static void speed_test(uint32_t *x) {
    int i;
    unsigned int time = 0;
    unsigned int time2;
    void *fn0 = &permutation;
    void *fn1 = &permutation_4;
    void *fn2 = &permutation_4_c_opt;

    for (i = 0; i < 20; i++) {
        time2 = fn_test2(fn0, (uint8_t *)x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("----------------------\n");
    printf("4xRef took  %d cycles.\n", time);

    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn1, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("----------------------\n");
    printf("C-ref took  %d cycles.\n", time);

    time = 0;
    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn2, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("----------------------\n");
    printf("C-opt took  %d cycles.\n", time);
}

void test_perm() {
    uint32_t x[25];
    printf("######################\n");

    check_test(x);
    speed_test(x);
}
