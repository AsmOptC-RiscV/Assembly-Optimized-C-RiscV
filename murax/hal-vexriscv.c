#include "hal.h"
#include <stddef.h>

/* Murax UART */

typedef struct {
  volatile uint32_t DATA;
  volatile uint32_t STATUS;
  volatile uint32_t CLOCK_DIVIDER;
  volatile uint32_t FRAME_CONFIG;
} Uart_Reg;

enum UartParity { NONE = 0, EVEN = 1, ODD = 2 };
enum UartStop { ONE = 0, TWO = 1 };

typedef struct {
  uint32_t dataLength;
  enum UartParity parity;
  enum UartStop stop;
  uint32_t clockDivider;
} Uart_Config;

static uint32_t uart_writeAvailability(Uart_Reg* reg)
{
  return (reg->STATUS >> 16) & 0xFF;
}

static void uart_write(Uart_Reg* reg, uint32_t data)
{
  while (uart_writeAvailability(reg) == 0)
    ;
  reg->DATA = data;
}

#define UART ((Uart_Reg*)(0xF0010000))

void hal_send(const uint8_t* in, const size_t len) {
  for (size_t i = 0; i < len; i++) {
    uart_write(UART, in[i]);
  }
}

void hal_send_str(const char* in)
{
  const char* cur = in;
  while(*cur) {
    uart_write(UART, *cur);
    cur += 1;
  }
}

__attribute__((naked)) uint64_t hal_get_time(void)
{
#define LE "\n\t"
  asm volatile (LE"csrr a1, mcycleh"
                LE"csrr a0, mcycle"
                LE"csrr a2, mcycleh"
                LE"bne a1, a2, hal_get_time"
                LE"ret");
}
