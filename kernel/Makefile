# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

SOURCES=boot.o interrupt.o main.o monitor.o io.o descriptor_table.o isr.o hal.o execptions.o pmm.o kheap.o common.o paging.o floppy.o vfs.o fat12.o process.o timer.o syscall.o
#SOURCES=boot.o interrupt.o main.o monitor.o io.o descriptor_table.o isr.o keyboard.o hal.o execptions.o pmm.o kheap.o common.o paging.o floppy.o vfs.o fat12.o process.o timer.o
#CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link 

clean:
	-rm *.o KERNEL

link:
	ld $(LDFLAGS) -o KERNEL $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
