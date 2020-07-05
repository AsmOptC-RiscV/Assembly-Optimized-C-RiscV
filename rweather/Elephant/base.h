#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/elephant.h"

#define crypto_aead_encrypt dumbo_aead_encrypt
#define crypto_aead_decrypt dumbo_aead_decrypt
#define crypto_aead_encrypt_2 jumbo_aead_encrypt
#define crypto_aead_decrypt_2 jumbo_aead_decrypt
#define crypto_aead_encrypt_3 delirium_aead_encrypt
#define crypto_aead_decrypt_3 delirium_aead_decrypt
