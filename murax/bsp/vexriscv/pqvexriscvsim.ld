OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY( _start )

__ram_size = 384K;

MEMORY
{
  rom (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 256K
  ram (wxa!ri) : ORIGIN = 0x80040000, LENGTH = 128K
}

INCLUDE nonvolatile.ld
