#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/tinyjambu.h"

#define crypto_aead_encrypt tiny_jambu_128_aead_encrypt
#define crypto_aead_decrypt tiny_jambu_128_aead_decrypt
#define crypto_aead_encrypt_2 tiny_jambu_192_aead_encrypt
#define crypto_aead_decrypt_2 tiny_jambu_192_aead_decrypt
#define crypto_aead_encrypt_3 tiny_jambu_256_aead_encrypt
#define crypto_aead_decrypt_3 tiny_jambu_256_aead_decrypt
