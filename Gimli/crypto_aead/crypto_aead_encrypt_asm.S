.text

.include "Gimli/Permutation/gimli.macros.s"

.globl crypto_aead_encrypt_asm
.type crypto_aead_encrypt_asm, %function
.align 3
crypto_aead_encrypt_asm:
  # 0(a0): unsigned char *c,
  # 0(a1): unsigned long long *clen, : Length in BYTES ! (mlen + 16)
  # 0(a2): const unsigned char *m,
  # a3: unsigned long long mlen,  : Length in BYTES !
  # a4: upper word of mlen.
  # 0(a5): const unsigned char *ad,
  # a6: unsigned long long adlen, : Length in BYTES !
  # a7: upper word of adlen.
  # 0(sp): const unsigned char *nsec,    // NOT USED
  # 4(sp): const unsigned char *npub,    // Nounce = 16 bytes => 0 4 8 12
  # 8(sp): const unsigned char *k        // Key = 32 bytes    => 0 4 8 12 16 20 24 28

  # Sorry we do not support more than 4GB of message
  bne    a4, zero, err
  # Sorry we do not support more than 4GB of data
  bne    a7, zero, err

  add   a4, zero, zero

  # clen = mlen + 16
  addi   t0, a3, 16
  sw     t0, 0(a1)

.align 2
  # Note the ABI: x will be in a0 and the return value is expected in a0.
  addi   t0, sp, 4     # save the pointer location in order to be able to load key and nounce
  addi   sp, sp, -64   # 20 * 4 + 16
  # sw     a0, 92(sp)  # just to be safe: *c
  # sw     a1, 88(sp)  # just to be safe: *clen
  # sw     a2, 84(sp)  # just to be safe: *m
  # sw     a3, 80(sp)  # just to be safe: lower mlen
  # sw     a4, 76(sp)  # just to be safe: upper mlem
  # sw     a5, 72(sp)  # just to be safe: *ad
  # sw     a6, 68(sp)  # just to be safe: lower adlen
  # sw     a7, 64(sp)  # just to be safe: upper adlen
  sw     s0, 60(sp)
  sw     s1, 56(sp)
  sw     s2, 52(sp)
  sw     s3, 48(sp)
  sw     s4, 44(sp)
  sw     s5, 40(sp)
  sw     s6, 36(sp)
  sw     s7, 32(sp)
  sw     s8, 28(sp)
  sw     s9, 24(sp)
  sw    s10, 20(sp)
  sw    s11, 16(sp)

  # first we parse the additional data
  add a1, zero, a5
  # a6 is the decreasing counter of length = -1 we switch to the next part.
  # Needs to take the value of mlen once we processed the additional data.

  # Load Nounce address
  lw     t1, 0(t0)
  # Load Nounce
  lw     s0, 0(t1)
  lw     s1, 4(t1)
  lw     s2, 8(t1)
  lw     s3, 12(t1)
  # Load Key address
  lw     t1, 4(t0)
  # Load Key
  lw     s4, 0(t1)
  lw     s5, 4(t1)
  lw     s6, 8(t1)
  lw     s7, 12(t1)
  lw     s8, 16(t1)
  lw     s9, 20(t1)
  lw    s10, 24(t1)
  lw    s11, 28(t1)

  # we need to apply the permutation first.
  j perm

absorb:
  # alignement is on 32 bytes.

  # state = ["s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"]
  # tmp = ["t0", "t1", "t2", "t3", "t4", "t5", "t6"]

  # check if remaining is more than 128 bits = 16 bytes
  addi   t5, a6, -16
  bgez   t5, 128f

  # since we are not looping anymore for absorption we can xor 1 in into byte 47
  addi   t3, zero, 1
  slli   t3, t3, 24
  xor    s11, s11, t3

  # check if remaining is more than 96 bits = 12 bytes
  addi   t5, a6, -12
  bgez   t5, 96f
  # check if remaining is more than 64 bits = 8 bytes
  addi   t5, a6, -8
  bgez   t5, 64f
  # check if remaining is more than 32 bits = 4 bytes
  addi   t5, a6, -4
  bgez   t5, 32f

# 0: <=32 bits
  lw     t0, 0(a1)
  # mv t0, zero
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s0, t0, a6,  t5,  t3,  t4, t2
  add    a7, zero, a6     # we save the value of a6 into a7 to know the number of bytes to sqeeze
  addi   a6, zero, -1     # we set a6 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a2, perm
  xor    t0, t0, t2
  sw     t0, 0(a0)
  addi   a0, a0, 4
  j perm
32:  # >= 32 && <64 bits
  lw     t0, 0(a1)
  xor    s0, s0, t0
  lw     t1, 4(a1)
  # mv t1, zero
  addi   a6, a6, -4
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s1, t1, a6,  t5,  t3,  t4, t2
  add    a7, zero, a6     # we save the value of a6 into a7 to know the number of bytes to sqeeze
  addi   a6, zero, -1     # we set a6 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a2, perm
  sw     s0, 0(a0)
  xor    t1, t1, t2
  sw     t1, 4(a0)
  addi   a0, a0, 8
  j perm
64:  # >= 64 && <96 bits
  lw     t0, 0(a1)
  xor    s0, s0, t0
  lw     t1, 4(a1)
  xor    s1, s1, t1
  lw     t2, 8(a1)
  # mv t2, zero
  addi   a6, a6, -8
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s2, t2, a6,  t5,  t3,  t4, t1
  add    a7, zero, a6     # we save the value of a6 into a7 to know the number of bytes to sqeeze
  addi   a6, zero, -1     # we set a6 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a2, perm
  sw     s0, 0(a0)
  sw     s1, 4(a0)
  xor    t2, t2, t1
  sw     t2, 8(a0)
  addi   a0, a0, 12
  j perm
96:  # >= 96 &$ <128 bits
  lw     t0, 0(a1)
  xor    s0, s0, t0
  lw     t1, 4(a1)
  xor    s1, s1, t1
  lw     t2, 8(a1)
  xor    s2, s2, t2
  lw     t3, 12(a1)
  # mv t3, zero
  addi   a6, a6, -12
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s3, t3, a6,  t5,  t1,  t4, t2
  add    a7, zero, a6     # we save the value of a6 into a7 to know the number of bytes to sqeeze
  addi   a6, zero, -1     # we set a6 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a2, perm
  sw     s0, 0(a0)
  sw     s1, 4(a0)
  sw     s2, 8(a0)
  xor    t3, t3, t2
  sw     t3, 12(a0)
  addi   a0, a0, 16
  j perm
128:  # >= 128 bits
  lw     t0, 0(a1)
  xor    s0, s0, t0
  lw     t1, 4(a1)
  xor    s1, s1, t1
  lw     t2, 8(a1)
  xor    s2, s2, t2
  lw     t3, 12(a1)
  xor    s3, s3, t3
  addi   a1, a1, 16
  addi   a6, a6, -16
  add    a2, a2, a4       # +0 if AD, +16 if message
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a2, perm
  sw     s0, 0(a0)
  sw     s1, 4(a0)
  sw     s2, 8(a0)
  sw     s3, 12(a0)
  addi   a0, a0, 16
perm:
  # before applying the permutation,
  # apply permutation
# asm_gimli x0, x1, x2, x3, y0, y1, y2, y3, z0, z1,  z2, z3,  t0, t1, cst, round, c1, c2
  asm_gimli s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, t0, t1, t2,  t3,    t4, t5

  bgez   a6, absorb
  beq    a2, a1, tag # we have absorbed the full message => generate the tag
  add    a1, zero, a2
  add    a6, zero, a3
  addi   a4, zero, 16    # we are now going through messages
  j absorb

tag:
  # final squeeze is tricky as we may not be aligned if a7 == 0 we are aligned ... a bit too far
  bnez   a7, noalign
  sw     s0, -4(a0)
  sw     s1, 0(a0)
  sw     s2, 4(a0)
  sw     s3, 8(a0)
  j ret
noalign:
  lw     a1, -4(a0)
  lw     a2, 8(a0)
  sll    a7, a7, 3      # a7 = rotation to left
  addi   a6, zero, 32
  sub    a6, a6, a7     # a6 = rotation to right

  addi   t6, zero, -1
  sll    t6, t6, a7     # 11111100 masks
  xori   t5, t6, -1     # 00000011 masks

  sll    t2, s0, a7
  srl    t3, s0, a6

  and    a1, t5, a1
  or     s0, a1, t2
  sw     s0, -4(a0)

  sll    t2, s1, a7
  or     s0, t3, t2
  sw     s0, 0(a0)
  srl    t3, s1, a6

  sll    t2, s2, a7
  or     s0, t3, t2
  sw     s0, 4(a0)
  srl    t3, s2, a6

  sll    t2, s3, a7
  or     s0, t3, t2
  sw     s0, 8(a0)
  srl    t3, s3, a6

  and    a2, t6, a2
  or     s0, a2, t3
  sw     s0, 12(a0)

ret:
  lw     s0, 60(sp)
  lw     s1, 56(sp)
  lw     s2, 52(sp)
  lw     s3, 48(sp)
  lw     s4, 44(sp)
  lw     s5, 40(sp)
  lw     s6, 36(sp)
  lw     s7, 32(sp)
  lw     s8, 28(sp)
  lw     s9, 24(sp)
  lw    s10, 20(sp)
  lw    s11, 16(sp)
  addi   sp, sp, 64 # 20 * 4 + 16

  addi   a0, zero, 0
  ret

err:
  add    a0, zero, 1
  ret
.size crypto_aead_encrypt_asm,.-crypto_aead_encrypt_asm
