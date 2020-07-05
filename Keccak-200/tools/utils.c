#include "utils.h"

void reset_state(uint32_t *x) {
    int i;
    for (i = 0; i < 25; ++i)
        x[i] = i * i * i + i * 0x9e3779b9;
}

void wipe_state(uint32_t *x) {
    int i;
    for (i = 0; i < 25; ++i)
        x[i] = 0;
}

void print_state(uint32_t *x) {
    int i;
    printf("----------------------\n");
    for (i = 0; i < 25; ++i) {
        printf("%08lx ", (unsigned long)x[i]);
        if (i % 5 == 4)
            printf("\n");
    }
}
