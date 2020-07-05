#pragma once

// do we have additional tests: void test_perm()
#define ADDITIONAL

#define crypto_hash crypto_hash
#define crypto_hash_2 crypto_hash_asm
#define crypto_hash_3 crypto_hash_c_opt
#define crypto_hash_4 crypto_hash_c_opt_unrolled

#define crypto_aead_encrypt crypto_aead_encrypt
#define crypto_aead_encrypt_2 crypto_aead_encrypt_asm
#define crypto_aead_encrypt_3 crypto_aead_encrypt_c_opt
#define crypto_aead_encrypt_4 crypto_aead_encrypt_c_opt_unrolled

#define crypto_aead_decrypt crypto_aead_decrypt
#define crypto_aead_decrypt_2 crypto_aead_decrypt_asm
#define crypto_aead_decrypt_3 crypto_aead_decrypt_c_opt

#include "Permutation/gimli.h"
#include "Permutation/test_perm.h"
#include "crypto_aead/crypto_aead.h"
#include "crypto_hash/crypto_hash.h"
#include "kat.h"
