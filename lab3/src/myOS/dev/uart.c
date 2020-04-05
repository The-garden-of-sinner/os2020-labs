extern unsigned char inb(unsigned short int port_from);
extern void outb(unsigned short int port_to, unsigned char value);

#define uart_base 0x3F8

void uart_put_char(unsigned char c) {
	outb(uart_base, c);
}

unsigned char uart_get_char(void) {
	while (!(inb(uart_base + 5) & 1));// loop until data is ready
	return inb(uart_base);
}

void uart_put_chars(char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		outb(uart_base, str[i]);
	}
}