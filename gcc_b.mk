PATH_RISCV=/opt/riscv64b/
PATH_RISC_BIN=$(PATH_RISCV)bin/
PATH_VERILATOR_BIN=/opt/verilator/bin/

# If these tools are not in your $PATH, enter the full path.
AR=$(PATH_RISC_BIN)riscv64-unknown-elf-ar
GCC=$(PATH_RISC_BIN)riscv64-unknown-elf-gcc
CLANG=clang
GDB=$(PATH_RISC_BIN)riscv64-unknown-elf-gdb
OBJDUMP=$(PATH_RISC_BIN)riscv64-unknown-elf-objdump
OBJCOPY=$(PATH_RISC_BIN)riscv64-unknown-elf-objcopy
RISCVPATH=$(PATH_RISCV)riscv64-unknown-elf
OPENOCD?=$(PATH_RISC_BIN)openocd
ECHO?=echo

RISCV_ARCH=rv64gcb
RISCV_ABI=lp64d
RISCV_CMODEL=medany
RISCV_ARCHFLAGS_GCC+=-march=$(RISCV_ARCH)
RISCV_ARCHFLAGS_GCC+=-mabi=$(RISCV_ABI)
RISCV_ARCHFLAGS_GCC+=-mcmodel=$(RISCV_CMODEL)

RISCV_ARCHFLAGS=$(RISCV_ARCHFLAGS_GCC)

CC=$(GCC)
