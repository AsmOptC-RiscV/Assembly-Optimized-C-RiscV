/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_lc_SnP_h_
#define _Xoodoo_lc_SnP_h_

/** For the documentation, see SnP-documentation.h.
 */

#define Xoodoo_lc_implementation      "32-bit reference implementation"
#define Xoodoo_lc_stateSizeInBytes    (3*4*4)
#define Xoodoo_lc_stateAlignment      4
#define Xoodoo_lc_HasNround

void Xoodoo_lc_StaticInitialize( void );
void Xoodoo_lc_Initialize(void *state);
void Xoodoo_lc_AddByte(void *state, unsigned char data, unsigned int offset);
void Xoodoo_lc_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_lc_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_lc_OverwriteWithZeroes(void *state, unsigned int byteCount);
void Xoodoo_lc_Permute_Nrounds(void *state, unsigned int nrounds);
void Xoodoo_lc_Permute_6rounds(void *state);
void Xoodoo_lc_Permute_12rounds(void *state);
void Xoodoo_lc_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_lc_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#endif
