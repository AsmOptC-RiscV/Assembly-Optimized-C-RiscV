include config.mk
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


include {compiler}.mk
# this defines:
# CC
# RISCV_ARCHFLAGS
# OPT2
# OPT3

# possibly override some options such as O2, O3, Os
-include {scheme}/make.incl.mk
# this defines:
# OPTS

include {platform}.mk
# this also includes:
# .PHONY: all
# all: $(TARGET)
# defines:
# C_TARGET
# LDSCRIPT
# INCLUDES
# COMMON_CFLAGS
# LDFLAGS


SCHEME_H     := scheme.h
MAKEFILE     := Makefile

# common files
NIST           := $(shell find -L {scheme} -path "*/-nist*")
DEPS           := $(filter-out $(NIST), $(shell find -L {scheme} -name "*.s"))
ASM_SRCS       := $(filter-out $(NIST), $(shell find -L {scheme} -name "*.S"))
C_SRCS         := $(filter-out $(NIST), $(shell find -L {scheme} -name "*.c"))
H              := $(filter-out $(NIST), $(shell find -L {scheme} -name "*.h"))
OBJ            := $(C_SRCS:.c=.o) $(ASM_SRCS:.S=.o)

UTILS_ASM_SRCS := utils/getcycles.S
UTILS_C_SRCS   := $(wildcard utils/*.c) $(wildcard test/*.c)
UTILS_H        := $(wildcard utils/*.h) $(wildcard test/*.h)
BASE_C_SRCS    := $(C_TARGET)
BASE_OBJS      := $(UTILS_ASM_SRCS:.S=.o) $(UTILS_C_SRCS:.c=.o) $(BASE_C_SRCS:.c=.o)


OBJS          := $(SDK_OBJS) $(BASE_OBJS) $(OBJ)
ASMS          := $(UTILS_ASM_SRCS) $(ASM_SRCS)
C_SRCS        := $(BASE_C_SRCS) $(UTILS_C_SRCS) $(C_SRCS)
HEADERS       := $(H) $(UTILS_H) $(SCHEME_H) $(SDK_H)

ALL_ASM_OBJS  := $(ASMS:.S=.o)
ALL_C_OBJS    := $(C_SRCS:.c=.o)
ALL_OBJS      := $(ALL_ASM_OBJS) $(ALL_C_OBJS)

CFLAGS        += $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS)

$(SDK_ASM_OBJS): %.o: %.S
	@$(ECHO) $(LIGHTBLUE)"building "$@" (.S)"$(DARKGRAY)
	$(CC) $(OPTS) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(SDK_C_OBJS): %.o: %.c
	@$(ECHO) $(LIGHTBLUE)"building "$@" (.c)"$(DARKGRAY)
	$(CC) $(OPTS) $(CFLAGS) $(INCLUDES) $(INCLUDES_C) -c -o $@ $<

$(ALL_ASM_OBJS): %.o: %.S $(DEPS) $(HEADERS)
	@$(ECHO) $(ORANGE)"building "$@" (.S)"$(DARKGRAY)
	$(CC) $(OPTS) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(ALL_C_OBJS): %.o: %.c $(HEADERS)
	@$(ECHO) $(YELLOW)"building "$@" (.c)"$(DARKGRAY)
	$(CC) $(OPTS) $(CFLAGS) $(INCLUDES) $(INCLUDES_C) -c -o $@ $<


main.bin: $(BASE_TARGET)
	@$(ECHO) $(ORANGE)"Generate bin "$@""$(DARKGRAY)
	$(OBJCOPY) -O binary $< $@

main.hex: $(BASE_TARGET)
	@$(ECHO) $(ORANGE)"Generate hex "$@""$(DARKGRAY)
	$(OBJCOPY) -O ihex $< $@

# main.v: $(BASE_TARGET)
# 	@$(ECHO) $(ORANGE)"building "$@""$(DARKGRAY)
# 	# [2019-12-12 Daan] Don't know what this would be needed for, but
# 	# maybe we can do more efficient simulations of the software?
# 	$(OBJCOPY) -O verilog $< $@

# SiFive
main.elf: $(LIBWRAP) $(SCHEME_H) $(HEADERS) $(OBJS) $(LDSCRIPT) $(ASMS)
	@$(ECHO) $(CYAN)"Building "$@$(DARKGRAY)
	$(GCC) $(OPTS) $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS_GCC) $(INCLUDES) $(OBJS) \
		-o $@ \
		$(LDFLAGS)
	@$(ECHO) $(GREEN)"Done."$(DARKGRAY)

# Murax
main: $(LIBWRAP) $(SCHEME_H) $(HEADERS) $(OBJS) $(LDSCRIPT) $(ASMS)
	@$(ECHO) $(LIGHTPURPLE)"building "$@""$(DARKGRAY)
	$(GCC) $(OPTS) $(COMMON_CFLAGS) $(RISCV_ARCHFLAGS_GCC) $(INCLUDES) $(OBJS) \
		-o $@ \
		$(LDFLAGS) $(RISCV_ARCHFLAGS_GCC)

# riscvOVPsim
mainOVP.elf: $(SCHEME_H) $(HEADERS) $(BASE_OBJS) $(OBJ) $(ASMS)
	@$(ECHO) $(CYAN)"Building "$@$(DARKGRAY)
	$(GCC) $(OPTS) $(RISCV_ARCHFLAGS_GCC) $(INCLUDES) $(BASE_OBJS) $(OBJ) \
		-o $@
	@$(ECHO) $(GREEN)"Done."$(DARKGRAY)

# spike with B extension
main_B.elf: $(SCHEME_H) $(HEADERS) $(BASE_OBJS) $(OBJ) $(ASMS)
	@$(ECHO) $(CYAN)"Building "$@$(DARKGRAY)
	$(GCC) $(OPTS) $(RISCV_ARCHFLAGS_GCC) $(INCLUDES) $(BASE_OBJS) $(OBJ) \
		-o $@
	@$(ECHO) $(GREEN)"Done."$(DARKGRAY)

murax/crt.o: murax/crt.S
# main.o: main.c

deps:
	@echo $(ALL_C_OBJS)

.PHONY: clean
clean:
	@$(ECHO) $(RED)"Cleaning..."$(DARKGRAY)
	-rm -f $(LIBWRAP_OBJS) $(LIBWRAP) $(ALL_OBJS) $(TARGET) $(DUMP) $(LOG) $(SCHEME_H) $(MAKEFILE) $(SDK_OBJS)





gimli-murax: SCHEME=Gimli
gimli-murax: COMPILER=GCC
gimli-murax: ARCHI=murax
gimli-murax: reload

gimli-sifive: SCHEME=Gimli
gimli-sifive: COMPILER=GCC
gimli-sifive: ARCHI=sifive
gimli-sifive: reload

gimli-C-murax: SCHEME=Gimli-C
gimli-C-murax: COMPILER=GCC
gimli-C-murax: ARCHI=murax
gimli-C-murax: reload

gimli-sifive: SCHEME=Gimli-C
gimli-sifive: COMPILER=GCC
gimli-sifive: ARCHI=sifive
gimli-sifive: reload

saturnin-murax: SCHEME=saturnin
saturnin-murax: COMPILER=GCC
saturnin-murax: ARCHI=murax
saturnin-murax: reload

saturnin-sifive: SCHEME=saturnin
saturnin-sifive: COMPILER=GCC
saturnin-sifive: ARCHI=sifive
saturnin-sifive: reload

xoodyak-murax: SCHEME=Xoodyak
xoodyak-murax: COMPILER=GCC
xoodyak-murax: ARCHI=murax
xoodyak-murax: reload

xoodyak-sifive: SCHEME=Xoodyak
xoodyak-sifive: COMPILER=GCC
xoodyak-sifive: ARCHI=sifive
xoodyak-sifive: reload

reload:
	make clean
	python3 configure.py {scheme} {compiler} {platform}
	make
