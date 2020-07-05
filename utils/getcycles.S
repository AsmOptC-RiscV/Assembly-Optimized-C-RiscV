.text

.globl getcycles
.align 2
getcycles:
#ifdef sim
    csrr a0, cycle
#else
   csrr a1, mcycleh
   csrr a0, mcycle
   csrr a2, mcycleh
   bne a1, a2, getcycles    
#endif
    ret
