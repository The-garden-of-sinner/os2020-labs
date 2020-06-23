#include "../include/tcb.h"
#include "../include/task_sched.h"
#include "../include/irq.h"

extern myTCB* currentTsk;

unsigned int defaultSlice = 10;
unsigned int defaultRtSlice = 50;
/*RR    */ extern struct scheduler scheduler_RR;
/*FCFS  */ extern struct scheduler scheduler_FCFS;
/*SJF   */ extern struct scheduler scheduler_SJF;
/*PRIO0 */ extern struct scheduler scheduler_PRIO0;
/*PRIO  */ extern struct scheduler scheduler_PRIO;
/*MQ    */ extern struct scheduler scheduler_MQ;
/*FMQ   */ extern struct scheduler scheduler_FMQ;

//这里列举了常见的所有的调度算法 本实验只要求实现两个即可（不包含FCFS）

extern void context_switch(myTCB* prevTsk, myTCB* nextTsk);

struct scheduler* sysScheduler = &scheduler_FCFS;  // default

unsigned int getSysScheduler(void)
{
	return sysScheduler->type;
}

void setSysScheduler(unsigned int what)
{
	switch (what)
	{
	case SCHEDULER_FCFS:
		sysScheduler = &scheduler_FCFS;
		break;
	case SCHEDULER_PRIORITY:
		sysScheduler = &scheduler_PRIO;
		break;
	case SCHEDULER_SJF:
		sysScheduler = &scheduler_SJF;
		break;

	default:
		sysScheduler = &scheduler_FCFS;
		break;
	}
}

//由于RR涉及到设置时间片大小 所以需要如下的两个函数getSysSchedulerPara setSysSchedulerPara
//其他调度器则不需要
void getSysSchedulerPara(unsigned int who, unsigned int* para)
{
	switch (who)
	{
	case SCHED_RR_SLICE:
		*para = defaultSlice;
		break;
	case SCHED_RT_RR_SLICE:
		*para = defaultRtSlice;
		break;
	default:;
	}
}

void setSysSchedulerPara(unsigned int who, unsigned int para)
{
	switch (who)
	{
	case SCHED_RR_SLICE:
		defaultSlice = para;
		break;
	case SCHED_RT_RR_SLICE:
		defaultRtSlice = para;
		break;
	default:;
	}
}

//每一个调度器中集成了几个函数 参考./task_sched/task_fifo.c中的scheduler scheduler_FCFS的结构
//实现以下的nextTsk enqueueTsk dequeueTsk schedulerInit scheduler_tick
myTCB* nextTsk(void)
{
	if (sysScheduler->nextTsk_func)
		return sysScheduler->nextTsk_func();
	else
		return (myTCB*)NULL;
}

void enqueueTsk(myTCB* tsk)
{
	if (sysScheduler->enqueueTsk_func)
		sysScheduler->enqueueTsk_func(tsk);
}

void dequeueTsk(myTCB* tsk)
{
	if (sysScheduler->dequeueTsk_func)
		sysScheduler->dequeueTsk_func(tsk);
}

void createTsk_hook(myTCB* created)
{
	if (sysScheduler->createTsk_hook)
		sysScheduler->createTsk_hook(created);
}

extern void scheduler_hook_main(void);

void schedulerInit(void)
{
	scheduler_hook_main();
	if (sysScheduler->schedulerInit_func)
		sysScheduler->schedulerInit_func();
}

void scheduler_tick(void)
{
	if (sysScheduler->tick_hook)
		sysScheduler->createTsk_hook(currentTsk);
}

void schedule(void)
{
	static int idle_times = 0;
	myTCB* prevTsk, * nextTsk;
	disable_interrupt();

	prevTsk = currentTsk;
	nextTsk = sysScheduler->nextTsk_func();
	currentTsk = nextTsk;
	context_switch(prevTsk, nextTsk);

	enable_interrupt();
}