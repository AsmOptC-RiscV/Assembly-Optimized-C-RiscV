#include "ascon.h"

// This is a translation of the optimizations made in asm.S.
// It does not manually split 64-bit words into 32-bit words.
// It does include the optimized sbox formula.

void ascon_opt(state *p, int round_constant) {
	uint64_t s0 = p->x0;
	uint64_t s1 = p->x1;
	uint64_t s2 = p->x2;
	uint64_t s3 = p->x3;
	uint64_t s4 = p->x4;
	uint64_t t0, t1, t2;
	uint64_t r0, r1, r2, r3, r4;
	for (; round_constant > 0x3c; round_constant -= 15) {
		// addition of round constant
		s2 ^= (uint64_t)round_constant;
		// substitution layer
		t0 = s1 ^ s2;
		t1 = s0 ^ s4;
		t2 = s3 ^ s4;
		s4 = ~s4;
		s4 = s4 | s3;
		r2 = s4 ^ t0;
		s3 = s3 ^ s1;
		s3 = s3 | t0;
		r1 = s3 ^ t1;
		s2 = s2 ^ t1;
		s2 = s2 | s1;
		r0 = s2 ^ t2;
		t1 = ~t1;
		s1 = s1 & t1;
		r4 = s1 ^ t2;
		s0 = s0 | t2;
		r3 = s0 ^ t0;
		// linear diffusion layer
		s0 = r0 ^ ROTR64(r0, 19) ^ ROTR64(r0, 28);
		s1 = r1 ^ ROTR64(r1, 61) ^ ROTR64(r1, 39);
		s2 = r2 ^ ROTR64(r2,  1) ^ ROTR64(r2,  6);
		s3 = r3 ^ ROTR64(r3, 10) ^ ROTR64(r3, 17);
		s4 = r4 ^ ROTR64(r4,  7) ^ ROTR64(r4, 41);
	}
	p->x0 = s0;
	p->x1 = s1;
	p->x2 = s2;
	p->x3 = s3;
	p->x4 = s4;
}
