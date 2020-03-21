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


int addr = vga_base; // addrΪ�������Դ��ַ
int cursor_x, cursor_y, cursor_pos; // ���λ��


void set_cursor_pos(int pos) {	//���ù��λ��
	outb(index_port, 0xe);
	outb(data_port, (pos >> 8) & 0xff);
	outb(index_port, 0xf);
	outb(data_port, pos & 0xff);
}


void append_char_raw(char c, int color) {
	if (cursor_x == buffer_height) {		//��Ļ�Ѿ�����
		addr = vga_base;
		for (int i = 0;i < buffer_height - 1;i++) {
			for (int j = 0;j < buffer_width / 2;j++) {
				__asm__ __volatile__("movl (%0),%%ebx"::"a"(addr + buffer_width * 2));	//���������ƶ�
				__asm__ __volatile__("movl %%ebx,(%0)"::"a"(addr));
				addr += long_width;		//movlÿ���ܰ���4��Byte
			}
		}
		for (int i = 0;i < buffer_width / 2;i++) {
			__asm__ __volatile__("movl $0,(%0)"::"a"(addr));
			addr += long_width;
		}
		cursor_pos -= buffer_width;
	}
	addr = vga_base + cursor_pos * 2;
	__asm__ __volatile__("movb %0,(%1)"::"a"(c), "b"(addr++));		//�ڶ�Ӧ��ַд���ַ�
	__asm__ __volatile__("movb %0,(%1)"::"a"(color), "b"(addr++));	//д����ɫ
	set_cursor_pos(++cursor_pos);									//������
	cursor_x = cursor_pos / buffer_width;
	cursor_y = cursor_pos % buffer_width;
}

void append_char(char c, int color) {//��������ַ�������\n�����������Ϊ�ո�����tab����2��
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
