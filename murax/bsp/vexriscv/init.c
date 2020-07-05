#include "weak_under_alias.h"

void __weak__init() {}

void __weak__fini() {}

weak_under_alias(_init);
weak_under_alias(_fini);

void _exit(int code) {
    (void)code;
    while (1) {}
}