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

static void check_test(uint32_t *x) {
    reset_state(x);
    print_state(x);
    gimli(x);
    print_state(x);

    reset_state(x);
    gimli_asm(x);
    print_state(x);

    reset_state(x);
    gimli_c_opt(x);
    print_state(x);

    reset_state(x);
    gimli_c_opt2(x);
    print_state(x);
}

static void speed_test(uint32_t *x) {
    int i;
    unsigned int time = 0;
    unsigned int time2;
    void *fn1 = &gimli;
    void *fn2 = &gimli_asm;
    void *fn3 = &gimli_c_opt;
    void *fn4 = &gimli_c_opt2;

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
    printf(".S    took  %d cycles.\n", time);

    time = 0;
    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn3, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("----------------------\n");
    printf("C-opt took  %d cycles.\n", time);

    time = 0;
    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn4, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("----------------------\n");
    printf("C-opt2 took %d cycles.\n", time);
}

void test_perm() {
    uint32_t x[12];
    printf("######################\n");

    check_test(x);
    speed_test(x);
}
