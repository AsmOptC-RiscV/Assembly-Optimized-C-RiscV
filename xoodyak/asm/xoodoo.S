.text
.align 1

// rotate left: tmp0 = reg <<< dist
// info: 2 tmp register, because we dont want to overwrite reg,
// otherwise only 1 tmp reg would be needed
.macro rol_tmp tmp0,tmp1,reg,val
    slli    \tmp0, \reg, \val
    srli    \tmp1, \reg, 32-\val
    xor     \tmp0, \tmp0, \tmp1
.endm

// rotate right: reg = reg >>> dist
.macro rol reg,tmp0,val
    slli    \tmp0, \reg, \val
    srli    \reg, \reg, 32-\val
    xor     \reg, \reg, \tmp0
.endm


// t0, t1, t2, t3, t4
.macro theta x00,x01,x02,x03,x10,x11,x12,x13,x20,x21,x22,x23

    xor     t0, \x03, \x13
    xor     t0, t0, \x23
    xor     t1, \x00, \x10
    xor     t1, t1, \x20

    rol_tmp t2,t4,t0,5
    rol_tmp t3,t4,t0,14
    xor     t0, t2, t3

    // sheet 0
    xor     \x00, \x00, t0
    xor     \x10, \x10, t0
    xor     \x20, \x20, t0

    xor     t0, \x01, \x11
    xor     t0, t0, \x21

    rol_tmp t2,t4,t1,5
    rol_tmp t3,t4,t1,14
    xor     t1, t2, t3

    // sheet 1
    xor     \x01, \x01, t1
    xor     \x11, \x11, t1
    xor     \x21, \x21, t1

    xor     t1, \x02, \x12
    xor     t1, t1, \x22

    rol_tmp t2,t4,t0,5
    rol_tmp t3,t4,t0,14
    xor     t0, t2, t3

    // sheet 2
    xor     \x02, \x02, t0
    xor     \x12, \x12, t0
    xor     \x22, \x22, t0

    rol_tmp t2,t4,t1,5
    rol_tmp t3,t4,t1,14
    xor     t1, t2, t3

    // sheet 3
    xor     \x03, \x03, t1
    xor     \x13, \x13, t1
    xor     \x23, \x23, t1

.endm


// t0
.macro rho_west x10,x11,x12,x13,x20,x21,x22,x23

    addi    t0,   \x13, 0
    addi    \x13, \x12, 0
    addi    \x12, \x11, 0
    addi    \x11, \x10, 0
    addi    \x10, t0,   0

    rol     \x20, t0, 11
    rol     \x21, t0, 11
    rol     \x22, t0, 11
    rol     \x23, t0, 11

.endm


// t0
.macro iota x00,rc

    li      t0, \rc
    xor     \x00, \x00, t0

.endm


// 1.
// t0
.macro chi_pattern_2 x00,x10,x20
    // x00 = x00 ^ (x10 & x20);
    and     t0, \x10, \x20
    xor     \x00, \x00, t0
    // x10 = x10 ^ (x20 | x00);
    or      t0, \x20, \x00
    xor     \x10, \x10, t0
    //x20 = x20 ^ (x00 & x10);
    and     t0, \x00, \x10
    xor     \x20, \x20, t0
.endm

// 1.
// t0, t1
.macro chi_pattern_4 x00,x10,x20
    // x00 = x00 ^ (~x10 & x20);
    xori    t0, \x10, -1
    and     t0, t0, \x20
    xor     \x00, \x00, t0
    // x10 = x10 ^ (x20 | x00);
    or      t0, \x20, \x00
    xor     \x10, \x10, t0
    // x20 = x20 ^ (x00 & ~x10);
    xori    t0, \x10, -1
    and     t0, \x00, t0
    xor     \x20, \x20, t0
.endm

/*
// t0
.macro chi x00,x01,x02,x03,x10,x11,x12,x13,x20,x21,x22,x23

    // lane 0
    xori    t0, \x10, -1
    and     t0, t0, \x20
    xor     \x00, \x00, t0

    xori    t0, \x20, -1
    and     t0, t0, \x00
    xor     \x10, \x10, t0

    xori    t0, \x00, -1
    and     t0, t0, \x10
    xor     \x20, \x20, t0

    // lane 1
    xori    t0, \x11, -1
    and     t0, t0, \x21
    xor     \x01, \x01, t0

    xori    t0, \x21, -1
    and     t0, t0, \x01
    xor     \x11, \x11, t0

    xori    t0, \x01, -1
    and     t0, t0, \x11
    xor     \x21, \x21, t0

    // lane 2
    xori    t0, \x12, -1
    and     t0, t0, \x22
    xor     \x02, \x02, t0

    xori    t0, \x22, -1
    and     t0, t0, \x02
    xor     \x12, \x12, t0

    xori    t0, \x02, -1
    and     t0, t0, \x12
    xor     \x22, \x22, t0

    // lane 3
    xori    t0, \x13, -1
    and     t0, t0, \x23
    xor     \x03, \x03, t0

    xori    t0, \x23, -1
    and     t0, t0, \x03
    xor     \x13, \x13, t0

    xori    t0, \x03, -1
    and     t0, t0, \x13
    xor     \x23, \x23, t0

.endm
*/

// t0, t1, t2
.macro rho_east x10,x11,x12,x13,x20,x21,x22,x23

    rol     \x10, t0, 1
    rol     \x11, t0, 1
    rol     \x12, t0, 1
    rol     \x13, t0, 1

    addi    t0, \x23, 0
    addi    t1, \x22, 0

    rol_tmp \x23, t2, \x21, 8
    rol_tmp \x21, t2, t0,  8
    rol_tmp \x22, t2, \x20, 8
    rol_tmp \x20, t2, t1,  8

.endm

/*
.macro round rc

    theta       a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11

    rho_west    a6,a7,t5,t6,s8,s9,s10,s11

    iota        a2, \rc

    chi         a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11

    rho_east    a6,a7,t5,t6,s8,s9,s10,s11

.endm
*/

// Xoodoo permutation
// void func_xoodoo_asm(unsigned int *state, unsigned int ns)
.globl func_xoodoo_asm
.type func_xoodoo_asm,%function

// a0 &state
// sp, ra
// 12 state
// 5 temp
// -> 20 register
func_xoodoo_asm: // input: a0:&state
    addi        sp, sp, -20     // allocate stack frame
    // store callee saved register
    sw          s8,  4(sp)
    sw          s9,  8(sp)
    sw          s10, 12(sp)
    sw          s11, 16(sp)

    // load complete state from stack
    lw          a2, 0(a0)       // load x00
    lw          a3, 4(a0)       // load x01
    lw          a4, 8(a0)       // load x02
    lw          a5, 12(a0)      // load x03
    lw          a6, 16(a0)      // load x10
    lw          a7, 20(a0)      // load x11
    lw          t5, 24(a0)      // load x12
    lw          t6, 28(a0)      // load x13
    lw          s8, 32(a0)      // load x20
    lw          s9, 36(a0)      // load x21
    lw          s10, 40(a0)     // load x22
    lw          s11, 44(a0)     // load x23

    /*round 0x00000058
    round 0x00000038
    round 0x000003C0
    round 0x000000D0
    round 0x00000120
    round 0x00000014
    round 0x00000060
    round 0x0000002C
    round 0x00000380
    round 0x000000F0
    round 0x000001A0
    round 0x00000012*/



    // Code generated

    // input mask
    not             a2, a2
    not             a3, a3
    not             a4, a4
    not             a5, a5

    // round 0
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000058
    chi_pattern_4   a2,a6,s8
    chi_pattern_4   a3,a7,s9
    chi_pattern_4   a4,t5,s10
    chi_pattern_4   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 1
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000038
    chi_pattern_2   a2,a6,s8
    chi_pattern_2   a3,a7,s9
    chi_pattern_2   a4,t5,s10
    chi_pattern_2   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 2
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x000003C0
    chi_pattern_4   a2,a6,s8
    chi_pattern_4   a3,a7,s9
    chi_pattern_4   a4,t5,s10
    chi_pattern_4   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 3
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x000000D0
    chi_pattern_2   a2,a6,s8
    chi_pattern_2   a3,a7,s9
    chi_pattern_2   a4,t5,s10
    chi_pattern_2   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 4
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000120
    chi_pattern_4   a2,a6,s8
    chi_pattern_4   a3,a7,s9
    chi_pattern_4   a4,t5,s10
    chi_pattern_4   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 5
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000014
    chi_pattern_2   a2,a6,s8
    chi_pattern_2   a3,a7,s9
    chi_pattern_2   a4,t5,s10
    chi_pattern_2   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 6
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000060
    chi_pattern_4   a2,a6,s8
    chi_pattern_4   a3,a7,s9
    chi_pattern_4   a4,t5,s10
    chi_pattern_4   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 7
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x0000002C
    chi_pattern_2   a2,a6,s8
    chi_pattern_2   a3,a7,s9
    chi_pattern_2   a4,t5,s10
    chi_pattern_2   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 8
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000380
    chi_pattern_4   a2,a6,s8
    chi_pattern_4   a3,a7,s9
    chi_pattern_4   a4,t5,s10
    chi_pattern_4   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 9
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x000000F0
    chi_pattern_2   a2,a6,s8
    chi_pattern_2   a3,a7,s9
    chi_pattern_2   a4,t5,s10
    chi_pattern_2   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 10
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x000001A0
    chi_pattern_4   a2,a6,s8
    chi_pattern_4   a3,a7,s9
    chi_pattern_4   a4,t5,s10
    chi_pattern_4   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // round 11
    theta           a2,a3,a4,a5,a6,a7,t5,t6,s8,s9,s10,s11
    rho_west        a6,a7,t5,t6,s8,s9,s10,s11
    iota            a2, 0x00000012
    chi_pattern_2   a2,a6,s8
    chi_pattern_2   a3,a7,s9
    chi_pattern_2   a4,t5,s10
    chi_pattern_2   a5,t6,s11
    rho_east        a6,a7,t5,t6,s8,s9,s10,s11

    // output mask
    not             a2, a2
    not             a3, a3
    not             a4, a4
    not             a5, a5


    // save complete state to stack
    sw          a2, 0(a0)       // save x00
    sw          a3, 4(a0)       // save x01
    sw          a4, 8(a0)       // save x02
    sw          a5, 12(a0)      // save x03
    sw          a6, 16(a0)      // save x10
    sw          a7, 20(a0)      // save x11
    sw          t5, 24(a0)      // save x12
    sw          t6, 28(a0)      // save x13
    sw          s8, 32(a0)      // save x20
    sw          s9, 36(a0)      // save x21
    sw          s10, 40(a0)     // save x22
    sw          s11, 44(a0)     // save x23

    // load callee saved register
    lw          s8,  4(sp)      //state
    lw          s9,  8(sp)
    lw          s10, 12(sp)
    lw          s11, 16(sp)
    addi        sp, sp, 20      // deallocate stack frame
    ret
