#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/skinny-aead.h"
#include "src/skinny-hash.h"

#define crypto_aead_encrypt skinny_aead_m1_encrypt
#define crypto_aead_decrypt skinny_aead_m1_decrypt
#define crypto_aead_encrypt_2 skinny_aead_m2_encrypt
#define crypto_aead_decrypt_2 skinny_aead_m2_decrypt
#define crypto_aead_encrypt_3 skinny_aead_m3_encrypt
#define crypto_aead_decrypt_3 skinny_aead_m3_decrypt
#define crypto_aead_encrypt_4 skinny_aead_m4_encrypt
#define crypto_aead_decrypt_4 skinny_aead_m4_decrypt
#define crypto_aead_encrypt_5 skinny_aead_m5_encrypt
#define crypto_aead_decrypt_5 skinny_aead_m5_decrypt
#define crypto_aead_encrypt_6 skinny_aead_m6_encrypt
#define crypto_aead_decrypt_6 skinny_aead_m6_decrypt
#define crypto_hash skinny_tk3_hash
#define crypto_hash_2 skinny_tk2_hash
