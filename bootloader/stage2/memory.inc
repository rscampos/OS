;*********************************************
;	memory.inc
;	- routines for memory stuffs
;*********************************************


bits	16

;--------------------------------------------
;       Memory map entry structure
;--------------------------------------------

struc   MemoryMapEntry
        .baseAddress            resq    1
        .length                 resq    1
        .type                   resd    1
        .acpi_null              resd    1
endstruc

;---------------------------------------------
;       Get memory size for >64M configuations
;       ret\ ax=KB between 1MB and 16MB
;       ret\ bx=number of 64K blocks above 16MB
;       ret\ bx=0 and ax= -1 on error
;---------------------------------------------

BiosGetMemorySize64MB:
        push    ecx
        push    edx
        xor     ecx, ecx        ;clear all registers. This is needed for testing later
        xor     edx, edx
        mov     ax, 0xe801
        int     0x15    
jc      .error
        cmp     ah, 0x86        ;unsupported function
je      .error
        cmp     ah, 0x80        ;invalid command
je      .error
jcxz    .use_ax                 ;bios may have stored it in ax,bx or cx,dx. test if cx is 0
        mov     ax, cx          ;its not, so it should contain mem size; store it
        mov     bx, dx

.use_ax:
        pop     edx             ;mem size is in ax and bx already, return it
        pop     ecx
        ret

.error:
        mov     ax, -1
        mov     bx, 0
        pop     edx
        pop     ecx
        ret

;-------------------------------;
; Bios Get Memory Map - Regions ; 
;-------------------------------;

BiosGetMemoryMap:
        pushad
        mov     eax, 0xE820     ; function 'Get Memory Map'
        xor     ebx, ebx        ; 0 - start at beginning of map
        mov     ecx, 0x14       ; size of the buffer - must be 20 bytes
        mov     edx, 'PAMS'     ; must contain these 4 bytes
        int     0x15
        jc      .error          ; CF is clear if successful
        cmp     eax, 'PAMS'     ; eax='PAMS' if successful
        jne     .error
        test    ebx, ebx        ; offset of next entry to copy from or 0 if done
        je      .done
        jmp     .next_entry
.next_entry:
        add     di, 0x14
        mov     eax, 0xE820
        mov     ecx, 0x14
        mov     edx, 'PAMS'
        mov     word[mmap_length],bx    ; salve bx before the interrupt
        int     0x15
        jc      .error          ; CF is clear if successful
        cmp     eax, 'PAMS'     ; eax='PAMS' if successful
        jne     .error
        test    ebx, ebx
        jne     .next_entry
        jmp     .done
.error:
        jmp .done
.done:
        popad
        ret
