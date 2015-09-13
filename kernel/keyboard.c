
#include "common.h"
#include "isr.h"
#include "keyboard.h"

void keyboard_callback(registers_t regs){
        u8int keybuffer;       
        //printf("[HAL] Keyboard: One keystroke was pressed.\n");
        keybuffer = in(KB_DATA);
        if (keybuffer < 0x80){
                printf("[HAL] Keyboard: %x\n", keybuffer);
                put_char(keystroker[keybuffer]);
        }
        //keybuffer = in(KB_COMMAND);
        //printf("[HAL] Keyboard: %x\n", keybuffer);
}

void init_keyboard(){
        //puts("    [+] Keyboard Driver loaded!\n");
        register_interrupt_handler(IRQ1,&keyboard_callback);
}
