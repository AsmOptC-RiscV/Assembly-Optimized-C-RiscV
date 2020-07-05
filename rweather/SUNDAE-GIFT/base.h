#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/sundae-gift.h"

#define crypto_aead_encrypt sundae_gift_0_aead_encrypt
#define crypto_aead_decrypt sundae_gift_0_aead_decrypt
#define crypto_aead_encrypt_2 sundae_gift_64_aead_encrypt
#define crypto_aead_decrypt_2 sundae_gift_64_aead_decrypt
#define crypto_aead_encrypt_3 sundae_gift_96_aead_encrypt
#define crypto_aead_decrypt_3 sundae_gift_96_aead_decrypt
#define crypto_aead_encrypt_4 sundae_gift_128_aead_encrypt
#define crypto_aead_decrypt_4 sundae_gift_128_aead_decrypt
