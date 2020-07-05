#pragma once

#define HASH

#define crypto_hash crypto_hash
#define crypto_hash_2 crypto_hash_asm
// #define crypto_hash_3
// #define crypto_hash_4

#define crypto_aead_encrypt crypto_aead_encrypt
#define crypto_aead_encrypt_2 crypto_aead_encrypt_asm
#define crypto_aead_encrypt_3 crypto_aead_encrypt_bs32x
// #define crypto_aead_encrypt_4 

#define crypto_aead_decrypt crypto_aead_decrypt
#define crypto_aead_decrypt_2 crypto_aead_decrypt_asm
#define crypto_aead_decrypt_3 crypto_aead_decrypt_bs32x
// #define crypto_aead_decrypt_4

#include "crypto_aead/crypto_aead.h"
#include "crypto_hash/crypto_hash.h"
#include "kat.h"
