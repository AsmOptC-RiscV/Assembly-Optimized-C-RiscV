TARGET       := mainOVP.elf
C_TARGET     := main.c

.PHONY: all
all: $(TARGET)

PLATFORM=riscvOVPsim

#LDSCRIPT = $(PLATFORM).ld

#LDFLAGS = -L$(PATH_RISCV)lib/gcc/riscv64-unknown-elf/9.2.0/rv32imac/ilp32
#LDFLAGS += -L$(RISCVPATH)lib/rv32imac/ilp32
#LDFLAGS += -specs=nosys.specs
#LDFLAGS += -Wl,-T$(LDSCRIPT)
#LDFLAGS += -nostartfiles -ffreestanding -Wl,--gc-sections
#LDFLAGS += -L.
#LDFLAGS += -Wl,--start-group -l$(PLATFORM)bsp -lc -Wl,--end-group

INCLUDES := -Iutils -I$(RISCVPATH)/include

OPTS ?= $(OPT3)

#include murax/bsp/vexriscv/vexriscv.mk

# SDK
#SDK_ASM_SRCS   := $(wildcard murax/*.S)
#SDK_H          := $(wildcard murax/*.h)
#SDK_C_SRCS     := $(filter-out murax/main.c, $(wildcard murax/*.c)) $(wildcard murax/nosys/*.c)
#SDK_C_OBJS     := $(SDK_C_SRCS:.c=.o)
#SDK_ASM_OBJS   := $(SDK_ASM_SRCS:.S=.o)
#SDK_OBJS       := $(SDK_C_OBJS) $(SDK_ASM_OBJS)


COMMON_CFLAGS 	:= 	-g -fno-builtin-printf \
		-Wno-unused-parameter \
		-Wall -Wextra -Wredundant-decls \
		-Wshadow -Wno-unused-function \
		-fno-common

#COMMON_CFLAGS 	:= 	-g $(OPTS) -march=rv32imc -mabi=ilp32 
		

help:
	@$(ECHO) $(RED)"Compile & Simulate"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"1. "$(CYAN)"make"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"2. "$(CYAN)"make startsim"$(DARKGRAY)

startsim: $(TARGET)
	@$(ECHO) $(CYAN)'./riscvOVPsim/bin/Linux64/riscvOVPsim.exe --program $(TARGET) --variant RV32IMC'$(DARKGRAY)
	@cd riscvOVPsim/bin/Linux64/ ; \
	./riscvOVPsim.exe --program ../../../$(TARGET) --variant RV32IMC

objdump: $(TARGET)
	@$(ECHO) $(LIGHTPURPLE)"Disassembling "$@$(DARKGRAY)
	$(OBJDUMP) -d $(TARGET) > main.dump
