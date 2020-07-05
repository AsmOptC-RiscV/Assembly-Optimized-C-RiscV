.globl instr_lw
.type instr_lw, %function
.align 3
instr_lw:
    lw a1, 0(a0);
    lw a1, 4(a0);
    lw a1, 8(a0);
    lw a1, 12(a0);
    lw a1, 16(a0);
    lw a1, 20(a0);
    lw a1, 24(a0);
    lw a1, 28(a0);
    lw a1, 32(a0);
    lw a1, 36(a0);
    ret
.size instr_lw,.-instr_lw