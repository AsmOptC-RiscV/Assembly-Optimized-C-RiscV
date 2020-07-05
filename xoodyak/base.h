#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "ref/crypto_aead.h"
#include "ref/crypto_hash.h"
#include "asm/crypto_aead.h"
#include "asm/crypto_hash.h"
#include "rw_lu/crypto_aead.h"
#include "rw_lu/crypto_hash.h"
#include "lc/crypto_aead.h"
#include "lc/crypto_hash.h"

#define crypto_aead_encrypt crypto_aead_encrypt_ref
#define crypto_aead_decrypt crypto_aead_decrypt_ref
#define crypto_aead_encrypt_2 crypto_aead_encrypt_asm
#define crypto_aead_decrypt_2 crypto_aead_decrypt_asm
#define crypto_aead_encrypt_3 crypto_aead_encrypt_rw_lu
#define crypto_aead_decrypt_3 crypto_aead_decrypt_rw_lu
#define crypto_aead_encrypt_4 crypto_aead_encrypt_lc
#define crypto_aead_decrypt_4 crypto_aead_decrypt_lc
//#define crypto_aead_encrypt_2 schwaemm_128_128_aead_encrypt
//#define crypto_aead_decrypt_2 schwaemm_128_128_aead_decrypt
//#define crypto_aead_encrypt_3 schwaemm_192_192_aead_encrypt
//#define crypto_aead_decrypt_3 schwaemm_192_192_aead_decrypt
//#define crypto_aead_encrypt_4 schwaemm_256_256_aead_encrypt
//#define crypto_aead_decrypt_4 schwaemm_256_256_aead_decrypt

#define crypto_hash crypto_hash_ref
#define crypto_hash_2 crypto_hash_asm
#define crypto_hash_3 crypto_hash_rw_lu
#define crypto_hash_4 crypto_hash_lc

// LM + FC results
// #define crypto_aead_encrypt_2 schwaemm_256_128_aead_encrypt_opt_c
// #define crypto_aead_decrypt_2 schwaemm_256_128_aead_decrypt_opt_c

// #define crypto_aead_encrypt_3 schwaemm_256_128_aead_encrypt_asm
// #define crypto_aead_decrypt_3 schwaemm_256_128_aead_decrypt_asm

// #define crypto_hash_2 esch_256_hash_opt_c
// #define crypto_hash_3 esch_256_hash_asm

