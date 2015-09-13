;*********************************************
;	stio.inc
;	- Input/Output routines
;*********************************************

 
bits	16

;---------------------------;
; Print 16-bit - pass to si ;
;---------------------------;
PRINT16:
        lodsb			; mov ds:si to al; si<-si+1
        or	al, al		; al=current character
        jz      PRINT16_DONE	; null terminator found
        mov     ah, 0eh		; get next character
        int     10h
        jmp     PRINT16

PRINT16_DONE:
        ret

;*********************************************
;       stdio32.inc
;       - Input/Output routines in 32-bit
;*********************************************


bits    32

%define VIDMEM  0xB8000         ; video memory
_CurX db 0x0                    ; current x location
_CurY db 0x0                    ; current y location0

%define         VIDMEM  0xB8000 ; video memory (Color text mode)
%define         COLS    80      ; width of screen (MDA mode)
%define         LINES   25      ; height of screen (MDA mode)
%define         CHAR_ATTRIB 0x34; character attribute (0011 0100)


;===========================================;
; Put the HW cursor in the current position ;
;===========================================;
MOVCUR:
        ;------------------------;
        ; get the linear address ;
        ;------------------------;
        xor     eax, eax
        xor     ebx, ebx
        mov     al, byte[_CurY] ; current position of y
        mov     bl, COLS        ; multiply by COLS
        mul     bl              ; ax - result
        mov     bl, byte[_CurX] ; current position of x
        add     ax, bx          ; ax - final linear address
        mov     bx, ax

        ;--------------------------------;
        ; set cursor location - low byte ;
        ;--------------------------------;

        mov     al, 0x0F        ; cursor location - low byte index
        mov     dx, 0x03D4      ; write it to the CRT index register
        out     dx, al          ; send to controller

        mov     dx, 0x03D5      ; write it to the CRT data register
        mov     al, bl
        out     dx, al          ; send to controller

        ;-----------------------------------;
        ; set cursor location - height byte ;
        ;-----------------------------------;

        mov     al, 0x0E        ; cursor location - height byte index
        mov     dx, 0x03D4
        out     dx, al

        mov     dx, 0x03D5
        mov     al, bh
        out     dx, al
        ret

;=============================;
; Clear the screen with space ;
;=============================;
CLEARSCREEN32:
        mov     al, 0x20        ; fill with ' ' - 0x20
        mov     ah, CHAR_ATTRIB ; set the background
        mov     edi, VIDMEM     ; start at VIDMEM
        mov     cx, COLS*LINES  ; cx - how many bytes to be clean
        rep     stosw           ; 2B by 2B
        mov     byte [_CurX], 0 ; reset address location of X
        mov     byte [_CurY], 0 ; reset address location of Y
        ret

PUTCH32:
        mov     edi, VIDMEM     ; get the video address

        ;----------------;
        ; linear address ;
        ;----------------;
        xor     eax, eax
        xor     edx, edx
        xor     ebx, ebx
        cmp     cl, 0x0A
        je      .NEWLINE
        mov     al, byte[_CurY]
        mov     bl, COLS
        mul     bl              ; result in ax
        mov     bl, byte[_CurX]
        add     ax, bx          ; ax - final result
        mov     bx, 0x2
        mul     bx              ; ax - position to put the char
        add     edi, eax
        mov     [edi], cl
        mov     cl, CHAR_ATTRIB
        mov     [edi+1], cl

        ;-----------------;
        ; update position ;
        ;-----------------;
        mov     al, byte[_CurX]
        inc     al
        cmp     al, 0x50        ; is it the end?
        je      .NEWLINE        ; if yes, update
        mov     byte[_CurX], al
        ret

        .NEWLINE:
                mov     al, 0x0         ; return to the begin
                mov     byte[_CurX], al
                mov     al, byte[_CurY]
                inc     al
                mov     byte[_CurY], al
                ret

;=============================;
; Print a string - put in esi ;
;=============================;
PRINT32:
        lodsb                   ; mov ds:si to al; si<-si+1
        or      al, al          ; al=current character
        jz      PRINT32_DONE    ; null terminator found
        mov     cl, al
        call    PUTCH32
        jmp     PRINT32

PRINT32_DONE:
        ret

