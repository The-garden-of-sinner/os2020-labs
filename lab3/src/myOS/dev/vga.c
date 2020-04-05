extern void disable_interrupt(void);
extern void enable_interrupt(void);

extern void uart_put_chars(unsigned char* str);
extern unsigned char uart_get_char(void);
extern unsigned char inb(unsigned short int port_from);
extern void outb(unsigned short int port_to, unsigned char value);

#define vga_base 0xB8000
#define vga_size 0x1000
#define index_port 0x3d4
#define data_port 0x3d5
#define buffer_width 80
#define buffer_height 24


int addr = vga_base; // video memory address
int cursor_pos; // cursor position


void set_cursor_pos(int pos) {
	outb(index_port, 0xe);
	outb(data_port, (pos >> 8) & 0xff);// set high 8 bits
	outb(index_port, 0xf);
	outb(data_port, pos & 0xff);// set low 8 bits
}

//put char with raw data
void append_char_raw(char c, int color) {
	if (cursor_pos / buffer_width == buffer_height) {// need to roll screen
		// copy data to previous lines
		addr = vga_base;
		for (int i = 0; i < buffer_height - 1; i++) {
			for (int j = 0; j < buffer_width / 2; j++) {
				__asm__ __volatile__("movl (%0),%%ebx"::"a"(addr + buffer_width * 2));
				__asm__ __volatile__("movl %%ebx,(%0)"::"a"(addr));
				addr += 4;
			}
		}
		// erase data in the last line
		for (int i = 0; i < buffer_width; i++) {
			__asm__ __volatile__("movw $0x0f20,(%0)"::"a"(addr));
			addr += 2;
		}
		cursor_pos=(buffer_height-1)*buffer_width;
	}
	// put char to cursor position
	addr = vga_base + cursor_pos * 2;
	__asm__ __volatile__("movb %0,(%1)"::"a"(c), "b"(addr++));
	__asm__ __volatile__("movb %0,(%1)"::"a"(color), "b"(addr++));
	set_cursor_pos(++cursor_pos);// set new cursor position
}

//put char with processed data
void append_char(char c, int color) {
	int tmp = buffer_width - cursor_pos % buffer_width;
	switch (c) {
	case '\t':// \t = put 4 space
		for (int i = 0; i < 4; i++) {
			append_char_raw(' ', color);
		}
		break;
	case '\n':// \n = fill the rest of current line with space
		for (int i = 0; i < tmp; i++) {
			append_char_raw(0x20, 0);
		}
		break;
	default:// directly put char
		append_char_raw(c, color);
		break;
	}
}

void clear_screen(void) {
	for (int i = 0; i < vga_size; i++) {
		__asm__ __volatile__("movw $0x0f20, (%0)"::"a"(addr));
		addr += 2;
	}

	set_cursor_pos(0);// initialize cursor position
	cursor_pos = 0;
	addr = vga_base;// initialize data cursor

}


void append2screen(char* str, int color) {
	for (int i = 0; str[i] != 0; i++) {
		append_char(str[i], color);
	}
}


void put_chars(char* str, int color, int line, int loc) {
	int addr = vga_base + (line * buffer_width + loc) * 2;
	int i = 0;
	while (str[i] != '\0') {
		char c = str[i];
		__asm__ __volatile__("movb %0,(%1)"::"a"(c), "b"(addr++));		//在对应地址写入字符
		__asm__ __volatile__("movb %0,(%1)"::"a"(color), "b"(addr++));	//写入颜色
		i++;
	}
}

void show_input(int color, char* str){
    int mem = vga_base + cursor_pos * 2;
	int i;
    for(i=0;str[i]!='\0';i++){
        __asm__ __volatile__("movb %0,(%1)"::"a"(str[i]), "b"(mem++));
        __asm__ __volatile__("movb %0,(%1)"::"a"(color), "b"(mem++));
    }
	__asm__ __volatile__("movb %0,(%1)"::"a"(' '), "b"(mem++));
	__asm__ __volatile__("movb %0,(%1)"::"a"(color), "b"(mem++));
	set_cursor_pos(cursor_pos+i);
}
