NO_COLOR="\033[0m"
RED = "\033[38;5;009m"
GREEN = "\033[38;5;010m"
YELLOW = "\033[38;5;011m"
ORANGE = "\033[38;5;214m"
LIGHTPURPLE = "\033[38;5;177m"
PURPLE = "\033[38;5;135m"
CYAN = "\033[38;5;014m"
LIGHTBLUE = "\033[38;5;39m"
BLUE = "\033[38;5;27m"
DARKBLUE = "\033[38;5;19m"
LIGHTGRAY = "\033[38;5;252m"
DARKGRAY = "\033[38;5;242m"
BRIGHTRED = "\033[91m"
BOLD = "\033[1m"
ECHO?=echo

PLATFORM ?= murax
RISCV_ARCH ?= rv32im
RISCV_ABI ?= ilp32
RISCV_CMODEL ?= medany
RISCV_ARCHFLAGS += -march=$(RISCV_ARCH)
RISCV_ARCHFLAGS += -mabi=$(RISCV_ABI)
RISCV_ARCHFLAGS += -mcmodel=$(RISCV_CMODEL)

CFLAGS += -Os -Wall -Wextra
CFLAGS += $(RISCV_ARCHFLAGS)
CFLAGS += -fstrict-volatile-bitfields
CFLAGS += --specs=nosys.specs

ifeq ($(PLATFORM),murax)
CFLAGS += -DVEXRISCV_VOLATILE
endif

ifeq ($(PLATFORM),pqvexriscvup5k)
CFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvicoboard)
CFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvsim)
CFLAGS += -DVEXRISCV_RWMTVEC
endif

CROSS_PREFIX ?= riscv64-unknown-elf
CC = /opt/riscv/bin/$(CROSS_PREFIX)-gcc
AR = /opt/riscv/bin/$(CROSS_PREFIX)-ar

SRCS = init.c start.s
OBJS = $(PLATFORM)_init.o $(PLATFORM)_start.o
TARGET = lib$(PLATFORM)bsp.a

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(ECHO) $(LIGHTBLUE)"building "$@""$(DARKGRAY)
	$(AR) rcs $@ $^

$(PLATFORM)_%.o: %.c
	@$(ECHO) $(YELLOW)"building "$@" (.c)"$(DARKGRAY)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PLATFORM)_%.o: %.S
	@$(ECHO) $(ORANGE)"building "$@""$(DARKGRAY)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@$(ECHO) $(RED)"cleaning..."$(DARKGRAY)
	rm -rf $(OBJS) $(TARGET)
