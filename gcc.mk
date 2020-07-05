# SiFive is rv32imac
# murax is rv32im

CC=$(GCC)
RISCV_ARCHFLAGS=$(RISCV_ARCHFLAGS_GCC)
GCC_CFLAGS_MURAX=-fstrict-volatile-bitfields --specs=nosys.specs

OPT2	           = -O2
OPT2            += -fgcse-after-reload
OPT2            += -finline-functions
# OPT2            += -fipa-cp-clone
# OPT2            += -floop-interchange
OPT2            += -floop-unroll-and-jam
# OPT2            += -fpeel-loops
# OPT2            += -fpredictive-commoning
# OPT2            += -fsplit-loops
# OPT2            += -fsplit-paths
# OPT2            += -ftree-loop-distribute-patterns
# OPT2            += -ftree-loop-distribution
# OPT2            += -ftree-loop-vectorize
# OPT2            += -ftree-partial-pre
# OPT2            += -ftree-slp-vectorize
# OPT2            += -funswitch-loops
# OPT2            += -fversion-loops-for-strides


OPT3 = -O3
