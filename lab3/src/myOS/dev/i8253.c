extern unsigned char inb(unsigned short int port_from);
extern void outb (unsigned short int port_to, unsigned char value);
extern void io_wait(void);
extern int myPrintk(int color, const char* format, ...);

void init8253(void){
    unsigned short para = 11932;//·ÖÆµ²ÎÊý
    unsigned char high,low;
    high = (para>>8)&0xff ;
    low = para & 0xff;
    myPrintk(0x7, "%d,%d\n",high,low);
    outb(0x43,0x34);
    io_wait();
    outb(0x40,low);
    io_wait();
    outb(0x40,high);
    io_wait();
}


