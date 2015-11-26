%include "common.inc"    ; basic input/output

[EXTERN isr_handler]
[EXTERN irq_handler]
[GLOBAL idt_install]
[GLOBAL gdt_install]
[GLOBAL tss_install]

idt_install:
        pop ebx
        pop eax
        lidt [eax]
        push    ebx
        ret

gdt_install:
        pop ebx
        pop eax
        lgdt [eax]
        mov     ax, 0x10
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
        push    ebx
        jmp     0x8:.flush
.flush:
        ret

tss_install:
        mov ax, 0x2B   ; index to our TSS struct, 0x28 (5th selector)
        ltr ax         ; 0x2b sets the RPL to 3.
        ret


%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push byte 0
    push %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

%macro IRQ 2		; define a macro, taking one parameter
  [GLOBAL irq%1]        ; %1 accesses the first parameter.
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

irq_common_stub:
        pusha

        mov ax, ds                      ; save data selector (can be from kernel or user)
        push eax
        mov ax, KERNEL_DATA_DESC        ; load the kernel data segment descriptor
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax        

        call irq_handler

        pop eax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        popa
        add esp, 8                      ; clean int number and error code
        sti                             ; set the interrupt
        iret

isr_common_stub:
        pusha

        mov ax, ds                      ; save data selector (can be from kernel or user)
        push eax
        mov ax, KERNEL_DATA_DESC        ; load the kernel data segment descriptor
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax        

        call isr_handler

        pop eax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        popa
        add esp, 8                      ; clean int number and error code
        sti                             ; set the interrupt
        iret

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

