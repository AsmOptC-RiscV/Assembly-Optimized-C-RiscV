.globl instr_xori
.type instr_xori, %function
.align 3
instr_xori:
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    xori a1, a2, -1;
    ret
.size instr_xori,.-instr_xori