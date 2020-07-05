TARGET       := main_B.elf
C_TARGET     := main.c

.PHONY: all
all: $(TARGET)

PLATFORM=spike

INCLUDES := -Iutils -I$(RISCVPATH_B)/include

OPTS ?= $(OPT3)

COMMON_CFLAGS 	:= 	-g -fno-builtin-printf \
		-Dsim \
		-Wno-unused-parameter \
		-Wall -Wextra -Wredundant-decls \
		-Wshadow -Wno-unused-function \
		-fno-common


help:
	@$(ECHO) $(RED)"Compile & Simulate"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"1. "$(CYAN)"make"$(DARKGRAY)
	@$(ECHO) $(LIGHTGRAY)"2. "$(CYAN)"make startsim"$(DARKGRAY)

startsim: $(TARGET)
	@$(ECHO) $(CYAN)"/opt/riscv64b/bin/spike --isa=RV64GCB pk" $(TARGET) $(DARKGRAY)
	@/opt/riscv64b/bin/spike --isa=RV64GCB pk $(TARGET)

objdump: $(TARGET)
	@$(ECHO) $(LIGHTPURPLE)"Disassembling "$@$(DARKGRAY)
	$(OBJDUMP_B) -d $(TARGET) > main.dump
