#!/bin/bash

DEVICE="/dev/loop0"
#DEVICE="/tmp/floppy.img"
SO_PATH=`pwd`/../
KERNEL="$SO_PATH/kernel"
MEM=128
#LOGS="-d cpu_reset -D ./qemu.log"
LOGS="-d pcall -D /tmp/qemu.log"

if [ "$1" = "-c" ]
then
        qemu-system-i386 $LOGS -fda $DEVICE -curses -m $MEM
elif [ "$1" = "-cm" ]
then
        qemu-system-i386 $LOGS -fda $DEVICE -curses -m $MEM -monitor tcp::4444,server

elif [ "$1" = "-g" ]
then
        qemu-system-i386 -fda $DEVICE -m $MEM
elif [ "$1" = "-d" ]
then
        qemu-system-i386 -fda $DEVICE -curses $LOGS -S -s -m $MEM

elif [ "$1" = "-dm" ]
then
        qemu-system-i386 -fda $DEVICE -curses $LOGS -S -s -m $MEM -monitor tcp::4444,server
elif [ "$1" = "-a" ]
then
        gdb -q -x ./gdbinit
elif [ "$1" = "-as" ]
then
        gdb -q $KERNEL/KERNELS -x ./gdbinit

else
        echo "This script is used to start the operation system using qemu.."
        echo "Options:"
        echo -e "\t" "-c CLI (curses)"
        echo -e "\t" "-cm CLI - Monitor (4444/TCP)"
        echo -e "\t" "-g GUI (graphical)"
        echo -e "\t" "-d debugging mode (GDB Server at 1234/TCP)"
        echo -e "\t" "-dm debugging mode (GDB Server at 1234/TCP) - Monitor (4444/TCP)"
        echo -e "\t" "-a debugging mode (Attach GDB at 1234/TCP)"
        echo -e "\t" "-as debugging mode /w symbols (Attach GDB at 1234/TCP)"

fi
