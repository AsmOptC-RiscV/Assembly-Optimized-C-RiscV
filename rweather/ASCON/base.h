#pragma once

// #define ADDITIONAL

#include "kat.h"
#include "src/ascon128.h"

#define crypto_hash ascon_hash
// #define crypto_hash_2 ascon_hash
// #define crypto_hash_3 ascon_hash
// #define crypto_hash_4 ascon_hash

#define crypto_aead_encrypt ascon128_aead_encrypt
#define crypto_aead_encrypt_2 ascon128a_aead_encrypt
#define crypto_aead_encrypt_3 ascon80pq_aead_encrypt
// #define crypto_aead_encrypt_4 crypto_aead_encrypt

#define crypto_aead_decrypt ascon128_aead_decrypt
#define crypto_aead_decrypt_2 ascon128a_aead_decrypt
#define crypto_aead_decrypt_3 ascon80pq_aead_decrypt
// #define crypto_aead_decrypt_4 crypto_aead_decrypt

#define NO_CHECK