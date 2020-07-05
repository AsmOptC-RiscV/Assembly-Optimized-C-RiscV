PLATFORM ?= murax

LIBWRAP_CFLAGS += -Os -Wall -Wextra
LIBWRAP_CFLAGS += $(RISCV_ARCHFLAGS_GCC) -fstrict-volatile-bitfields --specs=nosys.specs

ifeq ($(PLATFORM),murax)
LIBWRAP_CFLAGS += -DVEXRISCV_VOLATILE
endif

ifeq ($(PLATFORM),pqvexriscvup5k)
LIBWRAP_CFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvicoboard)
LIBWRAP_CFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvsim)
LIBWRAP_CFLAGS += -DVEXRISCV_RWMTVEC
endif

LIBWRAP_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

LIBWRAP_SRCS_C = init.c
LIBWRAP_SRCS_ASM = start.s

LIBWRAP_OBJS_ = $(LIBWRAP_SRCS_C:.c=.o) $(LIBWRAP_SRCS_ASM:.s=.o)
LIBWRAP_OBJS = $(addprefix $(LIBWRAP_DIR)$(PLATFORM)_,$(LIBWRAP_OBJS_))

LIBWRAP = lib$(PLATFORM)bsp.a

$(LIBWRAP): $(LIBWRAP_OBJS)
	@$(ECHO) $(LIGHTBLUE)"building "$@""$(DARKGRAY)
	$(AR) rcs $@ $^

$(LIBWRAP_DIR)$(PLATFORM)_%.o: $(LIBWRAP_DIR)%.c
	@$(ECHO) $(DARKBLUE)"building "$@" (.c)"$(DARKGRAY)
	$(GCC) $(LIBWRAP_CFLAGS) -c -o $@ $<

$(LIBWRAP_DIR)$(PLATFORM)_%.o: $(LIBWRAP_DIR)%.S
	@$(ECHO) $(DARKBLUE)"building "$@" (.S)"$(DARKGRAY)
	$(GCC) $(LIBWRAP_CFLAGS) -c -o $@ $<
