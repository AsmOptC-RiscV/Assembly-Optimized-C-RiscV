#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/comet.h"

// need to check for comet_64_cham_aead
#define crypto_aead_encrypt comet_128_cham_aead_encrypt
#define crypto_aead_decrypt comet_128_cham_aead_decrypt
#define crypto_aead_encrypt_2 comet_64_speck_aead_encrypt
#define crypto_aead_decrypt_2 comet_64_speck_aead_decrypt
#define crypto_aead_encrypt_3 comet_64_cham_aead_encrypt
#define crypto_aead_decrypt_3 comet_64_cham_aead_decrypt
