#include "aes.h"

// output0-output1: output (can overlap with input)
//   input0-input1: intput, const
//            mask: mask, const
//           shift: shift distance, immediate
inline static void __attribute__((always_inline))
swapmove(uint32_t *output0, uint32_t *output1, const uint32_t *input0,
         const uint32_t *input1, const uint32_t mask, const int shift) {
    uint32_t temp = ((*input0 << shift) ^ *input1) & mask;
    *output1 = *input1 ^ temp;
    *output0 = *input0 ^ (temp >> shift);
}

// k0-k3: round key, const
// b0-b7: bitsliced round key
// m0-m2: masks, const
inline static void __attribute__((always_inline))
ksbitslice(const uint32_t *k0, const uint32_t *k1, const uint32_t *k2,
           const uint32_t *k3, uint32_t *b0, uint32_t *b1, uint32_t *b2,
           uint32_t *b3, uint32_t *b4, uint32_t *b5, uint32_t *b6, uint32_t *b7,
           const uint32_t mask0, const uint32_t mask1, const uint32_t mask2) {
    swapmove(b0, b4, k0, k0, mask0, 1);
    swapmove(b1, b5, k1, k1, mask0, 1);
    swapmove(b2, b6, k2, k2, mask0, 1);
    swapmove(b3, b7, k3, k3, mask0, 1);

    swapmove(b0, b1, b0, b1, mask1, 2);
    swapmove(b2, b3, b2, b3, mask1, 2);
    swapmove(b4, b5, b4, b5, mask1, 2);
    swapmove(b6, b7, b6, b7, mask1, 2);

    swapmove(b0, b2, b0, b2, mask2, 4);
    swapmove(b1, b3, b1, b3, mask2, 4);
    swapmove(b4, b6, b4, b6, mask2, 4);
    swapmove(b5, b7, b5, b7, mask2, 4);
}

//   out: pointer to write to, const
// b0-b7: bitsliced round key, const
inline static void __attribute__((always_inline))
storerk(uint32_t *out, const uint32_t b0, const uint32_t b1, const uint32_t b2,
        const uint32_t b3, const uint32_t b4, const uint32_t b5,
        const uint32_t b6, const uint32_t b7) {
    out[0] = b0;
    out[1] = b1;
    out[2] = b2;
    out[3] = b3;
    out[4] = b4;
    out[5] = b5;
    out[6] = b6;
    out[7] = b7;
}

// k0-k3: round key
//    c0: mask 0xff0, const
//    c1: address of AESTable, const
//    c2: round constant, immediate
// r0-r4: tmp

inline static void __attribute__((always_inline))
ksround(uint32_t *k0, uint32_t *k1, uint32_t *k2, uint32_t *k3,
        const uint32_t mask, const uint8_t *aestable, const uint32_t imm) {
    uint32_t r3 = (*k3 << 4) & mask;
    uint32_t r0 = (*k3 >> 4) & mask;
    uint32_t r1 = (*k3 >> 12) & mask;
    uint32_t r2 = (*k3 >> 20) & mask;

    *k0 = *k0 ^ imm;
    *k0 = *k0 ^ aestable[r0];
    *k0 = *k0 ^ (aestable[r1] << 8);
    *k0 = *k0 ^ (aestable[r2] << 16);
    *k0 = *k0 ^ (aestable[r3] << 24);
    *k1 = *k1 ^ *k0;
    *k2 = *k2 ^ *k1;
    *k3 = *k3 ^ *k2;
}

inline static void __attribute__((always_inline))
subbytesnots(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3) {
    *b0 = ~*b0;
    *b1 = ~*b1;
    *b2 = ~*b2;
    *b3 = ~*b3;
}

void AES_128_keyschedule_C(const uint8_t *key, uint8_t *rk) {
    // a0: key (pointer to intput), then mask0
    // rk: rk (pointer to output)

    // load key
    uint32_t key0 = ((uint32_t *)key)[0];
    uint32_t key1 = ((uint32_t *)key)[1];
    uint32_t key2 = ((uint32_t *)key)[2];
    uint32_t key3 = ((uint32_t *)key)[3];

    // load table address once
    uint8_t *aestable = (uint8_t *)AESTable;

    // set 0x00000ff0 in t6
    const uint32_t mask = 0xff0;

    // set bitslice masks
    const uint32_t aaaaaaaa = 0xaaaaaaaa;
    const uint32_t cccccccc = 0xcccccccc;
    const uint32_t ffffffff = 0xf0f0f0f0;

    // state variables
    uint32_t temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;

    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    storerk((uint32_t *)(rk + 0), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x01);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 32), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x02);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 64), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x04);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 96), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x08);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 128), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x10);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 160), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x20);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 192), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x40);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 224), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x80);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 256), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x1b);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 288), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
    ksround(&key0, &key1, &key2, &key3, mask, aestable, 0x36);
    ksbitslice(&key0, &key1, &key2, &key3, &temp0, &temp1, &temp2, &temp3,
               &temp4, &temp5, &temp6, &temp7, aaaaaaaa, cccccccc, ffffffff);
    subbytesnots(&temp1, &temp3, &temp4, &temp7);
    storerk((uint32_t *)(rk + 320), temp0, temp4, temp1, temp5, temp2, temp6,
            temp3, temp7);
}

inline static void __attribute__((always_inline)) revbytes(uint32_t *b0) {
    // original is more complex but this order makes more sense.
    // may need optimizations.
    uint32_t r0 = *b0 & 0xff;
    *b0 = *b0 >> 8;
    uint32_t r1 = *b0 & 0xff;
    *b0 = *b0 >> 8;
    uint32_t r2 = *b0 & 0xff;
    *b0 = *b0 >> 8;
    r0 = r0 << 8;
    r0 = (r0 | r1) << 8;
    r0 = (r0 | r2) << 8;
    *b0 = *b0 | r0;
}

// b0-b7: state
inline static void __attribute__((always_inline))
bitslice(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3, uint32_t *b4,
         uint32_t *b5, uint32_t *b6, uint32_t *b7) {
    uint32_t mask = 0xaaaaaaaa;
    uint32_t temp2, temp3;
    swapmove(b0, b4, b0, b4, mask, 1);
    swapmove(b1, b5, b1, b5, mask, 1);
    swapmove(b2, b6, b2, b6, mask, 1);
    swapmove(b3, b7, b3, b7, mask, 1);

    mask = 0xcccccccc;
    swapmove(b0, &temp2, b0, b1, mask, 2);
    swapmove(b1, b5, b4, b5, mask, 2);
    swapmove(b4, b3, b2, b3, mask, 2);
    swapmove(&temp3, b7, b6, b7, mask, 2);

    mask = 0xf0f0f0f0;
    swapmove(b0, b4, b0, b4, mask, 4);
    swapmove(b2, b6, &temp2, b3, mask, 4);
    swapmove(b3, b7, b5, b7, mask, 4);
    swapmove(b1, b5, b1, &temp3, mask, 4);
}

// b0-b7: state
inline static void __attribute__((always_inline))
unbitslice(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3, uint32_t *b4,
           uint32_t *b5, uint32_t *b6, uint32_t *b7) {
    uint32_t mask = 0xf0f0f0f0;
    uint32_t temp2;
    swapmove(b0, b4, b0, b4, mask, 4);
    swapmove(b2, b6, b2, b6, mask, 4);
    swapmove(b3, b7, b3, b7, mask, 4);
    swapmove(&temp2, b5, b1, b5, mask, 4);

    mask = 0xcccccccc;
    swapmove(b0, b1, b0, b2, mask, 2);
    swapmove(b2, b6, b4, b6, mask, 2);
    swapmove(b4, b3, &temp2, b3, mask, 2);
    swapmove(&temp2, b7, b5, b7, mask, 2);

    mask = 0xaaaaaaaa;
    swapmove(b0, b4, b0, b4, mask, 1);
    swapmove(b1, b5, b1, b3, mask, 1);
    swapmove(b3, b7, b6, b7, mask, 1);
    swapmove(b2, b6, b2, &temp2, mask, 1);
}

// b0-b7: state
//    rk: pointer to round keys
inline static void __attribute__((always_inline))
addroundkey(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3,
            uint32_t *b4, uint32_t *b5, uint32_t *b6, uint32_t *b7,
            const uint32_t *rk) {
    *b0 ^= rk[0];
    *b1 ^= rk[1];
    *b2 ^= rk[2];
    *b3 ^= rk[3];
    *b4 ^= rk[4];
    *b5 ^= rk[5];
    *b6 ^= rk[6];
    *b7 ^= rk[7];
}

//  b0-b7: state
// r0-r17: tmp
inline static void __attribute__((always_inline))
subbytes(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3, uint32_t *b4,
         uint32_t *b5, uint32_t *b6, uint32_t *b7) {
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14,
        r15, r16, r17;
    r0 = *b3 ^ *b5;  /* Exec y14 = U3 ^ U5 into r0         */
    r1 = *b0 ^ *b6;  /* Exec y13 = U0 ^ U6 into r1         */
    r2 = r1 ^ r0;    /* Exec y12 = y13 ^ y14 into r2       */
    r3 = *b4 ^ r2;   /* Exec t1 = U4 ^ y12 into r3         */
    r4 = r3 ^ *b5;   /* Exec y15 = t1 ^ U5 into r4         */
    r5 = r2 & r4;    /* Exec t2 = y12 & y15 into r5        */
    r6 = r4 ^ *b7;   /* Exec y6 = y15 ^ U7 into r6         */
    r7 = r3 ^ *b1;   /* Exec y20 = t1 ^ U1 into r7         */
    r8 = *b0 ^ *b3;  /* Exec y9 = U0 ^ U3 into r8          */
    r9 = r7 ^ r8;    /* Exec y11 = y20 ^ y9 into r9        */
    r10 = r8 & r9;   /* Exec t12 = y9 & y11 into r10       */
    r11 = *b7 ^ r9;  /* Exec y7 = U7 ^ y11 into r11        */
    r12 = *b0 ^ *b5; /* Exec y8 = U0 ^ U5 into r12         */
    r13 = *b1 ^ *b2; /* Exec t0 = U1 ^ U2 into r13         */
    r14 = r4 ^ r13;  /* Exec y10 = y15 ^ t0 into r14       */
    r15 = r14 ^ r9;  /* Exec y17 = y10 ^ y11 into r15      */
    r16 = r0 & r15;  /* Exec t13 = y14 & y17 into r16      */
    r17 = r16 ^ r10; /* Exec t14 = t13 ^ t12 into r17      */
    *b1 = r14 ^ r12; /* Exec y19 = y10 ^ y8 into b1        */
    *b2 = r12 & r14; /* Exec t15 = y8 & y10 into b2        */
    *b2 = *b2 ^ r10; /* Exec t16 = t15 ^ t12 into b2       */
    *b4 = r13 ^ r9;  /* Exec y16 = t0 ^ y11 into b4        */
    *b5 = r1 ^ *b4;  /* Exec y21 = y13 ^ y16 into b5       */
    r3 = r1 & *b4;   /* Exec t7 = y13 & y16 into r3        */
    r10 = *b0 ^ *b4; /* Exec y18 = U0 ^ y16 into r10       */
    r13 = r13 ^ *b7; /* Exec y1 = t0 ^ U7 into r13         */
    *b3 = r13 ^ *b3; /* Exec y4 = y1 ^ U3 into b3          */
    r16 = *b3 & *b7; /* Exec t5 = y4 & U7 into r16         */
    r16 = r16 ^ r5;  /* Exec t6 = t5 ^ t2 into r16         */
    r16 = r16 ^ *b2; /* Exec t18 = t6 ^ t16 into r16       */
    *b1 = r16 ^ *b1; /* Exec t22 = t18 ^ y19 into b1       */
    *b0 = r13 ^ *b0; /* Exec y2 = y1 ^ U0 into b0          */
    r16 = *b0 & r11; /* Exec t10 = y2 & y7 into r16        */
    r16 = r16 ^ r3;  /* Exec t11 = t10 ^ t7 into r16       */
    *b2 = r16 ^ *b2; /* Exec t20 = t11 ^ t16 into b2       */
    *b2 = *b2 ^ r10; /* Exec t24 = t20 ^ y18 into b2       */
    *b6 = r13 ^ *b6; /* Exec y5 = y1 ^ U6 into b6          */
    r10 = *b6 & r13; /* Exec t8 = y5 & y1 into r10         */
    r3 = r10 ^ r3;   /* Exec t9 = t8 ^ t7 into r3          */
    r3 = r3 ^ r17;   /* Exec t19 = t9 ^ t14 into r3        */
    *b5 = r3 ^ *b5;  /* Exec t23 = t19 ^ y21 into b5       */
    r3 = *b6 ^ r12;  /* Exec y3 = y5 ^ y8 into r3          */
    r10 = r3 & r6;   /* Exec t3 = y3 & y6 into r10         */
    r5 = r10 ^ r5;   /* Exec t4 = t3 ^ t2 into r5          */
    r5 = r5 ^ r7;    /* Exec t17 = t4 ^ y20 into r5        */
    r5 = r5 ^ r17;   /* Exec t21 = t17 ^ t14 into r5       */
    r7 = r5 & *b5;   /* Exec t26 = t21 & t23 into r7       */
    r10 = *b2 ^ r7;  /* Exec t27 = t24 ^ t26 into r10      */
    r7 = *b1 ^ r7;   /* Exec t31 = t22 ^ t26 into r7       */
    r5 = r5 ^ *b1;   /* Exec t25 = t21 ^ t22 into r5       */
    r16 = r5 & r10;  /* Exec t28 = t25 & t27 into r16      */
    *b1 = r16 ^ *b1; /* Exec t29 = t28 ^ t22 into b1       */
    r17 = *b1 & *b0; /* Exec z14 = t29 & y2 into r17       */
    r11 = *b1 & r11; /* Exec z5 = t29 & y7 into r11        */
    r16 = *b5 ^ *b2; /* Exec t30 = t23 ^ t24 into r16      */
    r7 = r7 & r16;   /* Exec t32 = t31 & t30 into r7       */
    r7 = r7 ^ *b2;   /* Exec t33 = t32 ^ t24 into r7       */
    r16 = r10 ^ r7;  /* Exec t35 = t27 ^ t33 into r16      */
    *b2 = *b2 & r16; /* Exec t36 = t24 & t35 into b2       */
    r10 = r10 ^ *b2; /* Exec t38 = t27 ^ t36 into r10      */
    r10 = *b1 & r10; /* Exec t39 = t29 & t38 into r10      */
    r5 = r5 ^ r10;   /* Exec t40 = t25 ^ t39 into r5       */
    r10 = *b1 ^ r5;  /* Exec t43 = t29 ^ t40 into r10      */
    *b4 = r10 & *b4; /* Exec z3 = t43 & y16 into b4        */
    r11 = *b4 ^ r11; /* Exec tc12 = z3 ^ z5 into r11       */
    r1 = r10 & r1;   /* Exec z12 = t43 & y13 into r1       */
    *b6 = r5 & *b6;  /* Exec z13 = t40 & y5 into b6        */
    r10 = r5 & r13;  /* Exec z4 = t40 & y1 into r10        */
    *b4 = *b4 ^ r10; /* Exec tc6 = z3 ^ z4 into b4         */
    *b5 = *b5 ^ r7;  /* Exec t34 = t23 ^ t33 into b5       */
    *b2 = *b2 ^ *b5; /* Exec t37 = t36 ^ t34 into b2       */
    *b5 = r5 ^ *b2;  /* Exec t41 = t40 ^ t37 into b5       */
    r5 = *b5 & r14;  /* Exec z8 = t41 & y10 into r5        */
    r10 = *b5 & r12; /* Exec z17 = t41 & y8 into r10       */
    r12 = r7 ^ *b2;  /* Exec t44 = t33 ^ t37 into r12      */
    r4 = r12 & r4;   /* Exec z0 = t44 & y15 into r4        */
    r2 = r12 & r2;   /* Exec z9 = t44 & y12 into r2        */
    r3 = *b2 & r3;   /* Exec z10 = t37 & y3 into r3        */
    *b2 = *b2 & r6;  /* Exec z1 = t37 & y6 into b2         */
    *b2 = *b2 ^ r4;  /* Exec tc5 = z1 ^ z0 into b2         */
    r13 = *b4 ^ *b2; /* Exec tc11 = tc6 ^ tc5 into r13     */
    *b3 = r7 & *b3;  /* Exec z11 = t33 & y4 into b3        */
    *b1 = *b1 ^ r7;  /* Exec t42 = t29 ^ t33 into b1       */
    *b5 = *b1 ^ *b5; /* Exec t45 = t42 ^ t41 into b5       */
    r6 = *b5 & r15;  /* Exec z7 = t45 & y17 into r6        */
    *b4 = r6 ^ *b4;  /* Exec tc8 = z7 ^ tc6 into b4        */
    r0 = *b5 & r0;   /* Exec z16 = t45 & y14 into r0       */
    *b5 = *b1 & r9;  /* Exec z6 = t42 & y11 into b5        */
    *b5 = *b5 ^ *b4; /* Exec tc16 = z6 ^ tc8 into b5       */
    *b1 = *b1 & r8;  /* Exec z15 = t42 & y9 into b1        */
    r6 = *b1 ^ *b5;  /* Exec tc20 = z15 ^ tc16 into r6     */
    r0 = *b1 ^ r0;   /* Exec tc1 = z15 ^ z16 into r0       */
    *b1 = r3 ^ r0;   /* Exec tc2 = z10 ^ tc1 into b1       */
    r15 = *b1 ^ *b3; /* Exec tc21 = tc2 ^ z11 into r15     */
    r2 = r2 ^ *b1;   /* Exec tc3 = z9 ^ tc2 into r2        */
    *b0 = r2 ^ *b5;  /* Exec S0 = tc3 ^ tc16 into b0       */
    *b3 = r2 ^ r13;  /* Exec S3 = tc3 ^ tc11 into b3       */
    *b1 = *b3 ^ *b5; /* Exec S1 = S3 ^ tc16 ^ 1 into b1    */
    r0 = *b6 ^ r0;   /* Exec tc13 = z13 ^ tc1 into r0      */
    *b5 = r7 & *b7;  /* Exec z2 = t33 & U7 into b5         */
    r14 = r4 ^ *b5;  /* Exec tc4 = z0 ^ z2 into r14        */
    *b6 = r1 ^ r14;  /* Exec tc7 = z12 ^ tc4 into b6       */
    *b6 = r5 ^ *b6;  /* Exec tc9 = z8 ^ tc7 into b6        */
    *b6 = *b4 ^ *b6; /* Exec tc10 = tc8 ^ tc9 into b6      */
    *b2 = r17 ^ *b6; /* Exec tc17 = z14 ^ tc10 into b2     */
    *b5 = r15 ^ *b2; /* Exec S5 = tc21 ^ tc17 into b5      */
    *b2 = *b2 ^ r6;  /* Exec tc26 = tc17 ^ tc20 into b2    */
    *b2 = *b2 ^ r10; /* Exec S2 = tc26 ^ z17 ^ 1 into b2   */
    r14 = r14 ^ r11; /* Exec tc14 = tc4 ^ tc12 into r14    */
    r0 = r0 ^ r14;   /* Exec tc18 = tc13 ^ tc14 into r0    */
    *b6 = *b6 ^ r0;  /* Exec S6 = tc10 ^ tc18 ^ 1 into b6  */
    *b7 = r1 ^ r0;   /* Exec S7 = z12 ^ tc18 ^ 1 into b7   */
    *b4 = r14 ^ *b3; /* Exec S4 = tc14 ^ S3 into b4        */
    // [('b0', 'S0'), ('b1', 'S1'), ('b2', 'S2'), ('b3', 'S3'), ('b4',
    // 'S4'),
    // ('b5', 'S5'), ('b6', 'S6'), ('b7', 'S7'), ('r0', 'tc18'), ('r1',
    // 'z12'),
    // ('r2', 'tc3'), ('r3', 'z10'), ('r4', 'z0'), ('r5', 'z8'), ('r6',
    // 'tc20'),
    // ('r7', 't33'), ('r8', 'y9'), ('r9', 'y11'), ('r10', 'z17'), ('r11',
    // 'tc12'), ('r12', 't44'), ('r13', 'tc11'), ('r14', 'tc14'), ('r15',
    // 'tc21'), ('r16', 't35'), ('r17', 'z14')]
}

//    b0: register to apply shiftrows on
// m0-m5: masks to extract bits, const
inline static void __attribute__((always_inline))
shiftrow(uint32_t *b0, const uint32_t mask0, const uint32_t mask1,
         const uint32_t mask2, const uint32_t mask3, const uint32_t mask4,
         const uint32_t mask5) {

    // compact version:
    uint32_t r0;
    r0 = *b0 & 0xff;
    r0 = r0 | ((*b0 & mask0) << 2);
    r0 = r0 | ((*b0 & mask1) >> 6);
    r0 = r0 | ((*b0 & mask2) << 4);
    r0 = r0 | ((*b0 & mask3) >> 4);
    r0 = r0 | ((*b0 & mask4) << 6);
    *b0 = r0 | ((*b0 & mask5) >> 2);
}

inline static void __attribute__((always_inline))
shiftrows(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3, uint32_t *b4,
          uint32_t *b5, uint32_t *b6, uint32_t *b7) {

    const uint32_t r0 = 0x00003f00;
    const uint32_t r1 = 0x0000c000;
    const uint32_t r2 = 0x000f0000;
    const uint32_t r3 = 0x00f00000;
    const uint32_t r4 = 0x03000000;
    const uint32_t r5 = 0xfc000000;

    shiftrow(b0, r0, r1, r2, r3, r4, r5);
    shiftrow(b1, r0, r1, r2, r3, r4, r5);
    shiftrow(b2, r0, r1, r2, r3, r4, r5);
    shiftrow(b3, r0, r1, r2, r3, r4, r5);
    shiftrow(b4, r0, r1, r2, r3, r4, r5);
    shiftrow(b5, r0, r1, r2, r3, r4, r5);
    shiftrow(b6, r0, r1, r2, r3, r4, r5);
    shiftrow(b7, r0, r1, r2, r3, r4, r5);
}

//    o0: output (can overlap with input)
//    i0: input, const
//    c0: rotate distance, immediate
inline static void __attribute__((always_inline))
ror(uint32_t *o0, const uint32_t *i0, const int imm) {
    uint32_t r0 = *i0 >> imm;
    uint32_t r1 = *i0 << (32 - imm);
    *o0 = r0 | r1;
}

// b0-b7: state
// r0-r8: tmp
inline static void __attribute__((always_inline))
mixcolumns(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3, uint32_t *b4,
           uint32_t *b5, uint32_t *b6, uint32_t *b7) {
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7;
    ror(&r0, b0, 8);
    ror(&r1, b1, 8);
    ror(&r2, b2, 8);
    ror(&r3, b3, 8);
    ror(&r4, b4, 8);
    ror(&r5, b5, 8);
    ror(&r6, b6, 8);
    ror(&r7, b7, 8);

    *b0 = *b0 ^ r0;
    *b1 = *b1 ^ r1;
    *b2 = *b2 ^ r2;
    *b3 = *b3 ^ r3;
    *b4 = *b4 ^ r4;
    *b5 = *b5 ^ r5;
    *b6 = *b6 ^ r6;
    *b7 = *b7 ^ r7;

    r0 = r0 ^ *b1;
    r1 = r1 ^ *b2;
    r2 = r2 ^ *b3;
    r3 = r3 ^ *b0;
    r3 = r3 ^ *b4;
    r4 = r4 ^ *b0;
    r4 = r4 ^ *b5;
    r5 = r5 ^ *b6;
    r6 = r6 ^ *b0;
    r6 = r6 ^ *b7;
    r7 = r7 ^ *b0;

    ror(b0, b0, 16);
    ror(b1, b1, 16);
    ror(b2, b2, 16);
    ror(b3, b3, 16);
    ror(b4, b4, 16);
    ror(b5, b5, 16);
    ror(b6, b6, 16);
    ror(b7, b7, 16);

    *b0 = *b0 ^ r0;
    *b1 = *b1 ^ r1;
    *b2 = *b2 ^ r2;
    *b3 = *b3 ^ r3;
    *b4 = *b4 ^ r4;
    *b5 = *b5 ^ r5;
    *b6 = *b6 ^ r6;
    *b7 = *b7 ^ r7;
}

//  b0-b7: state
//     c0: pointer to the round keys, const
inline static void __attribute__((always_inline))
round_C(uint32_t *b0, uint32_t *b1, uint32_t *b2, uint32_t *b3, uint32_t *b4,
        uint32_t *b5, uint32_t *b6, uint32_t *b7, const uint32_t *c0) {
    subbytes(b0, b1, b2, b3, b4, b5, b6, b7);
    shiftrows(b0, b1, b2, b3, b4, b5, b6, b7);
    mixcolumns(b0, b1, b2, b3, b4, b5, b6, b7);
    addroundkey(b0, b1, b2, b3, b4, b5, b6, b7, c0);
}

void AES_128_encrypt_ctr_C(const param *p, const uint8_t *in, uint8_t *out,
                           uint32_t len) {
    while (len > 0) {
        uint32_t state0 = ((uint32_t *)p->nonce)[0];
        uint32_t state1 = ((uint32_t *)p->nonce)[1];
        uint32_t state2 = ((uint32_t *)p->nonce)[2];
        uint32_t state3 = ((uint32_t *)p->ctr)[0];

        uint32_t state4 = state0;
        uint32_t state5 = state1;
        uint32_t state6 = state2;
        uint32_t state7 = state3;
        // reverse, increase ctr, reverse
        revbytes(&state7);
        state7 += 1;
        revbytes(&state7);

        bitslice(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                 &state7);
        addroundkey(&state0, &state1, &state2, &state3, &state4, &state5,
                    &state6, &state7, (uint32_t *)p->rk);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 8);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 16);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 24);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 32);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 40);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 48);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 56);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 64);
        round_C(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                &state7, (uint32_t *)p->rk + 72);
        subbytes(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                 &state7);
        shiftrows(&state0, &state1, &state2, &state3, &state4, &state5, &state6,
                  &state7);
        addroundkey(&state0, &state1, &state2, &state3, &state4, &state5,
                    &state6, &state7, (uint32_t *)p->rk + 80);
        unbitslice(&state0, &state1, &state2, &state3, &state4, &state5,
                   &state6, &state7);

        ((uint32_t *)out)[0] = ((uint32_t *)in)[0] ^ state0;
        ((uint32_t *)out)[1] = ((uint32_t *)in)[1] ^ state1;
        ((uint32_t *)out)[2] = ((uint32_t *)in)[2] ^ state2;
        ((uint32_t *)out)[3] = ((uint32_t *)in)[3] ^ state3;
        ((uint32_t *)out)[4] = ((uint32_t *)in)[4] ^ state4;
        ((uint32_t *)out)[5] = ((uint32_t *)in)[5] ^ state5;
        ((uint32_t *)out)[6] = ((uint32_t *)in)[6] ^ state6;
        ((uint32_t *)out)[7] = ((uint32_t *)in)[7] ^ state7;
        in = in + 32;
        out = out + 32;
        // load ctr
        state3 = ((uint32_t *)p->ctr)[0];

        // decrease len
        len -= 32;

        // update ctr
        revbytes(&state3);
        state3 += 2;
        revbytes(&state3);
        ((uint32_t *)p->ctr)[0] = state3;
    }
}