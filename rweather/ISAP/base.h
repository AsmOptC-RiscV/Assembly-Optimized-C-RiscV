#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/isap.h"

#define crypto_aead_encrypt isap_keccak_128a_aead_encrypt
#define crypto_aead_decrypt isap_keccak_128a_aead_decrypt
#define crypto_aead_encrypt_2 isap_ascon_128a_aead_encrypt
#define crypto_aead_decrypt_2 isap_ascon_128a_aead_decrypt
#define crypto_aead_encrypt_3 isap_keccak_128_aead_encrypt
#define crypto_aead_decrypt_3 isap_keccak_128_aead_decrypt
#define crypto_aead_encrypt_4 isap_ascon_128_aead_encrypt
#define crypto_aead_decrypt_4 isap_ascon_128_aead_decrypt
