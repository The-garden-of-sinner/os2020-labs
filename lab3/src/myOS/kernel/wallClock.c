
int hh=0, mm=0, ss=0;
extern void set_timer_hook(void (*func)(void));
extern int ticks;
extern int myPrintk(int color, const char* format, ...);
extern int putf(int color, const char* format, ...);

void maybeUpdateWallClock(void)
{
        if(ticks%100) return;
		//100次tick记为1秒
        ss=(ss+1)%60;
        if(!ss) mm=(mm+1)%60;
        if(!mm && !ss) hh=(hh+1)%24;
		//计时
        putf(0x20, "%02d : %02d : %02d",hh,mm,ss);
}

void setWallClock(int h, int m, int s)
{
        if(h<0 || h>23) return;
        if(m<0 || m>59) return;
        if(s<0 || s>59) return;//超出范围

        hh=h,mm=m,ss=s;
        set_timer_hook(maybeUpdateWallClock);
        putf(0x20, "%02d : %02d : %02d",hh,mm,ss);
}

void getWallClock(int *h, int *m, int *s)
{
        *h = hh;
        *m = mm;
        *s = ss;
}

