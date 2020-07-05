.text

.include "Gimli/Permutation/gimli.macros.s"

.globl gimli_asm
.type gimli_asm, %function
.align 3
gimli_asm:
  # a0: p
  # t0-t6: tmp
  # s0-s11: state
  addi    sp, sp, -20
  sw      s8, 16(sp)
  sw      s9, 12(sp)
  sw     s10,  8(sp)
  sw     s11,  4(sp)

.align 2
# load state
    lw      a1, 0(a0) #0
    lw      a2, 4(a0) #1
    lw      a3, 8(a0) #2
    lw      a4, 12(a0) #3
    lw      a5, 16(a0) #4
    lw      a6, 20(a0) #5
    lw      a7, 24(a0) #6
    lw      t0, 28(a0) #7
    lw      t1, 32(a0) #8
    lw      t2, 36(a0) #9
    lw      t3, 40(a0) #10
    lw      t4, 44(a0) #11
    # Note the ABI: x will be in a0 and the return value is expected in a0.
  # asm_gimli x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3, t0, t1, cst, round, c1, c2
    asm_gimli a1, a2, a3, a4, a5, a6, a7, t0, t1, t2, t3, t4, t5, t6,  s8,   s9, s10, s11

.align 2
    sw      a1, 0(a0) #0
    sw      a2, 4(a0) #1
    sw      a3, 8(a0) #2
    sw      a4, 12(a0) #3
    sw      a5, 16(a0) #4
    sw      a6, 20(a0) #5
    sw      a7, 24(a0) #6
    sw      t0, 28(a0) #7
    sw      t1, 32(a0) #8
    sw      t2, 36(a0) #9
    sw      t3, 40(a0) #10
    sw      t4, 44(a0) #11

    lw      s8, 16(sp)
    lw      s9, 12(sp)
    lw     s10,  8(sp)
    lw     s11,  4(sp)

    addi    sp, sp, 20

    ret
.size gimli_asm,.-gimli_asm
