/**
 * Based on the implementation by the Keccak Team, namely, Guido Bertoni, Joan
 * Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.
 */

#include "keccak.h"
#define index(x, y) (((x) % 5) + 5 * ((y) % 5))

#define ROL32(a, offset)                                                       \
    ((offset != 0) ? ((((uint32_t)a) << offset) ^                              \
                      (((uint32_t)a) >> (sizeof(uint8_t) * 32 - offset)))      \
                   : a)

uint32_t KeccakRoundConstants_4[maxNrRounds] = {
    0xf,        0xf00000f0, 0xf000f0f0, 0x0,        0xf000f0ff, 0xf,
    0xf000000f, 0xf00f,     0xf000f0f0, 0xf000f000, 0xf00f,     0xf0f0,
    0xf000f0ff, 0xf000f0ff, 0xf000f00f, 0xff,       0xf0,       0xf0000000};

uint32_t KeccakRhoOffsets[nrLanes] = {0,  4,  24, 16, 12, 16, 16, 24, 28,
                                      16, 12, 8,  12, 4,  28, 4,  20, 28,
                                      20, 0,  8,  8,  20, 0,  24};

inline static void __attribute__((always_inline)) theta_4(uint32_t *A) {
    unsigned int x, y;
    uint32_t C[5], D[5];

    for (x = 0; x < 5; x++) {
        C[x] = 0;
        for (y = 0; y < 5; y++)
            C[x] ^= A[index(x, y)];
    }
    for (x = 0; x < 5; x++)
        D[x] = ROL32(C[(x + 1) % 5], 4) ^ C[(x + 4) % 5];
    for (x = 0; x < 5; x++)
        for (y = 0; y < 5; y++)
            A[index(x, y)] ^= D[x];
}

inline static void __attribute__((always_inline)) rho_4(uint32_t *A) {
    for (unsigned int x = 0; x < 5; x++)
        for (unsigned int y = 0; y < 5; y++)
            A[index(x, y)] =
                ROL32(A[index(x, y)], KeccakRhoOffsets[index(x, y)]);
}

inline static void __attribute__((always_inline)) pi_4(uint32_t *A) {
    uint32_t x = A[1];
    A[1] = A[6];
    A[6] = A[9];
    A[9] = A[22];
    A[22] = A[14];
    A[14] = A[20];
    A[20] = A[2];
    A[2] = A[12];
    A[12] = A[13];
    A[13] = A[19];
    A[19] = A[23];
    A[23] = A[15];
    A[15] = A[4];
    A[4] = A[24];
    A[24] = A[21];
    A[21] = A[8];
    A[8] = A[16];
    A[16] = A[5];
    A[5] = A[3];
    A[3] = A[18];
    A[18] = A[17];
    A[17] = A[11];
    A[11] = A[7];
    A[7] = A[10];
    A[10] = x;
    // uint32_t tempA[25];

    // for (unsigned int x = 0; x < 5; x++)
    //     for (unsigned int y = 0; y < 5; y++)
    //         tempA[index(x, y)] = A[index(x, y)];
    // for (unsigned int x = 0; x < 5; x++)
    //     for (unsigned int y = 0; y < 5; y++)
    //         A[index(0 * x + 1 * y, 2 * x + 3 * y)] = tempA[index(x, y)];
}

inline static void __attribute__((always_inline)) chi_4(uint32_t *A) {
    unsigned int x, y;
    uint32_t C[5];

    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++)
            C[x] =
                A[index(x, y)] ^ ((~A[index(x + 1, y)]) & A[index(x + 2, y)]);
        for (x = 0; x < 5; x++)
            A[index(x, y)] = C[x];
    }
}

inline static void __attribute__((always_inline))
iota_4(uint32_t *A, unsigned int indexRound) {
    A[index(0, 0)] ^= KeccakRoundConstants_4[indexRound];
}

inline static void __attribute__((always_inline))
KeccakP200Round_4(uint32_t *state, unsigned int indexRound) {
    theta_4(state);
    rho_4(state);
    pi_4(state);
    chi_4(state);
    iota_4(state, indexRound);
}

void permutation_4(uint32_t *state) {
    for (unsigned int i = 0; i < maxNrRounds; i++)
        KeccakP200Round_4(state, i);
}
