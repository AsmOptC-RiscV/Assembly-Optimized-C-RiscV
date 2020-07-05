#include "test_perm.h"

unsigned int __attribute__((noinline)) fn_test(void fn(uint8_t *), uint8_t *x) {
    uint64_t oldcount, newcount;
    fn(x);
    oldcount = getcycles();
    fn(x);
    newcount = getcycles();
    return (unsigned int)(newcount - oldcount);
}

#define stringify(x) #x
#define run_test(funname)                                                      \
    fn = &funname;                                                             \
    for (i = 0; i < 20; i++)                                                   \
        time1 = fn_test(fn, var);                                              \
    printf("---------------------------\n");                                   \
    printf("%d cycles: " stringify(funname) "\n", time1);

static void speed_test() {
    uint8_t var[10 * 4] = {0};

    int i;
    unsigned int time1;
    void *fn;

    run_test(instr_empty);
    printf("---------------------------\n");
    printf("\n");
    run_test(instr_lb);
    run_test(instr_sb);
    run_test(instr_lw);
    run_test(instr_sw);
    printf("---------------------------\n");
    printf("\n");
    run_test(instr_addi);
    run_test(instr_add);
    run_test(instr_andi);
    run_test(instr_and);
    run_test(instr_ori);
    run_test(instr_or);
    run_test(instr_slli);
    run_test(instr_sll);
    run_test(instr_srai);
    run_test(instr_sra);
    run_test(instr_srli);
    run_test(instr_srl);
    run_test(instr_sub);
    run_test(instr_xori);
    run_test(instr_xor);
    printf("---------------------------\n");
}

void test_perm() {
    printf("######################\n");
    speed_test();
}
