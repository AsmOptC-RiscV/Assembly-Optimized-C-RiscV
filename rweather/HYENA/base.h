#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/hyena.h"

#define crypto_aead_encrypt hyena_aead_encrypt
#define crypto_aead_decrypt hyena_aead_decrypt
