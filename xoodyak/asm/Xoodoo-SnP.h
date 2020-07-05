/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_asm_SnP_h_
#define _Xoodoo_asm_SnP_h_

/** For the documentation, see SnP-documentation.h.
 */

#define Xoodoo_asm_implementation      "32-bit reference implementation"
#define Xoodoo_asm_stateSizeInBytes    (3*4*4)
#define Xoodoo_asm_stateAlignment      4
#define Xoodoo_asm_HasNround

void Xoodoo_asm_StaticInitialize( void );
void Xoodoo_asm_Initialize(void *state);
void Xoodoo_asm_AddByte(void *state, unsigned char data, unsigned int offset);
void Xoodoo_asm_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_asm_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_asm_OverwriteWithZeroes(void *state, unsigned int byteCount);
void Xoodoo_asm_Permute_Nrounds(void *state, unsigned int nrounds);
void Xoodoo_asm_Permute_6rounds(void *state);
void Xoodoo_asm_Permute_12rounds(void *state);
void Xoodoo_asm_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_asm_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#endif
