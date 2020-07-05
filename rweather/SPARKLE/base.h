#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/sparkle.h"

#define crypto_aead_encrypt schwaemm_256_128_aead_encrypt
#define crypto_aead_decrypt schwaemm_256_128_aead_decrypt
#define crypto_aead_encrypt_2 schwaemm_128_128_aead_encrypt
#define crypto_aead_decrypt_2 schwaemm_128_128_aead_decrypt
#define crypto_aead_encrypt_3 schwaemm_192_192_aead_encrypt
#define crypto_aead_decrypt_3 schwaemm_192_192_aead_decrypt
#define crypto_aead_encrypt_4 schwaemm_256_256_aead_encrypt
#define crypto_aead_decrypt_4 schwaemm_256_256_aead_decrypt
#define crypto_hash esch_256_hash
#define crypto_hash_2 esch_384_hash
