#include <stdint.h>

typedef unsigned long long SIZE;

void interleave_4_blocks(uint32_t *output, const uint8_t *input1,
                         const uint8_t *input2, const uint8_t *input3,
                         const uint8_t *input4, const SIZE size);

void deinterleave_uint32_to_blocks(uint8_t *output1, uint8_t *output2,
                                   uint8_t *output3, uint8_t *output4,
                                   const uint32_t *input, const SIZE size);
