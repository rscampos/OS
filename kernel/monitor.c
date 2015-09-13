#include "common.h"
#include "io.h"

u8int _CurX = 0;
u8int _CurY = FIRST_LINE;

u8int global_char_attrib = 0x02;

void vga_banner_bottom(char * banner){
        u8int C_CurX, C_CurY;

        C_CurX = _CurX;
        C_CurY = _CurY;

        _CurX   = 0;
        _CurY   = 24;           /* last line */

        vga_change_color(0x4F);
        puts(banner);
        vga_restore_color();

        _CurX = C_CurX;
        _CurY = C_CurY;
}

void vga_banner_top(char * banner){
        u8int C_CurX, C_CurY;


        C_CurX = _CurX;
        C_CurY = _CurY;

        _CurX   = 0;
        _CurY   = 0;

        vga_change_color(0x4F);
        puts(banner);
        vga_restore_color();

        _CurX = C_CurX;
        _CurY = C_CurY;
}

void vga_change_color(u8int color){
        global_char_attrib = color;
}

void vga_restore_color(){
        global_char_attrib = CHAR_ATTRIB;
}

void mov_cursor(){
        u16int position;
        position = (_CurY*COLS + _CurX);

        /* Low byte*/
        outb(0x03D4, 0x0F);
        outb(0x03D5, (u8int) position);

        /* High byte*/
        outb(0x03D4, 0x0E);
        outb(0x03D5, (u8int) (position >> 8));
}

void clear_screen(){
        char * video = VIDMEM;
        int i;
        int start, end;
        start   = COLS*2;
        end     = (COLS * (LINES-1))*2;
        //for(i=0; i < (COLS * LINES)*2 ;i++){
        for(i=start; i < end ;i++){
                video[i] = 0x20;
                video[++i] = global_char_attrib;
        }
        _CurX = 0;
        _CurY = 1;
        mov_cursor();
}

int put_char(char * character){
        char * video = VIDMEM;
        u16int position;

        if(character == '\n'){
                _CurX = 0;
                _CurY++;
                mov_cursor();
                return 0;
        }
        if(character == '\b'){
                _CurX--;
                put_char(' ');
                _CurX--;
                mov_cursor();
                return 0;
        }
        position = (_CurY*COLS + _CurX)*2;
        video[position] = character;
        video[position+1] = global_char_attrib;
        _CurX++;

        if(_CurX == 80){
                _CurX = 0;
                _CurY++;
        }
        mov_cursor();
        return 0;

}

void puts(char * string){
        int i=0;
        while(string[i]!='\0'){
                put_char(string[i]);
                i++;
        }
}

void va_start(char ** ptr_fmt, int * args){
        int i=0, count=0, length=0;
        char temp[32];
        int * end_var;
        int valor;
        char character;
        char * fmt, first_fmt, second_fmt;
        fmt = *ptr_fmt;
        while(fmt[i]){
                if(fmt[i]=='%'){
                        i++;
                        /* ptr_fmt is the pointer to the format string, so
                         * we encrease it by 4 and get the first parameter.
                         * ptr_fmt is a pointer. Since pointer is 4 bytes
                         * longer, the increase of ptr_fmt by 1 (ptr_fmt+1)
                         * will increase the address by 4.
                         */
                        count++;
                        end_var = (int) ptr_fmt + (count*4);
                        switch(fmt[i++]){
                                case 'x':
                                case 'X':
                                        valor = *end_var;
                                        itoa(valor,temp,16);
                                        puts(temp);
                                        break;
                                case 'c':
                                        character = *end_var;
                                        put_char(character);
                                        break;                     
                                case 's':
                                        valor = *end_var;
                                        puts(valor);
                                        break;                     
                                case 'i':
                                case 'd':
                                        /* end_var must be a interger pointer
                                         * to be able to use *end_var and get
                                         * the value from the address.
                                         */
                                        valor = *end_var;
                                        itoa(valor,temp,10);
                                        puts(temp);
                                        break;                     

                                default:
                                        puts("\n Caracter não reconhecido!\n");
                        }
                }else{
                        put_char(fmt[i++]);
                }
        }
}

void printf(char * fmt, ...){
        int args;
        va_start(&fmt, &args);

}

void strcpy(char * dest, char * src){
        int i=0;
        while(src[i] != '\0'){
                dest[i]=src[i];
                i++;
        }
        dest[i]='\0';
}

void memcpy(char * dest, char * src, int n){
        int i=0;
        for(i;i<n;i++)
                dest[i]=src[i];
}
/*
void strcmp(char *s1, char *s2){
        int i=0;
        while(s1[i]!=s2[i])
}
*/
void strcat(char * dest, char * src){
        int cdest=0, csrc=0;
        while(dest[cdest]!='\0'){
                cdest++;
        }

        while(src[csrc]!='\0'){
                dest[cdest] = src[csrc];
                csrc++;
                cdest++;
        }
        dest[cdest] = '\0';
}

int strlen(char * buf){
        int i=0;
        while(buf[i]!='\0')
                i++;
        return i-1;
}

void itoa(int num, char * buf, int base){
        char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        int size=12;
        char character[size];
        char * pointer;
        character[--size]=0x0;
        bool negative = FALSE;
        int res=1, rem, div, calc;

        if(num < 0){
                negative = TRUE;
                num = num ^ 0xFFFFFFFF;
                num = num + 1;
        }

        calc = num;

        while(res){
                res = calc/base;
                rem = calc - (base*res);
                character[--size] =  bchars[rem];
                calc = res;
        }

        if(negative)
                character[--size] = '-';

        pointer = &character[size];
        strcpy(buf, pointer);
}

// TODO
// // scrool - move the line up
//
