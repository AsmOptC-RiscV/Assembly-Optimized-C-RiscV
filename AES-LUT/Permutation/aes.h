#pragma once

#include <stdint.h>

void AES_128_keyschedule(const uint8_t *, uint8_t *);
void AES_128_encrypt(const uint8_t *, const uint8_t *, uint8_t *);

void AES_128_keyschedule_C(const uint8_t *, uint8_t *);
void AES_128_encrypt_C(const uint8_t *, const uint8_t *, uint8_t *);