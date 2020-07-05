#pragma once

#include <stdint.h>
typedef uint64_t u64;
typedef uint64_t U64;
typedef uint8_t U8;
typedef uint8_t u8;

typedef struct {
	uint64_t x0, x1, x2, x3, x4;
} state;

void ascon_ref(state *s, int round_constant);
void ascon_asm(state *s, int round_constant);
void ascon_opt(state *s, int round_constant);
void ascon_opt_2(state *s, int round_constant);

inline static  uint64_t __attribute__((always_inline))
BYTES_TO_U64(const uint8_t* bytes, int n) {
	int i;
	uint64_t x = 0;
	for (i = 0; i < n; i++) x |= ((uint64_t)bytes[i]) << (56 - 8 * i);
	return x;
}

inline static  void __attribute__((always_inline))
U64_TO_BYTES(uint8_t* bytes, const uint64_t x, int n) {
	int i;
	for (i = 0; i < n; i++) bytes[i] = (uint8_t)(x >> (56 - 8 * i));
}

inline static  uint64_t __attribute__((always_inline))
BYTE_MASK(int n) {
	int i;
	uint64_t x = 0;
	for (i = 0; i < n; i++) x |= 0xffull << (56 - 8 * i);
	return x;
}

inline static  uint64_t __attribute__((always_inline))
ROTR64(uint64_t x, int n) { return (x << (64 - n)) | (x >> n); }
