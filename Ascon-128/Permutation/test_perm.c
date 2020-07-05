#include "test_perm.h"

#define START 0x96

unsigned int __attribute__((noinline))
fn_test(void fn(state *s, int round_constant), state *x) {
    uint64_t oldcount, newcount;
    fn(x, START);
    oldcount = getcycles();
    fn(x, START);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

static void check_test(state *x) {
    *x = reset_state;
    ascon_ref(x, START);
    print_state(x);

    *x = reset_state;
    ascon_asm(x, START);
    print_state_32((uint32_t *)x);

    *x = reset_state;
    ascon_opt(x, START);
    print_state_32((uint32_t *)x);

    *x = reset_state;
    ascon_opt_2(x, START);
    print_state_32((uint32_t *)x);
}

static void speed_test(state *x) {
    int i;
    unsigned int time = 0;
    unsigned int time2;
    void *fn1 = &ascon_ref;
    void *fn2 = &ascon_asm;
    void *fn3 = &ascon_opt;
    void *fn4 = &ascon_opt_2;

    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn1, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("--------------------------\n");
    printf("ascon_ref took  %d cycles.\n", time);

    time = 0;
    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn2, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("--------------------------\n");
    printf("ascon_asm took  %d cycles.\n", time);

    time = 0;
    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn3, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("--------------------------\n");
    printf("ascon_opt took  %d cycles.\n", time);

    time = 0;
    for (i = 0; i < 20; i++) {
        time2 = fn_test(fn4, x);
        if (time == 0) {
            time = time2;
        } else {
            time = time2 < time ? time2 : time;
        }
    }
    printf("--------------------------\n");
    printf("ascon_opt_2 took  %d cycles.\n", time);
}

void test_perm() {
    state x;
    printf("##########################\n");

    check_test(&x);
    speed_test(&x);
}
