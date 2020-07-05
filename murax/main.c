#include <stdbool.h>

#include "hal.h"

void init_buffer(unsigned char *buffer, unsigned long long numbytes)
{
  unsigned long long i;
	for (i = 0; i < numbytes; i++)
		buffer[i] = (unsigned char)i;
}

int main(void)
{
    unsigned char		   msg[64];
    unsigned long long i, length;
    hal_send_str("Hello World!\n");
    printf_("printf_: Hello World (%d) :D\n", 1337);
    printf_("printf_: Hello World (0x%X) :D\n", 0x1337);
    printf("printf: Hello World (0x%08X) :D\n", 0x1337);

    init_buffer(msg, sizeof(msg));

    for (length = 0; length < 64; length++)
    {
      printf("len %llu:\n",length);
      for (i = 0; i < length; i++)
    		printf("%02X", msg[i]);
        printf("\n");
    }

    printf("DONE!\n");
    volatile int a = 0;
    while (true) {
        a += 42;
    }

    return 0;
}
