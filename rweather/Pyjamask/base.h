#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/pyjamask.h"

#define crypto_aead_encrypt pyjamask_128_aead_encrypt
#define crypto_aead_decrypt pyjamask_128_aead_decrypt
#define crypto_aead_encrypt_2 pyjamask_masked_128_aead_encrypt
#define crypto_aead_decrypt_2 pyjamask_masked_128_aead_decrypt
#define crypto_aead_encrypt_3 pyjamask_96_aead_encrypt
#define crypto_aead_decrypt_3 pyjamask_96_aead_decrypt
#define crypto_aead_encrypt_4 pyjamask_masked_96_aead_encrypt
#define crypto_aead_decrypt_4 pyjamask_masked_96_aead_decrypt
