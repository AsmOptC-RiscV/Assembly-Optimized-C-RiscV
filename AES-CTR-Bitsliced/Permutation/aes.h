#pragma once

#include <stdint.h>

typedef struct param {
    uint8_t nonce[12];
    uint8_t ctr[4];
    uint8_t rk[2 * 11 * 16];
} param;

extern uint32_t AESTable[1024];

extern void AES_128_keyschedule(const uint8_t *, uint8_t *);
extern void AES_128_encrypt_ctr(param const *, const uint8_t *, uint8_t *,
                                uint32_t);

void AES_128_keyschedule_C(const uint8_t *, uint8_t *);
void AES_128_encrypt_ctr_C(param const *, const uint8_t *, uint8_t *, uint32_t);