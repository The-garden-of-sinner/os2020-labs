#ifndef __TICK_H__
#define __TICK_H__

int ticks;
typedef void(*fun)(void);
void set_timer_hook(void (*func)(void));
void tick(void);

#endif