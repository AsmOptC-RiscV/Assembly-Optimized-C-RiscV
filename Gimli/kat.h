#pragma once

#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4

#define MAX_MESSAGE_LENGTH_AEAD 128
#define MAX_ASSOCIATED_DATA_LENGTH 128

// hash related
#define CRYPTO_BYTES 32
#define MAX_MESSAGE_LENGTH_HASH 128
// #define MAX_MESSAGE_LENGTH_HASH			 32

// aead related
#define CRYPTO_KEYBYTES 32
#define CRYPTO_NSECBYTES 0
#define CRYPTO_NPUBBYTES 16
#define CRYPTO_ABYTES 16
#define CRYPTO_NOOVERLAP 1
