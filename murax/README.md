# Getting the simulator to run

## Checklist for Daan

- [ ] Get it working w/ clang
- [ ] **Get UART working**
- [ ] Embed build system in the general make structure


## Getting started

- Download the simulator from <https://github.com/mupq/pqriscv-vexriscv.git>.
- Build and install openocd-vexriscv (https://github.com/SpinalHDL/openocd_riscv)

libyaml-dev

python3.7-dbg

## Workflow

- Start 3 terminals.
- In terminal 1: `cd` to `murax/`.
- In terminal 2 & 3: `cd` to your `pqriscv-vexriscv` repo.
- In terminal 1: run `make clean` and then `make`. This will build an example main file (currently located at `murax/main.c`).
  See `murax/Makefile` for more details.
- In terminal 2: run `sbt "runMain mupq.PQVexRiscvSim --ram 256,128"` to start the simulator.
  Note, the simulator will forever run, and will keep a core on your machine busy until it is terminated.
- In terminal 3: run `openocd-vexriscv --file pqvexriscvsim.cfg`
- In terminal 1: run the following GDB command:

```shell
/opt/riscv/bin/riscv64-unknown-elf-gdb -ex 'set remotetimeout 15' -ex 'target remote :3333' -ex 'load' -ex 'break main' -ex 'continue' main
# set remotetimeout 15      set the timout of the remote (openocd) to 15 seconds
# target remote :3333       connect to `openocd-vexriscv` on localhost:3333
# load                      load `main` onto the simulated core
# break main                set a breakpoint on main
# continue                  start running (until we hit main)
```

## Important notes

- ALWAYS call `make clean` before calling `make`.
- I chose to not call the contructors in libc! Do **not** rely on them!
