#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/orange.h"

#define crypto_aead_encrypt orange_zest_aead_encrypt
#define crypto_aead_decrypt orange_zest_aead_decrypt
#define crypto_hash orangish_hash
