/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodyak_h_
#define _Xoodyak_h_

#include <stddef.h>
#include "Xoodoo-SnP.h"
#include "Cyclist.h"

#define Xoodyak_asm_f_bPrime    48
#define Xoodyak_asm_Rhash       16
#define Xoodyak_asm_Rkin        44
#define Xoodyak_asm_Rkout       24
#define Xoodyak_asm_lRatchet    16


KCP_DeclareCyclistStructure(Xoodyak_asm, Xoodoo_asm_stateSizeInBytes, Xoodoo_asm_stateAlignment)
KCP_DeclareCyclistFunctions(Xoodyak_asm)

#endif
