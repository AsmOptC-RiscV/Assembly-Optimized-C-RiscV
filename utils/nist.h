#pragma once

#ifdef MURAX
#include "../murax/hal.h"
#else
#include <stdio.h>
#endif

void print_bstr(const char *label, const unsigned char *data, unsigned long long length);

void init_buffer(unsigned char *buffer, unsigned long long numbytes);

void xor_bstr(unsigned char *out, const unsigned char *in,
              unsigned long long numbytes);

void highlight_tag(const char *label, unsigned long long numbytes, unsigned long long taglen);