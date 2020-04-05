extern unsigned char inb(unsigned short int port_from);
extern void outb (unsigned short int port_to, unsigned char value);
void io_wait(void)

{
    __asm__ __volatile__( "jmp 1f\n\t"
                           "1:jmp 2f\n\t"
                           "2:" );
}

void init8259A(void){

//FP
    outb(0x20, 0x11);
    io_wait();
    outb(0x21, 0x20);
    io_wait();
    outb(0x21, 0x04);
    io_wait();
    outb(0x21, 0x03);
    io_wait();

//SP
    outb(0xA0, 0x11);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();


    outb(0x21, inb(0x21) & ~1);

}
