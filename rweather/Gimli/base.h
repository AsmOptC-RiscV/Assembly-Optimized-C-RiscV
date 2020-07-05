#pragma once

// #define ADDITIONAL

#include "kat.h"
#include "src/gimli24.h"

#define crypto_aead_encrypt gimli24_aead_encrypt
#define crypto_aead_decrypt gimli24_aead_decrypt
#define crypto_hash gimli24_hash
