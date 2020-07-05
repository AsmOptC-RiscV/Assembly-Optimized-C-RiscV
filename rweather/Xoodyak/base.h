#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/xoodyak.h"

#define crypto_aead_encrypt xoodyak_aead_encrypt
#define crypto_aead_decrypt xoodyak_aead_decrypt
#define crypto_hash xoodyak_hash
