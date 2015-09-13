#!/bin/bash

DEVICE="/dev/loop0"
SO_PATH=`pwd`/../
KERNEL="$SO_PATH/kernel"
MEM=512

if [ "$1" = "-c" ]
then
        qemu-system-i386 -fda $DEVICE -curses -m $MEM
elif [ "$1" = "-g" ]
then
        qemu-system-i386 -fda $DEVICE -m $MEM
elif [ "$1" = "-d" ]
then
        qemu-system-i386 -fda $DEVICE -curses -S -s -m $MEM
elif [ "$1" = "-a" ]
then
        gdb -q $KERNEL/KERNELS -x ./gdbinit
else
        echo "This script is used to start the operation system using qemu."
        echo "Options:"
        echo -e "\t" "-c CLI (curses)"
        echo -e "\t" "-g GUI (graphical)"
        echo -e "\t" "-d debugging mode (GDB Server at 1234/TCP)"
        echo -e "\t" "-a debugging mode (Attach GDB at 1234/TCP)"

fi
