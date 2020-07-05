#include "utils.h"

// clang-format off
void print_state(state *x) {
    printf("--------------------------\n");
    printf("%016llx %016llx %016llx %016llx %016llx\n",
		x->x0, x->x1, x->x2, x->x3, x->x4);
}

void print_state_32(uint32_t *x) {
    printf("--------------------------\n");
    printf("%08lx%08lx %08lx%08lx %08lx%08lx %08lx%08lx %08lx%08lx\n",
		(unsigned long) x[1], (unsigned long) x[0], 
		(unsigned long) x[3], (unsigned long) x[2], 
		(unsigned long) x[5], (unsigned long) x[4], 
		(unsigned long) x[7], (unsigned long) x[6], 
		(unsigned long) x[9], (unsigned long) x[8]) ;
}
// clang-format on