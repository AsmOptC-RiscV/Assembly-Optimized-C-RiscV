#include "interleaving.h"

// Interleaves a 8 bit int with zeros into a 16 bit int
inline static uint16_t __attribute__((always_inline))
interleave_uint8_with_zeros(const uint8_t input) {
    uint16_t word = input;
    word = (word ^ (word << 4)) & 0x0f0f;
    word = (word ^ (word << 2)) & 0x3333;
    word = (word ^ (word << 1)) & 0x5555;
    word = word << 1;
    return word;
}

// Interleaves a 16 bit int with zeros into a 32 bit int
inline static uint32_t __attribute__((always_inline))
interleave_uint16_with_zeros(const uint16_t input) {
    uint32_t word = input;
    word = (word ^ (word << 8)) & 0x00ff00ff;
    word = (word ^ (word << 4)) & 0x0f0f0f0f;
    word = (word ^ (word << 2)) & 0x33333333;
    word = (word ^ (word << 1)) & 0x55555555;
    word = word << 1;

    return word;
}

inline static uint32_t __attribute__((always_inline))
interleave_4_bytes(const uint8_t first, const uint8_t second,
                   const uint8_t third, const uint8_t fourth) {
    // clang-format off
    uint32_t ret = interleave_uint16_with_zeros(interleave_uint8_with_zeros(first));
    ret |= interleave_uint16_with_zeros(interleave_uint8_with_zeros(second)) >> 1;
    ret |= interleave_uint16_with_zeros(interleave_uint8_with_zeros(third)) >> 2;
    ret |= interleave_uint16_with_zeros(interleave_uint8_with_zeros(fourth)) >> 3;
    // clang-format on
    return ret;
}

void interleave_4_blocks(uint32_t *output, const uint8_t *input1,
                         const uint8_t *input2, const uint8_t *input3,
                         const uint8_t *input4, const SIZE size) {
    for (SIZE i = 0; i < size; i++) {
        output[i] =
            interleave_4_bytes(input1[i], input2[i], input3[i], input4[i]);
    }
}

inline static uint16_t __attribute__((always_inline))
deinterleave_uint32_part(const uint32_t x) {
    uint32_t y = x & 0x55555555;
    y = (y | (y >> 1)) & 0x33333333;
    y = (y | (y >> 2)) & 0x0F0F0F0F;
    y = (y | (y >> 4)) & 0x00FF00FF;
    y = (y | (y >> 8)) & 0x0000FFFF;
    return (uint16_t)y;
}

inline static uint8_t __attribute__((always_inline))
deinterleave_uint16_part(const uint16_t x) {
    uint32_t y = x & 0x55555555;
    y = (y | (y >> 1)) & 0x3333;
    y = (y | (y >> 2)) & 0x0F0F;
    y = (y | (y >> 4)) & 0x00FF;

    return (uint8_t)y;
}

inline static void __attribute__((always_inline))
deinterleave_uint16(uint8_t *part1, uint8_t *part2, const uint16_t z) {
    *part2 = deinterleave_uint32_part(z);
    *part1 = deinterleave_uint32_part(z >> 1);
}

inline static void __attribute__((always_inline))
deinterleave_uint32_to_uint16(uint16_t *part1, uint16_t *part2,
                              const uint32_t z) {
    *part2 = deinterleave_uint32_part(z);
    *part1 = deinterleave_uint32_part(z >> 1);
}

inline static void __attribute__((always_inline))
deinterleave_uint32_to_bytes(uint8_t *output1, uint8_t *output2,
                             uint8_t *output3, uint8_t *output4,
                             const uint32_t input) {
    uint16_t intermediate1;
    uint16_t intermediate2;

    deinterleave_uint32_to_uint16(&intermediate1, &intermediate2, input);
    deinterleave_uint16(output1, output3, intermediate1);
    deinterleave_uint16(output2, output4, intermediate2);
}

void deinterleave_uint32_to_blocks(uint8_t *output1, uint8_t *output2,
                                   uint8_t *output3, uint8_t *output4,
                                   const uint32_t *input, const SIZE size) {

    for (SIZE i = 0; i < size; i++) {
        deinterleave_uint32_to_bytes(&output1[i], &output2[i], &output3[i],
                                     &output4[i], input[i]);
    }
}