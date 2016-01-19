
/* Clear the screen. Send 0x20 to the
 * memory.
 */
void clear_screen();

/* Send one byte to the display.
 */
int put_char(char);

/* Send a string to the display.
 */
void puts(char *);

/* Mov the hardware cursor to the
 * current posistion in the screen.
 */
void mov_cursor();

/* Display a formated string with
 * variable parameters.
 */
void printf(char * fmt, ...);

/* Convert int to ASCII.
 */
void itoa(int, char * buf, int base);
void s_itoa(int num, char * buf, int base);

/* Copy from source to destination.
 */
void strcpy(char * dest, char * src);

/* Concat source with destination.
 */
void strcat(char * dest, char * src);

/* Counts how many bytes there is in the 
 * buffer.
 */
int strlen(const char * buf);

/* Chage the colors of background and foreground */
void vga_change_colors(u8int color);

/* Restore the default colors */
void vga_restore_colors();


void vga_banner_top(char * banner);
void vga_banner_bottom(char * banner);

/* Copy 'n' bytes between memory */
void memcpy(char *, char *, int);
