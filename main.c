#include "scheme.h"

#include "test/test.h"
#include "test/test_aead.h"
#include "test/test_hash.h"

#ifdef MURAX
#include "murax/hal.h"
#endif

#include <stdio.h>

inline static void __attribute__((always_inline)) do_nothing() {
    for (int i = 0; i < 1000; ++i) {
        __asm__("NOP");
    }
    for (int i = 0; i < 2; ++i)
        printf("\n");
}

int main(void) {
    // Instruction cache misses are relatively expensive, so for more
    // consistent benchmarks that depend less on the relative speed of
    // the QSPI flash, you might want to fill the instruction cache first.
    // Note that it can hold 16 KiB of instructions on the HiFive1.

   do_nothing();

    printf("starting benchmarking program\n");

   do_nothing();

#ifdef ADDITIONAL
    additional_tests();
#endif

#ifdef crypto_hash
    test_crypto_hash();
#endif

#ifdef crypto_aead_encrypt
    test_crypto_aead();
#endif

    printf("DONE!\n");

    return 0;
}
