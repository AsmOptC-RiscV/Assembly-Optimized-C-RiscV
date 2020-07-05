#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/grain128.h"

#define crypto_aead_encrypt grain128_aead_encrypt
#define crypto_aead_decrypt grain128_aead_decrypt
