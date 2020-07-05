.globl instr_sw
.type instr_sw, %function
.align 3
instr_sw:
    sw zero, 0(a0);
    sw zero, 4(a0);
    sw zero, 8(a0);
    sw zero, 12(a0);
    sw zero, 16(a0);
    sw zero, 20(a0);
    sw zero, 24(a0);
    sw zero, 28(a0);
    sw zero, 32(a0);
    sw zero, 36(a0);
    ret
.size instr_sw,.-instr_sw