#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/spoc.h"

#define crypto_aead_encrypt spoc_128_aead_encrypt
#define crypto_aead_decrypt spoc_128_aead_decrypt
#define crypto_aead_encrypt_2 spoc_64_aead_encrypt
#define crypto_aead_decrypt_2 spoc_64_aead_decrypt