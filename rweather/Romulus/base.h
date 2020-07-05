#pragma once

// #define ADDITIONAL
#define NO_CHECK

#include "kat.h"
#include "src/romulus.h"

#define crypto_aead_encrypt romulus_n1_aead_encrypt
#define crypto_aead_decrypt romulus_n1_aead_decrypt
#define crypto_aead_encrypt_2 romulus_n2_aead_encrypt
#define crypto_aead_decrypt_2 romulus_n2_aead_decrypt
#define crypto_aead_encrypt_3 romulus_n3_aead_encrypt
#define crypto_aead_decrypt_3 romulus_n3_aead_decrypt
#define crypto_aead_encrypt_4 romulus_m1_aead_encrypt
#define crypto_aead_decrypt_4 romulus_m1_aead_decrypt
#define crypto_aead_encrypt_5 romulus_m2_aead_encrypt
#define crypto_aead_decrypt_5 romulus_m2_aead_decrypt
#define crypto_aead_encrypt_6 romulus_m3_aead_encrypt
#define crypto_aead_decrypt_6 romulus_m3_aead_decrypt
