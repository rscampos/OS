;*********************************************
;	A20.inc
;	- A20 functions 
;       Works for QEMU using int 0x15
;*********************************************

 
bits	16

ENABLEA20:

        ; query for support
        mov ax, 0x2403
        int 0x15

        ; status
        mov ax, 0x2402
        int 0x15

        ; disable
        mov ax, 0x2403
        int 0x15

        ; enable  
        mov ax, 0x2403
        int 0x15
        
        ret
