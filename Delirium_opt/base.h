#pragma once

// #define NO_CHECK

#include "kat.h"
#include "opt/crypto_aead.h"
#include "ref/crypto_aead_ref.h"

#define crypto_aead_encrypt elephant_encrypt_ref
#define crypto_aead_decrypt elephant_decrypt_ref

#define crypto_aead_encrypt_2 elephant_encrypt
#define crypto_aead_decrypt_2 elephant_decrypt
