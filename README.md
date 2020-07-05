# Lightweight Crypto on Risc-V


## Installation of the dependencies.

Have a look at install.sh

We install compilers and libraries in /opt/riscv and in /opt/verilator

:warning: Execute the install.sh line by line instead of one go (which would likely fail).

## Compiling a scheme

First edit `config.mk` and `config.sh` to reflect the correct path of the compilers

### 1. For Sifive

1. Open 2 terminals.
2. Execute `./configure.py [SCHEME Folder] [compiler] sifive` to generate the correct `Makefile` and dependencies.
3. Compile with `make`
4. Plug your SiFive board, you should see a `/dev/ttyUSB0` and `/dev/ttyUSB1` appear.
5. In the second terminal, execute `./watch.sh`.  
   This will start a `screen` on `ttyUSB1` and log the output into `main.log`
6. In the first terminal, execute `make upload` (may need sudo).  
   This will upload `main.elf` to the board, you should see output on the second terminal.
7. To compile for another scheme, execute `make clean`, this will remove any generated file (including the `Makefile`)

:warning: it is possible for your board to not display results, this is a bug due to the latest compilers.
We included the binary `main-valid.elf` to "reset" the board in a working state. To flash it on the board, simply do `make reset`.

### 2. For pqriscv-vexriscv (aka. murax)

1. Open 3 terminals.
2. Execute `./configure.py [SCHEME Folder] gcc murax` to generate the correct `Makefile` and dependencies.
3. In terminal 1, Compile with `make`
4. In terminal 2, run `make startsim` to boot the simulator
5. In terminal 3, once the you read `WAITING FOR TCP JTAG CONNECTION` in terminal 2, run `make openocd` to start openocd.
6. In terminal 1, once you read `Info : Listening on port 4444 for telnet connections`, run `make flash`. This will upload the binary to the simulator.
7. In terminal 2, you should see the result of the execution.

**This simulator requires compilation with GCC and with `-O2` instead of `-O3`**

### 3. For riscvOVPsim

1. Execute `./configure.py [SCHEME Folder] [compiler] riscvOVPsim`
2. Execute `make startsim`

## Compiling and simulating using the bit manipulation extension (B)

1. Clone the "RISC-V Bitmanip (Bit Manipulation) Extension" `git clone https://github.com/riscv/riscv-bitmanip`.
1. Build and install the tools following the instructions from https://github.com/riscv/riscv-bitmanip/tree/master/tools.
2. Execute `./configure.py [SCHEME Folder] gcc_b spike` to generate the correct `Makefile` and dependencies.
3. Compile with `make RISCV_ARCH=rv64gcb` in order to build a version using the B extension or `make RISCV_ARCH=rv64gc` to build a version without the B extension.
4. Execute `make startsim RISCV_ARCH=rv64gcb` in order to simulate using the the B extension or `make startsim RISCV_ARCH=rv64gc` to simulate w/o B extension.
    The corresponding binary (see step 4) should be available.
