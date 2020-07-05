#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/spix.h"

#define crypto_aead_encrypt spix_aead_encrypt
#define crypto_aead_decrypt spix_aead_decrypt
