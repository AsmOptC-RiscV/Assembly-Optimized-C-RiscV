include ../config.mk
# this defines:
# $PATH_RISCV = /opt/riscv/
# $PATH_RISC_BIN = /opt/riscv/bin/
# AR = $($PATH_RISC_BIN)riscv64-unknown-elf-ar
# GCC = $($PATH_RISC_BIN)riscv64-unknown-elf-gcc
# CLANG = clang
# GDB = $($PATH_RISC_BIN)riscv64-unknown-elf-gdb
# OBJDUMP = $($PATH_RISC_BIN)riscv64-unknown-elf-objdump
# OBJCOPY = $($PATH_RISC_BIN)riscv64-unknown-elf-objcopy
# RISCVPATH = $(PATH_RISCV)riscv64-unknown-elf
# OPENOCD? = $($PATH_RISC_BIN)openocd
# ECHO? = echo
# RISCV_ARCH ?= rv32im
# RISCV_ABI ?= ilp32
# RISCV_CMODEL ?= medany
# RISCV_ARCHFLAGS_GCC += -march=$(RISCV_ARCH)
# RISCV_ARCHFLAGS_GCC += -mabi=$(RISCV_ABI)
# RISCV_ARCHFLAGS_GCC += -mcmodel=$(RISCV_CMODEL)
# RISCV_ARCHFLAGS_CLANG+=--target=riscv32


include ../clang.mk
# include ../gcc.mk
# this defines:
# CC
# RISCV_ARCHFLAGS

COMMON_CFLAGS := -g -O3 -fno-builtin-printf \
				-Wno-unused-parameter \
				-Wall -Wextra -Wredundant-decls \
				-Wshadow -Wno-unused-function \
				-fno-common \
				-I$(RISCVPATH)/include

CFLAGS  += $(COMMON_CFLAGS) $(GCC_CFLAGS_MURAX) $(RISCV_ARCHFLAGS)


.PHONY: all
all: main.hex main.bin

PLATFORM ?= pqvexriscvsim

LDSCRIPT = $(PLATFORM).ld
# LD Flags that must be used to link executables for the Murax SoC
LDFLAGS = \
		-L/opt/riscv/lib/gcc/riscv64-unknown-elf/9.2.0/rv32imac/ilp32 \
		-L$(RISCVPATH)/lib/rv32imac/ilp32
LDFLAGS += $(RISCV_GCC_ARCHFLAGS)
LDFLAGS += --specs=nosys.specs
LDFLAGS += -Wl,-T$(LDSCRIPT)
LDFLAGS += -nostartfiles -ffreestanding -Wl,--gc-sections
LDFLAGS += -L.
LDFLAGS += -Wl,--start-group -l$(PLATFORM)bsp -lc -Wl,--end-group

include bsp/vexriscv/vexriscv.mk

SDK_ASM_SRCS   := $(wildcard *.S)
SDK_C_SRCS     := $(wildcard *.c)
SDK_C_OBJS     := $(SDK_C_SRCS:.c=.o)
SDK_ASM_OBJS   := $(SDK_ASM_SRCS:.S=.o)
SDK_OBJS       := $(SDK_C_OBJS) $(SDK_ASM_OBJS)

main.bin: main
	@$(ECHO) $(PURPLE)"obj "$@""$(DARKGRAY)
	$(OBJCOPY) -O binary $< $@

main.hex: main
	@$(ECHO) $(PURPLE)"obj "$@""$(DARKGRAY)
	$(OBJCOPY) -O ihex $< $@

main.v: main
	@$(ECHO) $(ORANGE)"building "$@""$(DARKGRAY)
	# [2019-12-12 Daan] Don't know what this would be needed for, but
	# maybe we can do more efficient simulations of the software?
	$(OBJCOPY) -O verilog $< $@

main: $(LIBWRAP) $(SDK_OBJS) $(LDSCRIPT)
	@$(ECHO) $(LIGHTPURPLE)"building "$@""$(DARKGRAY)
	$(GCC) $(RISCV_ARCHFLAGS_GCC) $(COMMON_CFLAGS) $(SDK_OBJS) \
		-o $@ \
		$(LDFLAGS)

hal-vexriscv.o: hal-vexriscv.c
	@$(ECHO) $(BRIGHTRED)"building "$@" (.c)"$(DARKGRAY)
	$(GCC) $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS_GCC) -fstrict-volatile-bitfields --specs=nosys.specs -c -o $@ $<
main.o: main.c
	@$(ECHO) $(BRIGHTRED)"building "$@" (.c)"$(DARKGRAY)
	$(GCC) $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS_GCC) -fstrict-volatile-bitfields --specs=nosys.specs -c -o $@ $<
printf.o: printf.c
	@$(ECHO) $(BRIGHTRED)"building "$@" (.c)"$(DARKGRAY)
	$(GCC) $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS_GCC) -fstrict-volatile-bitfields --specs=nosys.specs -c -o $@ $<
hal.o: hal.c
	@$(ECHO) $(BRIGHTRED)"building "$@" (.c)"$(DARKGRAY)
	$(GCC) $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS_GCC) -fstrict-volatile-bitfields --specs=nosys.specs -c -o $@ $<


crt.o: crt.S

%.o: %.S
	@$(ECHO) $(ORANGE)"building "$@" (.S)"$(DARKGRAY)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	@$(ECHO) $(YELLOW)"building "$@" (.c)"$(DARKGRAY)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@$(ECHO) $(RED)"cleaning..."$(DARKGRAY)
	rm -f main main.bin main.hex $(SDK_OBJS) $(LIBWRAP_OBJS) $(LIBWRAP)

help:
	@$(ECHO) $(RED)"Compile & Simulate"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"1. "$(CYAN)"make"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"2. open 3 terminals"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"3. Terminal 2: "$(CYAN)"make startsim"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"4. Terminal 3: "$(CYAN)"make openocd"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"5. Terminal 1: "$(CYAN)"make flash"$(DARKGRAY)

startsim:
	cd ../Building_Tools/pqriscv-vexriscv/ ; sbt "runMain mupq.PQVexRiscvSim --ram 256,128"

openocd:
	cd ../Building_Tools/pqriscv-vexriscv/ ; /opt/verilator/bin/openocd --file pqvexriscvsim.cfg

flash:
	/opt/riscv/bin/riscv64-unknown-elf-gdb -ex 'set remotetimeout 15' -ex 'target remote :3333' -ex 'load' -ex 'break main' -ex 'continue' main
	# set remotetimeout 15      set the timout of the remote (openocd) to 15 seconds
	# target remote :3333       connect to `openocd-vexriscv` on localhost:3333
	# load                      load `main` onto the simulated core
	# break main                set a breakpoint on main
	# continue                  start running (until we hit main)
