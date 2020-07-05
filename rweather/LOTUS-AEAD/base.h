#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/lotus-locus.h"

#define crypto_aead_encrypt lotus_aead_encrypt
#define crypto_aead_decrypt lotus_aead_decrypt
#define crypto_aead_encrypt_2 locus_aead_encrypt
#define crypto_aead_decrypt_2 locus_aead_decrypt
