#include "ascon.h"

void ascon_ref(state *p, int round_constant) {
    state s = *p;
    state t;
    for (; round_constant > 0x3c; round_constant -= 15) {
        // addition of round constant
        s.x2 ^= (uint64_t)round_constant;
        // substitution layer
        s.x0 ^= s.x4;
        s.x4 ^= s.x3;
        s.x2 ^= s.x1;
        // start of keccak s-box
        t.x0 = ~s.x0;
        t.x1 = ~s.x1;
        t.x2 = ~s.x2;
        t.x3 = ~s.x3;
        t.x4 = ~s.x4;
        t.x0 &= s.x1;
        t.x1 &= s.x2;
        t.x2 &= s.x3;
        t.x3 &= s.x4;
        t.x4 &= s.x0;
        s.x0 ^= t.x1;
        s.x1 ^= t.x2;
        s.x2 ^= t.x3;
        s.x3 ^= t.x4;
        s.x4 ^= t.x0;
        // end of keccak s-box
        s.x1 ^= s.x0;
        s.x0 ^= s.x4;
        s.x3 ^= s.x2;
        s.x2 = ~s.x2;
        // linear diffusion layer
        s.x0 ^= ROTR64(s.x0, 19) ^ ROTR64(s.x0, 28);
        s.x1 ^= ROTR64(s.x1, 61) ^ ROTR64(s.x1, 39);
        s.x2 ^= ROTR64(s.x2, 1) ^ ROTR64(s.x2, 6);
        s.x3 ^= ROTR64(s.x3, 10) ^ ROTR64(s.x3, 17);
        s.x4 ^= ROTR64(s.x4, 7) ^ ROTR64(s.x4, 41);
    }
    *p = s;
}
