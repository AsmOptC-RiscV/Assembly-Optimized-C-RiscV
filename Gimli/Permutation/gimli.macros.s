.text
#    o0: output (can overlap with input)
#    i0: input, const
#    c0: rotate distance, immediate
# r0-r1: tmp
.macro ror o0, i0, c0, r0, r1
    srli   \r0, \i0, \c0
    slli   \r1, \i0, 32-\c0
    or     \o0, \r0, \r1
.endm


#    apply the SPbox
#   arguments are quite explicit
.macro spbox x, y, z, u, v
    # START SPBOX
    # u = rotate x <<< 24
    ror \u, \x, 8, \u, \x
    # v = rotate y <<< 9
    ror \v, \y, 23, \v, \y
    # compute z (post swap: x)
    and \x, \u, \v
    slli \x, \x, 3
    xor \x, \x, \v
    xor \x, \x, \z
    # compute y
    or  \y, \u, \z
    slli \y, \y, 1
    xor \y, \y, \u
    xor \y, \y, \v
    # compute x (post swap: z)
    and \v, \v, \z
    slli \v, \v, 2
    xor \v, \u, \v
    slli \z, \z, 1
    xor \z, \z, \v
    # END SPBOX
.endm


# state: row of the state
# const in: temporary variable containing the input
# const len: length in BYTES of the input either 0, 1, 2, 3
# given state = [a b c d]; input = [e f g h]; len = 1
# returns state = [a b c+1 d+h], input = [e f g h]
.macro pad state, input, length, pad, mask, t2
  addi \pad, zero, 1     # pad =  00000001
  addi \mask, zero, -1   # mask = 11111111
  sll \t2, \length, 3       # t2 = len * 8       # we work on bits !
  sll \pad, \pad, \t2    # pas = pad << 16 -> 00010000
  sll \mask, \mask, \t2
  xori \mask, \mask, -1
  and \mask, \mask, \input
   or  \mask, \pad, \mask
  xor  \state, \state, \mask    # to = to xor t2
.endm


# to: row of the state
# const in: temporary variable containing the input
# const len: length in BYTES of the input either 0, 1, 2, 3
# given state = [a b c d]; input = [e f g h]; len = 1
# returns state = [a b c+1 d+h], input = [e f g h], saved = d
.macro padSav state, input, len, pad, mask, t2, saved
  addi \pad, zero, 1     # pad =  00000001
  addi \mask, zero, -1   # mask = 11111111
  sll \t2, \len, 3       # t2 = len * 8       # we work on bits !
  sll \pad, \pad, \t2    # pas = pad << 16 -> 00010000
  sll \mask, \mask, \t2
  xori \mask, \mask, -1
  and \saved, \mask, \state
  and \mask, \mask, \input
   or  \mask, \pad, \mask
  xor  \state, \state, \mask    # to = to xor t2
.endm



# .macro pad to, in, len, pad, t2, t3
#   addi \pad, zero, 1
#   sll  \t2, \len, 3   # t2 = len * 8       # we work on bits !
#   8: #loop to xor bytes
#   beqz \t2, 127f
#     slli \pad, \pad, 8
#     addi \t2, \t2, -8
#     srl \t3, \in, \t2
#     andi \t3, \t3, 0xff
#     or \pad, \pad, \t3
#   j 8b
#   127:
#    xor  \to, \to, \pad    # to = to xor t2
# .endm


.macro asm_gimli x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3, t0, t1, cst, round, c1, c2
.align 3
    .equ ROUNDCONSTANT, 0x9e377900
    lui \cst,      %hi(ROUNDCONSTANT)
    addi \cst, \cst, %lo(ROUNDCONSTANT)
    addi \round, \cst, 24
    addi \c1, zero, 0
    addi \c2, zero, 0
123451:  # loop
    beq \round, \cst, 123455f
123452:  # loop
    spbox \x0, \y0, \z0, \t0, \t1
    spbox \x1, \y1, \z1, \t0, \t1
    spbox \x2, \y2, \z2, \t0, \t1
    spbox \x3, \y3, \z3, \t0, \t1
    bgtz \c2, 123454f
    xori \c2, \c2, 1
    bgtz \c1, 123453f
    xori \c1, \c1, 1
    xor \x0, \x0, \x1
    xor \x1, \x0, \x1
    xor \x0, \x0, \x1
    xor \x2, \x2, \x3
    xor \x3, \x2, \x3
    xor \x2, \x2, \x3
    # add \t0, \round, \cst
    xor \x0, \x0, \round
    addi \round, \round, -1
    j 123452b
123453: # bigswap
    xori \c1, \c1, 1
    xor \x0, \x0, \x2
    xor \x2, \x0, \x2
    xor \x0, \x0, \x2
    xor \x1, \x1, \x3
    xor \x3, \x1, \x3
    xor \x1, \x1, \x3
    addi \round, \round, -1
    j 123452b
123454:
    addi \round, \round, -1
    xori \c2, \c2, 1
    j 123451b
123455: # save values
.endm
