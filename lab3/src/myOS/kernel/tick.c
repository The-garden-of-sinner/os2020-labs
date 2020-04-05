#define SIZE 256
extern int myPrintf(int color,const char *format, ...);
int ticks=0;
typedef void(*fun)(void);
fun list[SIZE];
int cnt=0;

void set_timer_hook(void (*func)(void)){
    list[cnt++]=func;
}
void timer_hook_parse(void){
    for(int i=0;i<cnt;i++) list[i]();
}
void tick(void){
    ticks++;
    timer_hook_parse();
}

