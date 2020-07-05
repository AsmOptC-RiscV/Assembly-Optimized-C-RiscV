#ifndef PERMUTATIONS_H_
#define PERMUTATIONS_H_

typedef unsigned char u8;
typedef unsigned long long u64;

typedef struct {
  u64 x0, x1, x2, x3, x4;
} state;

#define EXT_BYTE64(x, n) ((u8)((u64)(x) >> (8 * (7 - (n)))))
#define INS_BYTE64(x, n) ((u64)(x) << (8 * (7 - (n))))
#define ROTR64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))

#define ROUND(C)                    \
  do {                              \
    state t, r;                     \
    s.x2 ^= C;                      \
    t.x0 = s.x1 ^ s.x2; \
    t.x1 = s.x0 ^ s.x4; \
    t.x2 = s.x3 ^ s.x4; \
    s.x4 = ~s.x4; \
    s.x4 = s.x4 | s.x3; \
    r.x2 = s.x4 ^ t.x0; \
    s.x3 = s.x3 ^ s.x1; \
    s.x3 = s.x3 | t.x0; \
    r.x1 = s.x3 ^ t.x1; \
    s.x2 = s.x2 ^ t.x1; \
    s.x2 = s.x2 | s.x1; \
    r.x0 = s.x2 ^ t.x2; \
    t.x1 = ~t.x1; \
    s.x1 = s.x1 & t.x1; \
    r.x4 = s.x1 ^ t.x2; \
    s.x0 = s.x0 | t.x2; \
    r.x3 = s.x0 ^ t.x0; \
    s.x0 = r.x0 ^ ROTR64(r.x0, 19) ^ ROTR64(r.x0, 28); \
    s.x1 = r.x1 ^ ROTR64(r.x1, 61) ^ ROTR64(r.x1, 39); \
    s.x2 = r.x2 ^ ROTR64(r.x2,  1) ^ ROTR64(r.x2,  6); \
    s.x3 = r.x3 ^ ROTR64(r.x3, 10) ^ ROTR64(r.x3, 17); \
    s.x4 = r.x4 ^ ROTR64(r.x4,  7) ^ ROTR64(r.x4, 41); \
  } while (0)

#define P12()    \
  do {           \
    ROUND(0xf0); \
    ROUND(0xe1); \
    ROUND(0xd2); \
    ROUND(0xc3); \
    ROUND(0xb4); \
    ROUND(0xa5); \
    ROUND(0x96); \
    ROUND(0x87); \
    ROUND(0x78); \
    ROUND(0x69); \
    ROUND(0x5a); \
    ROUND(0x4b); \
  } while (0)

#define P6()     \
  do {           \
    ROUND(0x96); \
    ROUND(0x87); \
    ROUND(0x78); \
    ROUND(0x69); \
    ROUND(0x5a); \
    ROUND(0x4b); \
  } while (0)

#endif  // PERMUTATIONS_H_

