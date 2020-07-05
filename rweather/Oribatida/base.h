#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/oribatida.h"

#define crypto_aead_encrypt oribatida_256_aead_encrypt
#define crypto_aead_decrypt oribatida_256_aead_decrypt
#define crypto_aead_encrypt_2 oribatida_192_aead_encrypt
#define crypto_aead_decrypt_2 oribatida_192_aead_decrypt
