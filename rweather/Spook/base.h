#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/spook.h"

#define crypto_aead_encrypt spook_128_512_su_aead_encrypt
#define crypto_aead_decrypt spook_128_512_su_aead_decrypt
#define crypto_aead_encrypt_2 spook_128_384_su_aead_encrypt
#define crypto_aead_decrypt_2 spook_128_384_su_aead_decrypt
#define crypto_aead_encrypt_3 spook_128_512_mu_aead_encrypt
#define crypto_aead_decrypt_3 spook_128_512_mu_aead_decrypt
#define crypto_aead_encrypt_4 spook_128_384_mu_aead_encrypt
#define crypto_aead_decrypt_4 spook_128_384_mu_aead_decrypt
