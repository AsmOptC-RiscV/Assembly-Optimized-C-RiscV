# This macro computes boolean formulas of the substitution layer in
# parallel on the registers s0 through s4, using t0, t1 and t2 as
# temporary registers. This macro must be run twice with different
# state registers, because each 64-bit state word is split in two
# 32-bit machine words. Note that after this macro, the state words
# end up in different registers. In order to make space to move the
# state words back into position, the final result is stored in
# register r0 instead.
.macro sbox s0, s1, s2, s3, s4, r0
	xor t0, \s1, \s2
	xor t1, \s0, \s4
	xor t2, \s3, \s4
	not \s4, \s4
	or \s4, \s4, \s3
	xor \s4, \s4, t0
	xor \s3, \s3, \s1
	or \s3, \s3, t0
	xor \s3, \s3, t1
	xor \s2, \s2, t1
	or \s2, \s2, \s1
	xor \s2, \s2, t2
	not t1, t1
	and \s1, \s1, t1
	xor \s1, \s1, t2
	or \s0, \s0, t2
	xor \r0, \s0, t0
.endm

# The xorror macro applies the rotates and bitwise additions of the
# linear diffusion layer to a single state word. Because of the
# limitations of assembly macros, it takes many arguments: dl and dh
# are the registers for storing the low and high part of the result.
# sl, sh, sl0, sh0, sl1 and sh1 are the source registers for the state
# word, without rotation, with rotation r0 and with rotation r1
# respectively. Because 32-bit shift instructions can only shift by 31
# at most, representing rotate amounts above 32 is done by subtracting
# 32 and swapping the respective source registers. This is why the
# source registers must be supplied three times.
.macro xorror dl, dh, sl, sh, sl0, sh0, r0, sl1, sh1, r1
	slli t0, \sl0, (32 - \r0)
	srli t2, \sh0, \r0
	xor t0, t0, t2
	slli t2, \sl1, (32 - \r1)
	xor t0, t0, t2
	srli t2, \sh1, \r1
	xor t0, t0, t2
	slli t1, \sh0, (32 - \r0)
	srli t2, \sl0, \r0
	xor t1, t1, t2
	slli t2, \sh1, (32 - \r1)
	xor t1, t1, t2
	srli t2, \sl1, \r1
	xor t1, t1, t2
	xor \dl, \sl, t1
	xor \dh, \sh, t0
.endm

.text

.globl ascon_asm
.align 2
ascon_asm:
	# a0 state pointer
	# a1 round constant

	addi sp, sp, -20
	sw s0, (sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)

	mv t6, a0
	mv t5, a1
	li a5, 0x3c

	# t6 state pointer
	# t5 round constant
	# a5 final round constant

	lw a0, (t6)
	lw s0, 4(t6)
	lw a1, 8(t6)
	lw s1, 12(t6)
	lw a2, 16(t6)
	lw s2, 20(t6)
	lw a3, 24(t6)
	lw s3, 28(t6)
	lw a4, 32(t6)
	lw s4, 36(t6)

	# a0-a4 lower halves of state
	# s0-s4 upper halves of state

# The main permutation loop that loops over all rounds in a
# permutation, defined in terms of the sbox and xorror macros. t5
# contains the round constant, a0 through a4 contain the least
# significant halves of the five state words, s0 through s4 contain
# the most significant halves of the five state words. t0 through t4
# are used as temporary registers and a5 contains the round constant
# where the loop should end, 0x3c.
round:
	# Addition of round constant
	xor a2, a2, t5

	# Substitution layer
	sbox a0, a1, a2, a3, a4, t3
	sbox s0, s1, s2, s3, s4, t4

	# Linear diffusion layer
	xorror a0, s0, a2, s2, a2, s2, 19, a2, s2, 28
	xorror a2, s2, a4, s4, a4, s4, 1,  a4, s4, 6
	xorror a4, s4, a1, s1, a1, s1, 7,  s1, a1, 9
	xorror a1, s1, a3, s3, s3, a3, 29, s3, a3, 7
	xorror a3, s3, t3, t4, t3, t4, 10, t3, t4, 17

	# Compute next round constant
	addi t5, t5, -15

	# Loop back if final round has not been reached
	bne a5, t5, round

	sw a0, (t6)
	sw s0, 4(t6)
	sw a1, 8(t6)
	sw s1, 12(t6)
	sw a2, 16(t6)
	sw s2, 20(t6)
	sw a3, 24(t6)
	sw s3, 28(t6)
	sw a4, 32(t6)
	sw s4, 36(t6)

	lw s0, (sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	addi sp, sp, 20

	ret
