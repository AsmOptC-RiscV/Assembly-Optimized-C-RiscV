#!/bin/sh
set -e

echo "This require at least 16 GB of free disk space."
apt install git autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev python-dbg python-dev pkg-config libusb-1.0-0-dev libyaml-dev

# get SBT
echo "deb https://dl.bintray.com/sbt/debian /" | tee -a /etc/apt/sources.list.d/sbt.list
curl -sL "https://keyserver.ubuntu.com/pks/lookup?op=get&search=0x2EE0EA64E40A89B84B2DF73499E82A75642AC823" | apt-key add
apt update
apt install sbt

git submodule init
git submodule update
cd Building_Tools/riscv-gnu-toolchain

# riscv-gnu-toolchain also contains submodules...
git submodule init
git submodule update
# we build newlib
./configure --enable-multilib --with-cmodel=medany --prefix=/opt/riscv
sudo make -j4 newlib

# we reconfigure gdb to also use python
cd riscv-gdb
./configure --host=x86_64-pc-linux-gnu --target=riscv64-unknown-elf --with-python=/usr/bin/python2.7 --prefix=/opt/riscv
sudo make -j4
sudo make install

# next we install riscv-openocd (for sifive)
cd ..
cd ..
cd riscv-openocd
./bootstrap
./configure --prefix=/opt/riscv --disable-werror
make -j4
sudo make install

# next we install verilator
cd ..
cd verilator
autoconf
./configure --prefix=/opt/verilator
make -j4
sudo make install

# next we install openocd but for verilator this time
cd ..
cd openocd_riscv
./configure --prefix=/opt/verilator --disable-werror
make -j4
sudo make install
cd ..
cd ..

# add user to dialout to be able to use USB devices
sudo usermod -a -G dialout $(whoami)

# we set up gdb to accept our .gdbinit
echo "set auto-load safe-path "$(pwd) >> ~/.gdbinit

# we install dependencies for python3 to run benchmarks
python3 -m pip install pyserial
python3 -m pip install progress

# we set up the symbolic links from rweather-repo to rweather for benchmarks
python3 gen-tree-rweather.py
