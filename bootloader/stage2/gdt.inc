;*********************************************
;	gdb.inc
;	- Install GDT (Global Descritor Table)
;*********************************************

 
bits	16

;===============================;
; Global Descriptor Table (GDT) ;
;===============================;

INSTALLGDT:
	cli
	lgdt	[toc]
	sti
	ret

;----------------------------------;
; Offset 0 in GDT: null descriptor ;
;----------------------------------;
gdt_data: 
	dd 0 				; null descriptor
	dd 0 
 
;------------------------------------;
; Kernel Space Code (Offset 0x8)     ;
;------------------------------------;
; gdt code:				; code descriptor
	dw 0FFFFh 			; limit low (2B)
	dw 0				; base low (2B)
	db 0 				; base middle (1B)
	db 10011010b 			; access byte (bits 5 e 6 - ring 0)
	db 11001111b 			; flags(4-bit) + limit middle(4-bit)
	db 0 				; base high (1B)
 
;------------------------------------;
; Kernel Space Data (Offset 0x10)    ;
;------------------------------------;
; gdt data:				; data descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 			; access byte (bits 5 e 6 - ring 0)
	db 11001111b 			; flags(4-bit) + limit middle(4-bit)
	db 0				; base high

;------------------------------------;
; User Space Code (Offset 0x18       ;
;------------------------------------;
; gdt code:				; code descriptor
	dw 0FFFFh 			; limit low (2B)
	dw 0				; base low (2B)
	db 0 				; base middle (1B)
	db 11111010b 			; access byte (bits 5 e 6 - ring 3)
	db 11001111b 			; flags(4-bit) + limit middle(4-bit)
	db 0 				; base high (1B)
 
;------------------------------------;
; User Space Data (Offset 0x20)      ;
;------------------------------------;
; gdt data:				; data descriptor
	dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 11110010b 			; access byte (bits 5 e 6 - ring 3)
	db 11001111b 			; flags(4-bit) + limit middle(4-bit)
	db 0				; base high
 
;-------------------------------------;
; toc: 6 bytes (GDT size and address) ;
;-------------------------------------;
end_of_gdt:
toc: 
	dw end_of_gdt - gdt_data - 1 	; limit (Size of GDT)
	dd gdt_data 			; base of GDT

;-----------------;
; GDT Descriptors ;
;-----------------;

%define NULL_DESC 0
%define KERNEL_CODE_DESC 0x8
%define KERNEL_DATA_DESC 0x10
%define USER_CODE_DESC 0x18
%define USER_DATA_DESC 0x20
