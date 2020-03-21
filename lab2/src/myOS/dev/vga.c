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
#define long_width 4


int addr = vga_base; // addr为操作的显存地址
int cursor_x, cursor_y, cursor_pos; // 光标位置


void set_cursor_pos(int pos) {	//设置光标位置
	outb(index_port, 0xe);
	outb(data_port, (pos >> 8) & 0xff);
	outb(index_port, 0xf);
	outb(data_port, pos & 0xff);
}


void append_char_raw(char c, int color) {
	if (cursor_x == buffer_height) {		//屏幕已经满了
		addr = vga_base;
		for (int i = 0;i < buffer_height - 1;i++) {
			for (int j = 0;j < buffer_width / 2;j++) {
				__asm__ __volatile__("movl (%0),%%ebx"::"a"(addr + buffer_width * 2));	//逐行往上移动
				__asm__ __volatile__("movl %%ebx,(%0)"::"a"(addr));
				addr += long_width;		//movl每次能搬运4个Byte
			}
		}
		for (int i = 0;i < buffer_width / 2;i++) {
			__asm__ __volatile__("movl $0,(%0)"::"a"(addr));
			addr += long_width;
		}
		cursor_pos -= buffer_width;
	}
	addr = vga_base + cursor_pos * 2;
	__asm__ __volatile__("movb %0,(%1)"::"a"(c), "b"(addr++));		//在对应地址写入字符
	__asm__ __volatile__("movb %0,(%1)"::"a"(color), "b"(addr++));	//写入颜色
	set_cursor_pos(++cursor_pos);									//光标后移
	cursor_x = cursor_pos / buffer_width;
	cursor_y = cursor_pos % buffer_width;
}

void append_char(char c, int color) {//输出单个字符，遇到\n将该行其后置为空格，遇到tab缩进2格
	int tmp = buffer_width - cursor_y;
	switch (c)
	{
	case '\t':
		for (int i = 0;i < 2;i++) {
			append_char_raw(' ', color);
		}
		break;
	case '\n':
		for (int i = 0;i < tmp;i++) {
			append_char_raw(0x20, 0);
		}
		break;
	default:
		append_char_raw(c, color);
		break;
	}
}

void clear_screen(void) {
	for (int i = 0;i < vga_size;i++) {
		__asm__ __volatile__("movl $0x0f200f20, (%0)"::"a"(addr));//
		addr += long_width;
	}
	set_cursor_pos(0);
	cursor_pos = cursor_x = cursor_y = 0;
	addr = vga_base;
}


void append2screen(char* str, int color) {
	for (int i = 0;str[i] != 0;i++) {
		append_char(str[i], color);
	}
}
