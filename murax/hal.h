#ifndef VECRISCV_HAL_H_
#define VECRISCV_HAL_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define printf printf_

void hal_send(const uint8_t* in, const size_t len);
void hal_send_str(const char* in);

int printf_(const char* format, ...);

void _putchar(char c);
void _write(int fd, const void* ptr, size_t len);

#endif /* VECRISCV_HAL_H_ */
