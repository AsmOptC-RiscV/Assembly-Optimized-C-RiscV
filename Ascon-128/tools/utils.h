#pragma once

#include "../../utils/stdlibs.h"
#include "../../utils/nist.h"
#include "../Permutation/ascon.h"

#define phi 0x9e3779b97f4a7c15

static const state reset_state = { 1 * phi, 2 * phi, 3 * phi, 4 * phi, 5 * phi };

void print_state(state* x);
void print_state_32(uint32_t* x);
