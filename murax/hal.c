#include <stdio.h>

#include <stdint.h>
#include <stddef.h>

#include <errno.h>
#undef errno
extern int errno;

#include "hal.h"

// int printf_(const char* format, ...);

void _putchar(char c) {
    hal_send((uint8_t*)&c, 1);
}


void _write(int fd, const void* ptr, size_t len) {
    // Don't care about the fd.  Just put everything on the UART console.
    (void)fd;
    hal_send(ptr, len);
}

// void _fstat(int fd) {}
// void _sbrk(ptrdiff_t incr)
// {
//   extern char _end[];
//   extern char _heap_end[];
//   static char *curbrk = _end;

//   if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
//     return NULL - 1;

//   curbrk += incr;
//   return curbrk - incr;
// }
