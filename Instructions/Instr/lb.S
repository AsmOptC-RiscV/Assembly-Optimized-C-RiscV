.globl instr_lb
.type instr_lb, %function
.align 3
instr_lb:
    lb a1, 0(a0);
    lb a1, 1(a0);
    lb a1, 2(a0);
    lb a1, 3(a0);
    lb a1, 4(a0);
    lb a1, 5(a0);
    lb a1, 6(a0);
    lb a1, 7(a0);
    lb a1, 8(a0);
    lb a1, 9(a0);
    ret
.size instr_lb,.-instr_lb