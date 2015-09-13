#!/bin/bash
if [ "$1" = "-h" ]
then
        echo "This script is used to compile the operation system."
        echo "Options:"
        echo -e "\t" "-v (verbose for error)"
        exit
fi

DEVICE="/dev/loop0"
IMAGE="floppy.img"
SO_PATH="/opt/operation"
SCRIPTS=$SO_PATH"/scripts"
BOOTLOADER="$SO_PATH/bootloader"
KERNEL="$SO_PATH/kernel"
STAGE1="$BOOTLOADER/stage1"
STAGE2="$BOOTLOADER/stage2"
TMP="/tmp"
DIR_FS="/media/fat12"
DIR_FILES="/opt/ownos/files"

echo "[+] Setup the directories:"
echo "  [-] Creating \"$TMP\" if there isn't."
mkdir -p $TMP
echo "  [-] Creating \"$DIR_FS\" if there isn't."
mkdir -p $DIR_FS
echo "  [-] Creating \"$DIR_FILES\" if there isn't."
mkdir -p $DIR_FILES

################################################################################
#####                               setdown                                #####
################################################################################

echo "[+] Umount the device: \"$DEVICE\"."
umount $DEVICE

echo "[+] Detaching the file associated with \"$DEVICE\"."
losetup -d $DEVICE

################################################################################
#####                               setdup                                 #####
################################################################################

echo "[+] Compiling the stage1."
nasm -f bin $STAGE1/stage1.asm -o $STAGE1/stage1

echo "[+] Compiling the stage2."
cd $STAGE2
nasm -f bin stage2.asm -o stage2

cp stage2 $DIR_FILES/STAGE2.SYS
cd - > /dev/null

echo "[+] Compiling the kernel."
cd $KERNEL

echo "  [-] Creating the kernel image (/w debugging symbols)."
rm -rf KERNELS > /dev/null
make clean 2> /dev/null > /dev/null

if [ "$1" = "-v" ]
then
        make -e "CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -ggdb"> $SCRIPTS/output.log 2> $SCRIPTS/error.log
else
        make -e "CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -ggdb"> /dev/null 2> /dev/null
fi

mv KERNEL KERNELS

echo "  [-] Creating the kernel image."
make clean 2> /dev/null > /dev/null
make -e "CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector"> /dev/null 2> /dev/null

cp KERNEL $DIR_FILES
cd - > /dev/null

echo "[+] Creating the floppy image \"$TMP/$IMAGE\"."
dd if=/dev/zero of=$TMP/$IMAGE bs=512 count=2880 2> /dev/null

echo "[+] Associating \"$IMAGE\" with \"$DEVICE\"."
losetup $DEVICE $TMP/$IMAGE

echo "[+] Formating \"$DEVICE\" for MS-DOS (FAT12)."
mkdosfs -F 12 $DEVICE > /dev/null

echo "[+] Mounting \"$DEVICE\" at \"$DIR_FS\" using MS-DOS."
mount -t msdos $DEVICE $DIR_FS

echo "[+] Copying the stage1 to the bootsector in \"$DEVICE\"."
dd if=$STAGE1/stage1 of=$DEVICE bs=512 count=2880 2> /dev/null

echo "[+] Flushing the \"$DEVICE\" (will update the image)."
blockdev --flushbufs $DEVICE

################################################################################
#####                           coying the files                           #####
################################################################################

echo "[+] Copying stage2, kernel and all the files to \"$DIR_FS\"."
cp $DIR_FILES/* $DIR_FS

echo "[+] Flushing the \"$DEVICE\" (will update the image)."
blockdev --flushbufs $DEVICE
echo "[+] Operation System ready to launch!!!"
