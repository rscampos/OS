;
; boot.s -- Kernel start location. Also defines multiboot header.
; Based on Bran's kernel development tutorial file start.asm
;

[BITS 32]                       ; All instructions should be 32-bit.
[GLOBAL start]                  ; Kernel entry point.
[EXTERN setuphal]               ; Set up the kernel routines
[EXTERN main]                   ; This is the entry point of our C code
[EXTERN end]                    ; address of 'end' from linker
[EXTERN bss]                    ; address of 'bss' from linker
[EXTERN code]                   ; address of 'code' from linker
[EXTERN data]                   ; address of 'data' from linker
 
start:
	; Execute the kernel:
	cli			; Disable interrupts.
        mov 	ax, 0x10	; offset 0x10 - data selector?
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax
	mov 	ss, ax
	mov 	esp, 0x90000	; set the stack
	mov	ebp, esp
	push	ebx             ; pushes Multiboot information structure
        call    setuphal
        call 	main		; call our main() function.
	jmp $			; Enter an infinite loop, to stop the processor
                              	; executing whatever rubbish is in the memory
				; after our kernel!
