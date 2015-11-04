;*********************************************
;       stage2.asm
;
;       Operating Systems Development Tutorial
;*********************************************


bits    16              ; We are still in 16 bit Real Mode
org     0x3000          ; After loading stage2 at 0x3000, stage2 jumps to it
start:  jmp Stage2

;----------------------------------------------------------
; functions
;----------------------------------------------------------

%include "stdio.inc"    ; basic input/output
%include "gdt.inc"      ; GDT table
%include "fat12.inc"    ; Fat12 Routines
%include "common.inc"	; some variables
%include "memory.inc"	; some variables
%include "A20.inc"	; A20 functions

;===============================;
; 	Stage2 Entry Point 	;
;===============================;

boot_info:
memoryLo               dw 1       ; memory size. Present if flags[0] is set
memoryHi               dw 1
mmap_length            dw 1     ; memory map. Present if flags[6] is set
mmap_addr              dw 1


Stage2:
        ;-------------------------------;
        ;   Setup segments and stack    ;
        ;-------------------------------;

        cli                             ; clear interrupts
        xor     ax, ax                  ; null segments
        mov     ds, ax
        mov     es, ax
        mov     ax, 0x9000              ; stack begins at 0x9000-0xffff
        mov     ss, ax
        mov     sp, 0xFFFF
        push    ax
        sti                             ; enable interrupts

        ;-------------------------------;
        ;   Print msg - RMode yet       ;
        ;-------------------------------;

        mov     si, msg
        call    PRINT16

	;-------------------------------;
        ;   Enable A20			;
        ;-------------------------------;

        call ENABLEA20
 
        ;-------------------------------;
        ;   Get memory Size	        ;
        ;-------------------------------;
        
        call    BiosGetMemorySize64MB
        mov     word [memoryLo], bx
        mov     word [memoryHi], ax
 
        ;-------------------------------;
        ;   Get memory map	        ;
        ;-------------------------------;
        
        mov     di, MULTIBOOT
        mov     word [mmap_addr],di
        call    BiosGetMemoryMap
        xor     edi, edi

	;-------------------------------;
        ;   Load the kernel image       ;
        ;-------------------------------;

	; sizerootdir
	mov     ax, 0x20			; size of each entry
        mul     WORD[bpbRootEntries]		; numbers of root entrys
        div     WORD[bpbBytesPerSector]		; size of sector in bytes
        mov     BYTE[sizerootdir], al		; store to use when we need
	
	; offsetrootdir
        mov     al, BYTE[bpbNumberOfFATs]	; number of fats (fat12 - 2)
        mul     WORD[bpbSectorsPerFAT]		; sector per fat (usually 9)
        add     ax, WORD[bpbReservedSectors]	; reserved sectors (usually 1)
	mov	BYTE[offsetrootdir], al		; offset to root dir
	
	; offsetdataregion
	add	al, BYTE[sizerootdir]		; size + offset RDT (Root Directory Table)
	mov	BYTE[offsetdataregion], al	; offset to data region (file & dir)(usually  33)

	call	LOAD_ROOT			; Load the RDT
	call	FIND				; Find the file
	call	LOAD_FAT			; Load the FAT to memory
	call	LOAD_IMAGE			; Load the image at 0x10000

        ;-------------------------------;
        ;   Load the GDT                ;
        ;-------------------------------;
        call INSTALLGDT

        ;-------------------------------;
        ;   Go into PMode               ;
        ;-------------------------------;
        cli
        mov     eax, cr0                ; get the current value of cr0
        or      eax, 0x1                ; change JUST the first bit (Bit 0) - PE bit
        mov     cr0, eax                ; change the value of cr0

	jmp	KERNEL_CODE_DESC:Stage3

bits    32
Stage3:

        ;-------------------------------;
        ;   Move the image to 1MB       ;
        ;-------------------------------;
	mov	ax, WORD[countersectors]	; how many sectors
	mov	bx, WORD[bpbBytesPerSector]	; bytes per sector
	mul	bx				; total of bytes
	mov	bx, 0x4
	div	bx
	mov	cx, ax				; for rep
	mov	esi, KERNELSTART
	mov	edi, IMAGEPMODE-0x1000          ; because we are using ELF, we
	cld                                     ; load 1M - 0x1000 for jump to
        mov     ax, 0x10
        mov     es, ax
        mov     ds, ax
	rep	movsd                           ; 1MB. The first byte is at 1M

        ;--------------------------------;
        ; Set the information for kernel ;
        ;--------------------------------;
        mov     eax, 0x2badb002
        mov     ebx, boot_info 
        call    IMAGEPMODE	                ; call the image kernell - elf

msg	DB      "[Stage2] Welcome to My Operating System!", 0
fileimg	DB      "KERNEL     ", 0		; Linux - ELF
