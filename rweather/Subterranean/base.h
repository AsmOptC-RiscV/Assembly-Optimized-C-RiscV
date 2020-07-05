#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/subterranean.h"

#define crypto_aead_encrypt subterranean_aead_encrypt
#define crypto_aead_decrypt subterranean_aead_decrypt
#define crypto_hash subterranean_hash
