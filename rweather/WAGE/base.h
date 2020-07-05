#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/wage.h"

#define crypto_aead_encrypt wage_aead_encrypt
#define crypto_aead_decrypt wage_aead_decrypt
