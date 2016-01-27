bits 32
org 0x0804A000

	;xor eax, eax
	;l1:
	;	inc eax
	;	cmp eax, 0xFFFFFFFF
	;	jg 	exit
	;jmp l1
	;exit:
	
	;;;;;;;; 1;;;;;;;;;;
	;----- write -------
	xor	eax, eax
        mov     eax, 4
        mov     ecx, hello1
        mov     edx, 6

        int 0x80

	;------ change -----
	xor	eax, eax
	int 0x80

	;;;;;;;; 2;;;;;;;;;;
	;----- write -------
	xor	eax, eax
        mov     eax, 4
        mov     ecx, hello2
        mov     edx, 6

        int 0x80

	;------ change -----
	xor	eax, eax
	int 0x80

	;;;;;;;; 3;;;;;;;;;;
	;----- write -------
	xor	eax, eax
        mov     eax, 4
        mov     ecx, hello3
        mov     edx, 6

        int 0x80

	;------ exit -----
	xor	eax, eax
	mov	eax, 1
	int 0x80


hello1  DB      "Hello 1! from PROG4", 0
hello2  DB      "Hello 2! from PROG4", 0
hello3  DB      "Hello 3! from PROG4", 0
