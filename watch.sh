#!/bin/sh

DEVICE=/dev/ttyUSB1
BAUD=115200

exec screen -L -Logfile main.log "$DEVICE" $BAUD
