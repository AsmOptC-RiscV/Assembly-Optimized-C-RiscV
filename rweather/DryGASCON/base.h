#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/drygascon.h"

#define crypto_aead_encrypt drygascon128_aead_encrypt
#define crypto_aead_decrypt drygascon128_aead_decrypt
#define crypto_aead_encrypt_2 drygascon256_aead_encrypt
#define crypto_aead_decrypt_2 drygascon256_aead_decrypt
#define crypto_hash drygascon128_hash
#define crypto_hash_2 drygascon256_hash