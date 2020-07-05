#pragma once

// #define ADDITIONAL

#include "kat.h"
#include "src/gift-cofb.h"

#define crypto_aead_encrypt gift_cofb_aead_encrypt
#define crypto_aead_decrypt gift_cofb_aead_decrypt
