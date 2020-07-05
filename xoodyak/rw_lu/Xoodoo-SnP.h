/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_rw_lu_SnP_h_
#define _Xoodoo_rw_lu_SnP_h_

/** For the documentation, see SnP-documentation.h.
 */

#define Xoodoo_rw_lu_implementation      "32-bit reference implementation"
#define Xoodoo_rw_lu_stateSizeInBytes    (3*4*4)
#define Xoodoo_rw_lu_stateAlignment      4
#define Xoodoo_rw_lu_HasNround

void Xoodoo_rw_lu_StaticInitialize( void );
void Xoodoo_rw_lu_Initialize(void *state);
void Xoodoo_rw_lu_AddByte(void *state, unsigned char data, unsigned int offset);
void Xoodoo_rw_lu_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_rw_lu_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_rw_lu_OverwriteWithZeroes(void *state, unsigned int byteCount);
void Xoodoo_rw_lu_Permute_Nrounds(void *state, unsigned int nrounds);
void Xoodoo_rw_lu_Permute_6rounds(void *state);
void Xoodoo_rw_lu_Permute_12rounds(void *state);
void Xoodoo_rw_lu_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodoo_rw_lu_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#endif
