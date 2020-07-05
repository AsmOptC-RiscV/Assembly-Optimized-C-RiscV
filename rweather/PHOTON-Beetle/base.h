#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/photon-beetle.h"

#define crypto_aead_encrypt photon_beetle_128_aead_encrypt
#define crypto_aead_decrypt photon_beetle_128_aead_decrypt
#define crypto_aead_encrypt_2 photon_beetle_32_aead_encrypt
#define crypto_aead_decrypt_2 photon_beetle_32_aead_decrypt
#define crypto_hash photon_beetle_hash
