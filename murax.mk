TARGET       := main.hex main.bin
BASE_TARGET  := main
C_TARGET     := main.c

.PHONY: all
all: $(TARGET)

PLATFORM=pqvexriscvsim

# PLATFORM_BSP_DIR := $(dir $(lastword $(MAKEFILE_LIST)))bsp/vexriscv

LDSCRIPT = $(PLATFORM).ld

LDFLAGS = -L$(PATH_RISCV)lib/gcc/riscv64-unknown-elf/9.2.0/rv32imac/ilp32
LDFLAGS += -L$(RISCVPATH)lib/rv32imac/ilp32
LDFLAGS += -specs=nosys.specs
LDFLAGS += -Wl,-T$(LDSCRIPT)
LDFLAGS += -nostartfiles -ffreestanding -Wl,--gc-sections
LDFLAGS += -L.
LDFLAGS += -Wl,--start-group -l$(PLATFORM)bsp -lc -Wl,--end-group

INCLUDES := -Iutils -I$(RISCVPATH)/include

OPTS ?= $(OPT2)

include murax/bsp/vexriscv/vexriscv.mk

# SDK
SDK_ASM_SRCS   := $(wildcard murax/*.S)
SDK_H          := $(wildcard murax/*.h)
SDK_C_SRCS     := $(filter-out murax/main.c, $(wildcard murax/*.c)) $(wildcard murax/nosys/*.c)
SDK_C_OBJS     := $(SDK_C_SRCS:.c=.o)
SDK_ASM_OBJS   := $(SDK_ASM_SRCS:.S=.o)
SDK_OBJS       := $(SDK_C_OBJS) $(SDK_ASM_OBJS)


COMMON_CFLAGS 	:= 	-g -fno-builtin-printf \
		-Wno-unused-parameter \
		-Wall -Wextra -Wredundant-decls \
		-Wshadow -Wno-unused-function \
		-fno-common -Iutils -DMURAX
		

help:
	@$(ECHO) $(RED)"Compile & Simulate"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"1. "$(CYAN)"make"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"2. open 3 terminals"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"3. Terminal 2: "$(CYAN)"make startsim"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"4. Terminal 3: "$(CYAN)"make openocd"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"5. Terminal 1: "$(CYAN)"make flash"$(DARKGRAY)


startsim:
	@$(ECHO) $(CYAN)'sbt "runMain mupq.PQVexRiscvSim --ram 256,128"'$(DARKGRAY)
	@# the PATH is required for verilator to work with sbt.
	@export PATH=$(PATH):$(PATH_VERILATOR_BIN) ; \
	cd Building_Tools/pqriscv-vexriscv/ ; \
	sbt "runMain mupq.PQVexRiscvSim --ram 256,128"

openocd:
	@$(ECHO) $(CYAN)"$(PATH_VERILATOR_BIN)/openocd --file pqvexriscvsim.cfg"$(DARKGRAY)
	@ cd Building_Tools/pqriscv-vexriscv/ ; \
	$(PATH_VERILATOR_BIN)/openocd --file pqvexriscvsim.cfg

flash: main main.bin main.hex
	$(GDB) -ex 'set remotetimeout 15' -ex 'target remote :3333' -ex 'load' -ex 'break main' -ex 'continue' main -ex 'continue' 
	# set remotetimeout 15      set the timout of the remote (openocd) to 15 seconds
	# target remote :3333       connect to `openocd-vexriscv` on localhost:3333
	# load                      load `main` onto the simulated core
	# break main                set a breakpoint on main
	# continue                  start running (until we hit main)

objdump: main.hex
	@$(ECHO) $(LIGHTPURPLE)"Disassembling "$@$(DARKGRAY)
	$(OBJDUMP) -d main > main.dump

