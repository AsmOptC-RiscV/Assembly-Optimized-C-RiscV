#include "utils.h"

void reset_state(uint32_t *x) {
    int i;
    for (i = 0; i < 12; ++i)
        x[i] = i * i * i + i * 0x9e3779b9;
}

void print_state(uint32_t *x) {
    int i;
    printf("----------------------\n");
    for (i = 0; i < 12; ++i) {
        printf("%08lx ", (unsigned long)x[i]);
        if (i % 4 == 3)
            printf("\n");
    }
}
