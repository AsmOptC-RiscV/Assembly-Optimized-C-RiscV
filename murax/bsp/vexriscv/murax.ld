OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY( _start )

MEMORY
{
  ram (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 8k
}

INCLUDE volatile.ld
