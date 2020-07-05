TARGET       := main.elf
C_TARGET     := $(TARGET:.elf=.c)

.PHONY: all
all: $(TARGET)

include sifive_sdk/libwrap/libwrap.mk

# SDK
SDK_ASM_SRCS   := $(wildcard sifive_sdk/*.S)
SDK_C_SRCS     := $(wildcard sifive_sdk/*.c)
SDK_C_OBJS     := $(SDK_C_SRCS:.c=.o)
SDK_ASM_OBJS   := $(SDK_ASM_SRCS:.S=.o)
SDK_OBJS       := $(SDK_C_OBJS) $(SDK_ASM_OBJS)

#flags:
LDSCRIPT      := sifive_sdk/flash.lds
INCLUDES      := -Isifive_sdk/include -Iutils -I$(RISCVPATH)/include
# INCLUDES_C    := -include sys/cdefs.h

OPTS ?= $(OPT3)

COMMON_CFLAGS := -g -fno-builtin-printf \
				-Wno-unused-parameter \
				-Wall -Wextra -Wredundant-decls \
				-Wshadow -Wno-unused-function \
				-fno-common
				# -Wundef 

LDFLAGS       += -static -T$(LDSCRIPT) -nostartfiles \
				-Lsdk --specs=nano.specs -Wl,--gc-sections


DUMP         := $(TARGET:.elf=.dump)
LOG          := $(TARGET:.elf=.log) # to modify in watch.sh

objdump: $(TARGET)
	@$(ECHO) $(LIGHTPURPLE)"Disassembling "$@$(DARKGRAY)
	$(OBJDUMP) -D $(TARGET) > $(DUMP)


upload: $(TARGET)
	@$(ECHO) $(PURPLE)"Uploading "$<$(DARKGRAY)
	-rm -f $(LOG)
	sudo ./upload.sh $<

reset: main-valid.elf
	@$(ECHO) $(PURPLE)"Reseting board by uploading "$<$(DARKGRAY)
	-rm -f $(LOG)
	sudo ./upload.sh $<  > /dev/null 2>&1
