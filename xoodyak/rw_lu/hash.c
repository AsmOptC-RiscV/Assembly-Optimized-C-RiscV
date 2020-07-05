/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "crypto_hash.h"

#ifndef crypto_hash_BYTES
    #define crypto_hash_BYTES 32
#endif

#include "Xoodyak.h"

int crypto_hash_rw_lu(unsigned char *out, const unsigned char *in, unsigned long long inlen)
{
    Xoodyak_rw_lu_Instance    instance;

    Xoodyak_rw_lu_Initialize(&instance, NULL, 0, NULL, 0, NULL, 0);
    Xoodyak_rw_lu_Absorb(&instance, in, (size_t)inlen);
    Xoodyak_rw_lu_Squeeze(&instance, out, crypto_hash_BYTES);
    #if 0
    {
        unsigned int i;
        for (i = 0; i < crypto_hash_BYTES; ++i )
        {
            printf("\\x%02x", out[i] );
        }
        printf("\n");
    }
    #endif
    return 0;
}