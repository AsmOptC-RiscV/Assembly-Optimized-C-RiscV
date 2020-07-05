.globl instr_sb
.type instr_sb, %function
.align 3
instr_sb:
    sb zero, 0(a0);
    sb zero, 1(a0);
    sb zero, 2(a0);
    sb zero, 3(a0);
    sb zero, 4(a0);
    sb zero, 5(a0);
    sb zero, 6(a0);
    sb zero, 7(a0);
    sb zero, 8(a0);
    sb zero, 9(a0);
    ret
.size instr_sb,.-instr_sb