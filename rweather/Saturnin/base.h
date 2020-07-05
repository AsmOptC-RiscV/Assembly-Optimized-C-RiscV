#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/saturnin.h"

#define crypto_aead_encrypt saturnin_aead_encrypt
#define crypto_aead_decrypt saturnin_aead_decrypt
#define crypto_aead_encrypt_2 saturnin_short_aead_encrypt
#define crypto_aead_decrypt_2 saturnin_short_aead_decrypt
#define crypto_hash saturnin_hash
