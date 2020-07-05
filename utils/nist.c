#include "nist.h"

void init_buffer(unsigned char *buffer, unsigned long long numbytes) {
    for (unsigned long long i = 0; i < numbytes; i++)
        buffer[i] = (unsigned char)i;
}

void print_bstr(const char *label, const unsigned char *data,
                unsigned long long length) {
    printf("%s", label);

    for (unsigned long long i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

void xor_bstr(unsigned char *out, const unsigned char *in,
              unsigned long long numbytes) {
    for (unsigned long long i = 0; i < numbytes; i++) {
        *out ^= *in;
        out++;
        in++;
    }
}

void highlight_tag(const char *label, unsigned long long numbytes,
                   unsigned long long taglen) {
    printf("%s", label);

    for (unsigned long long i = 0; i < numbytes - taglen; i++) {
        printf("..");
    }
    for (unsigned long long i = 0; i < taglen; i++) {
        printf("XX");
    }
    printf("\n");
}
