#!/bin/sh
GDB_PORT=3333

# Check argument.
if [ -z "$1" ]; then
    echo "$0 <binary>"
    exit 0
fi

if [ ! -f "$1" ]; then
    echo "$1 is not a valid file"
    exit 1
fi

# Retrieve dependencies.
. ./config.sh

# Check dependencies.
checkdep() {
    path_to_dep=$(command -v "$1")
    if [ ! -x "$1" ] && [ ! -x "$path_to_dep" ]; then
        echo "$1 does not seem to exist. Please set the right path in config.mk or add the directory to your \$PATH."
        exit 1
    fi
}
checkdep "$GDB"
checkdep "$OPENOCD"

echo $GDB
echo $OPENOCD

"$OPENOCD" \
    -s "sifive_sdk" \
    -f openocd.cfg & \
"$GDB" "$1" --batch \
    -ex "set remotetimeout 240" \
    -ex "target extended-remote localhost:$GDB_PORT" \
    -ex "monitor reset halt" \
    -ex "monitor flash protect 0 64 last off" \
    -ex "load" \
    -ex "monitor resume" \
    -ex "monitor shutdown" \
    -ex "quit" && \
echo "Done."
