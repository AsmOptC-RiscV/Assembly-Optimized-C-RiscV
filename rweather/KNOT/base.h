#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/knot.h"

#define crypto_aead_encrypt knot_aead_128_256_encrypt
#define crypto_aead_decrypt knot_aead_128_256_decrypt
#define crypto_aead_encrypt_2 knot_aead_128_384_encrypt
#define crypto_aead_decrypt_2 knot_aead_128_384_decrypt
#define crypto_aead_encrypt_3 knot_aead_192_384_encrypt
#define crypto_aead_decrypt_3 knot_aead_192_384_decrypt
#define crypto_aead_encrypt_4 knot_aead_256_512_encrypt
#define crypto_aead_decrypt_4 knot_aead_256_512_decrypt
#define crypto_hash knot_hash_256_256
#define crypto_hash_2 knot_hash_256_384
#define crypto_hash_3 knot_hash_384_384
#define crypto_hash_4 knot_hash_512_512