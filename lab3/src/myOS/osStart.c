extern void init8259A(void);
extern void init8253(void);
extern int myPrintk(int color,const char *format, ...);
extern void clear_screen(void);
extern void myMain(void);
extern void setWallClock(int h, int m, int s);

void osStart(void)
{
    __asm__ __volatile__("Call enable_interrupt");
    init8253();
    init8259A();
	clear_screen();
	setWallClock(12,0,0);
	myMain();
	while(1);
}
