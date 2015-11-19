bits 32
org 0x0804A000
        xor     eax, eax
        xor     ebx, ebx
        xor     ecx, ecx
        xor     edx, edx

        mov     eax, 4
        mov     ecx, hellos
        mov     edx, 6

        int 0x80

hellos  DB      "Hello from PROG2!!!", 0
