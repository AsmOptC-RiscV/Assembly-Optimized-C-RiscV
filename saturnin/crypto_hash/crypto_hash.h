#pragma once

int crypto_hash(unsigned char *out, const unsigned char *in, unsigned long long inlen);

int crypto_hash_asm(unsigned char *out, const unsigned char *in, unsigned long long inlen);
