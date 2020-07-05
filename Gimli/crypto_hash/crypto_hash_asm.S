.text

.include "Gimli/Permutation/gimli.macros.s"

.globl crypto_hash_asm
.type crypto_hash_asm, %function
.align 3
crypto_hash_asm:
  # a0: unsigned char *out,
  # a1: const unsigned char *in,
  # a2: unsigned long long inlen : Length in BYTES !
  # a3: actually contains 00000000

  # state = ["s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"]
  # actually we only use t0-t6 and a3

  # Sorry we do not support hashing more than 4GB of data
  bne     a3, zero, err

  addi    sp, sp, -56
  sw      a3, 52(sp)
  sw      s0, 48(sp)
  sw      s1, 44(sp)
  sw      s2, 40(sp)
  sw      s3, 36(sp)
  sw      s4, 32(sp)
  sw      s5, 28(sp)
  sw      s6, 24(sp)
  sw      s7, 20(sp)
  sw      s8, 16(sp)
  sw      s9, 12(sp)
  sw     s10,  8(sp)
  sw     s11,  4(sp)

.align 2
  # Note the ABI: x will be in a0 and the return value is expected in a0.
  # create empty state
  addi    s0,  zero, 0
  addi    s1,  zero, 0
  addi    s2,  zero, 0
  addi    s3,  zero, 0
  addi    s4,  zero, 0
  addi    s5,  zero, 0
  addi    s6,  zero, 0
  addi    s7,  zero, 0
  addi    s8,  zero, 0
  addi    s9,  zero, 0
  addi    s10, zero, 0
  addi    s11, zero, 0

absorb:
  # alignement is on 32 bytes.
  # temp used in absorb:
  # t0, t1, t2, t3, t4
  # t6 is a marker for squeeze DO NOT TOUCH
  # it gets decreased after each gimli iteration
  # we reset it after each full absorption
  # 2 = absorb,
  # 1 = last absorb,
  # 0 = first squeeze
  # -1 = last squeeze

  # check if remaining is more than 128 bits = 16 bytes
  # addi t6, zero, 2
  addi t6, a2, -16
  bgez t6, 128f
  # since we are not looping anymore for absorption we can xor 1 in into byte 47
  addi    t3, zero, 1
  slli    t3, t3, 24
  xor     s11, s11, t3
  # check if remaining is more than 96 bits = 12 bytes
  addi t6, a2, -12
  bgez t6, 96f
  # check if remaining is more than 64 bits = 8 bytes
  addi t6, a2, -8
  bgez t6, 64f
  # check if remaining is more than 32 bits = 4 bytes
  addi t6, a2, -4
  bgez t6, 32f
# 0: <=32 bits
  lw      t0, 0(a1)
  # pad   to, in, len, tmp, tmp, tmp
  pad     s0, t0, a2,  t5,  t3,  t4
  addi t6, zero, 1
  j perm
32:  # >= 32 && <64 bits
  lw      t0, 0(a1)
  xor     s0, s0, t0
  lw      t1, 4(a1)
  addi    a2, a2, -4
  # pad   to, in, len, tmp, tmp, tmp
  pad     s1, t1, a2,  t5,  t3,  t4
  addi t6, zero, 1
  j perm
64:  # >= 64 && <96 bits
  lw      t0, 0(a1)
  xor     s0, s0, t0
  lw      t1, 4(a1)
  xor     s1, s1, t1
  lw      t2, 8(a1)
  addi    a2, a2, -8
  # pad   to, in, len, tmp, tmp, tmp
  pad     s2, t2, a2,  t5,  t3,  t4
  addi t6, zero, 1
  j perm
96:  # >= 96 &$ <128 bits
  lw      t0, 0(a1)
  xor     s0, s0, t0
  lw      t1, 4(a1)
  xor     s1, s1, t1
  lw      t2, 8(a1)
  xor     s2, s2, t2
  lw      t3, 12(a1)
  addi    a2, a2, -12
  # pad   to, in, len, tmp, tmp, tmp
  pad    s3, t3, a2,  t5,  t2,  t4
  addi t6, zero, 1
  j perm
128:  # >= 128 bits
  lw      t0, 0(a1)
  xor     s0, s0, t0
  lw      t1, 4(a1)
  xor     s1, s1, t1
  lw      t2, 8(a1)
  xor     s2, s2, t2
  lw      t3, 12(a1)
  xor     s3, s3, t3

  addi    a1, a1, 16
  addi    a2, a2, -16
  addi    t6, zero, 2
perm:
  # apply permutation
# asm_gimli x0, x1, x2, x3, y0, y1, y2, y3, z0, z1,  z2, z3,   t0, t1, cst, round, c1, c2
  asm_gimli s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,  t0, t1, t2, t3,    t4, t5

  addi t6, t6, -1
  bgtz t6, absorb

squeeze:
.align 2
  sw      s0, 0(a0) #0
  sw      s1, 4(a0) #1
  sw      s2, 8(a0) #2
  sw      s3, 12(a0) #3

  addi    a0, a0, 16
  beqz    t6, perm

  lw      a3, 52(sp)
  lw      s0, 48(sp)
  lw      s1, 44(sp)
  lw      s2, 40(sp)
  lw      s3, 36(sp)
  lw      s4, 32(sp)
  lw      s5, 28(sp)
  lw      s6, 24(sp)
  lw      s7, 20(sp)
  lw      s8, 16(sp)
  lw      s9, 12(sp)
  lw     s10,  8(sp)
  lw     s11,  4(sp)

  addi    sp, sp, 56
  addi    a0, zero, 0
  ret

err:
  add     a0, zero, 1
  ret
.size crypto_hash_asm,.-crypto_hash_asm
