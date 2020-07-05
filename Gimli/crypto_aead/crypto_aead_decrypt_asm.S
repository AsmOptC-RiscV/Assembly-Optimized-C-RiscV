.text

.include "Gimli/Permutation/gimli.macros.s"


.globl crypto_aead_decrypt_asm
.type crypto_aead_decrypt_asm, %function
.align 3
crypto_aead_decrypt_asm:
  # 0(a0): unsigned char *m,
  # 0(a1): unsigned long long *mlen,
  # 0(a2): unsigned char *nsec,           // NOT USED
  # 0(a3): const unsigned char *c,
  # a4: unsigned long long clen,         : Length in BYTES !
  # a5: upper word of clen
  # 0(a6): const unsigned char *ad,
  # a7: unsigned long long adlen,        : Length in BYTES !
  # 0(sp): upper word of adlen
  # 0(sp+4): const unsigned char *npub,  // Nounce = 16 bytes => 0 4 8 12
  # 4(sp+4): const unsigned char *k      // Key = 32 bytes    => 0 4 8 12 16 20 24 28

  # Sorry we do not support more than 4GB of clen
  bne    a5, zero, err
  # Sorry we do not support more than 4GB of data
  lw     a5, 0(sp)
  bne    a5, zero, err

  ## if (clen < 16) return -1;
  # *mlen = tlen = clen - 16;
  addi   a4, a4, -16
  bltz   a4, err2
  sw     a4, 0(a1)

  # incrementation of message
  add   a5, zero, zero

.align 2
  # Note the ABI: x will be in a0 and the return value is expected in a0.
  addi   t0, sp, 4     # save the pointer location in order to be able to load key and nounce
  addi   sp, sp, -96   # 20 * 4 + 16
  sw     a0, 92(sp)  # just to be safe: *m
  sw     a1, 88(sp)  # just to be safe: *mlen
  sw     a2, 84(sp)  # just to be safe: *nsec
  sw     a3, 80(sp)  # just to be safe: *c
  sw     a4, 76(sp)  # just to be safe: lower clem
  sw     a5, 72(sp)  # just to be safe: upper clen
  sw     a6, 68(sp)  # just to be safe: *ad
  sw     a7, 64(sp)  # just to be safe: lower adlen
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
  add a1, zero, a6
  # a7 is the decreasing counter of length = -1 we switch to the next part.
  # Needs to take the value of mlen = a4 once we processed the additional data.

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
  addi   t5, a7, -16
  bgez   t5, 128f

  # since we are not looping anymore for absorption we can xor 1 in into byte 47
  addi   t3, zero, 1
  slli   t3, t3, 24
  xor    s11, s11, t3

  # check if remaining is more than 96 bits = 12 bytes
  addi   t5, a7, -12
  bgez   t5, 96f
  # check if remaining is more than 64 bits = 8 bytes
  addi   t5, a7, -8
  bgez   t5, 64f
  # check if remaining is more than 32 bits = 4 bytes
  addi   t5, a7, -4
  bgez   t5, 32f

# 0: <=32 bits
  lw     t0, 0(a1)
  # mv t0, zero
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s0, t0, a7,  t5,  a6,  t4, t6
  add    a6, zero, a7     # we save the value of a7 into a6 to know the number of bytes to sqeeze
  addi   a7, zero, -1     # we set a7 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a3, perm
  xor    s0, s0, t6    # remove the streamkey from from the state
  xor    t0, t0, t6    # decrypt the messag
  lw     t1, 0(a0)
  sw     t0, 0(a0)     ## TODO: FIX MISSING BITS
  addi   a1, a1, 4
  addi   a3, a3, 4
  j perm
32:  # >= 32 && <64 bits
  lw     t0, 0(a1)
  xor    s0, s0, t0
  lw     t1, 4(a1)
  # mv t1, zero
  addi   a7, a7, -4
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s1, t1, a7,  t5,  a6,  t4, t6
  add    a6, zero, a7     # we save the value of a7 into a6 to know the number of bytes to sqeeze
  addi   a7, zero, -1     # we set a7 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a3, perm
  sw     s0, 0(a0)
  # t0 countains the cipher text = previous state of the sponge
  add    s0, zero, t0
  xor    s1, s1, t6    # remove the streamkey from from the state
  xor    t1, t1, t6    # decrypt the messag
  sw     t1, 4(a0)
  addi   a1, a1, 8
  addi   a3, a3, 8
  j perm
64:  # >= 64 && <96 bits
  lw     t0, 0(a1)
  xor    s0, s0, t0
  lw     t1, 4(a1)
  xor    s1, s1, t1
  lw     t2, 8(a1)
  # mv t2, zero
  addi   a7, a7, -8
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s2, t2, a7,  t5,  a6,  t4, t6
  add    a6, zero, a7     # we save the value of a7 into a6 to know the number of bytes to sqeeze
  addi   a7, zero, -1     # we set a7 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a3, perm
  sw     s0, 0(a0)
  sw     s1, 4(a0)
  # t0, t1 countains the cipher text = previous state of the sponge
  add    s0, zero, t0
  add    s1, zero, t1
  xor    s2, s2, t6    # remove the streamkey from from the state
  xor    t2, t2, t6    # decrypt the messag
  sw     t2, 8(a0)
  addi   a1, a1, 12
  addi   a3, a3, 12
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
  addi   a7, a7, -12
  # padSav to, in, len, tmp, tmp, tmp, save
  padSav   s3, t3, a7,  t5,  a6,  t4, t6
  add    a6, zero, a7     # we save the value of a7 into a6 to know the number of bytes to sqeeze
  addi   a7, zero, -1     # we set a6 to -1 to indicate that we need to switch between AD and MSG or generate the TAG
  # SQEEZE FOR CIPHER TEXT
  bne    a1, a3, perm
  sw     s0, 0(a0)
  sw     s1, 4(a0)
  sw     s2, 8(a0)
  # t0, t1, t2 countains the cipher text = previous state of the sponge
  add    s0, zero, t0
  add    s1, zero, t1
  add    s2, zero, t2
  xor    s3, s3, t6    # remove the streamkey from from the state
  xor    t3, t3, t6    # decrypt the messag
  sw     t3, 12(a0)
  addi   a1, a1, 16
  addi   a3, a3, 16
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
  addi   a7, a7, -16
  add    a3, a3, a5       # +0 if AD, +16 if message
  # SQEEZE FOR s0, s1, s2, s3 countains now the plaintext
  bne    a1, a3, perm
  sw     s0, 0(a0)
  sw     s1, 4(a0)
  sw     s2, 8(a0)
  sw     s3, 12(a0)
  # t0, t1, t2, t3 countains the cipher text = previous state of the sponge
  add    s0, zero, t0
  add    s1, zero, t1
  add    s2, zero, t2
  add    s3, zero, t3
  addi   a0, a0, 16
perm:
  # before applying the permutation,
  # apply permutation
# asm_gimli x0, x1, x2, x3, y0, y1, y2, y3, z0, z1,  z2, z3,  t0, t1, cst, round, c1, c2
  asm_gimli s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, t0, t1, t2,  t3,    t4, t5

  bgez a7, absorb
  # bne a1, a3, tag # we have absorbed the full message => generate the tag
  beq a1, a3, tag # we have absorbed the full message => generate the tag
  add a1, zero, a3
  add a7, zero, a4
  addi a5, zero, 16    # we are now going through messages
  j absorb

tag:
  # final squeeze is tricky as we may not be aligned if a7 == 0 we are aligned ... a bit too far
  bnez a6, noalign
  lw t0, -4(a1)
  lw t1, 0(a1)
  lw t2, 4(a1)
  lw t3, 8(a1)
  # t0 t1 t2 t3 contains the tag
  j check
noalign:
  j ret
  lw t5, -4(a1)
  lw t0, 0(a1)
  lw t1, 4(a1)
  lw t2, 8(a1)
  lw t3, 12(a1)

  # t0 t1 t2 t3 contains the tag, shifted by a5 * 8 to the left
  sll  a6, a6, 3      # a7 = rotation to left
  addi a5, zero, 32
  sub  a5, a5, a6     # a6 = rotation to right

  srl t3, t3, a5
  sll t6, t2, a6
  or  t3, t3, t6

  srl t2, t2, a5
  sll t6, t1, a6
  or  t2, t2, t6

  srl t1, t1, a5
  sll t6, t0, a6
  or  t1, t1, t6

  srl t0, t0, a5
  sll t6, t5, a6
  or  t0, t0, t6

check:
  bne t0, s0, erase
  bne t1, s1, erase
  bne t2, s2, erase
  bne t3, s3, erase
  j ret
erase:
  lw     a0, 92(sp)  # just to be safe: *m
  lw     a1, 88(sp)  # just to be safe: *mlen
  lw     a2, 0(a1)
  addi t1, zero, -1
3445300:
  addi  t2, a2, -16
  bgez  t2, 3445316f
  addi  t2, a2, -12
  bgez  t2, 3445312f
  addi  t2, a2, -8
  bgez  t2, 344538f
  addi  t2, a2, -4
  bgez  t2, 344534f
344530:
  lw      t0, 0(a0)
  sll     t2, t2, 3
  sll     t1, t1, t2
  xori    t1, t1, -1
  and     t0, t0, t1
  sw      t0, 0(a0)
  j ret
344534:
  sw    zero, 0(a0)
  lw      t0, 4(a0)
  sll     t2, t2, 3
  sll     t1, t1, t2
  xori    t1, t1, -1
  and     t0, t0, t1
  sw      t0, 4(a0)
  j ret
344538:
  sw    zero, 0(a0)
  sw    zero, 4(a0)
  lw      t0, 8(a0)
  sll     t2, t2, 3
  sll     t1, t1, t2
  xori    t1, t1, -1
  and     t0, t0, t1
  sw      t0, 8(a0)
  j ret
3445312:
  sw    zero, 0(a0)
  sw    zero, 4(a0)
  sw    zero, 8(a0)
  lw      t0, 12(a0)
  sll     t2, t2, 3
  sll     t1, t1, t2
  xori    t1, t1, -1
  and     t0, t0, t1
  sw      t0, 12(a0)
  j ret
3445316:
  sw    zero, 0(a0)
  sw    zero, 4(a0)
  sw    zero, 8(a0)
  sw    zero, 12(a0)
  addi  a0, a0, 16
  addi  a2, a2, -16
j 3445300b

ret:
  lw      s0, 60(sp)
  lw      s1, 56(sp)
  lw      s2, 52(sp)
  lw      s3, 48(sp)
  lw      s4, 44(sp)
  lw      s5, 40(sp)
  lw      s6, 36(sp)
  lw      s7, 32(sp)
  lw      s8, 28(sp)
  lw      s9, 24(sp)
  lw     s10, 20(sp)
  lw     s11, 16(sp)
  addi    sp, sp, 96 # 20 * 4 + 16

  addi    a0, zero, 0
  ret

err:
  add     a0, zero, 1
  ret

err2:
  add     a0, zero, -1
  ret

.size crypto_aead_decrypt_asm,.-crypto_aead_decrypt_asm
