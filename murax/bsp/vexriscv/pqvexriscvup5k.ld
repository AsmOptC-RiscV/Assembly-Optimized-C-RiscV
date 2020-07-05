OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY( _start )

__ram_size = 128K;

MEMORY
{
  rom (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 64K
  ram (wxa!ri) : ORIGIN = 0x80010000, LENGTH = 64K
}

INCLUDE volatile-split.ld
