#include "../include/tcb.h"
#include "../include/taskPara.h"
#include "../include/task_sched.h"

//extern void initLeftExeTime_sjf(myTCB* tsk);

// may modified during scheduler_init
tskPara defaultTskPara = {
	.priority = 0,
	.exeTime = MAX_EXETIME,
	.arrTime = 0,
	.schedPolicy = SCHED_UNDEF
}; //task设计调度的一些参数的默认值

void _setTskPara(myTCB* task, tskPara* para)
{
	if (para == NULL)
		task->para = defaultTskPara;
	else
	{
		task->para = *para;
		switch (para->schedPolicy)
		{
		case SCHEDULER_FCFS:
			task->para.schedPolicy = SCHED_FCFS;
			break;
		case SCHEDULER_SJF:
			task->para.schedPolicy = SCHED_SJF;
			break;
		case SCHEDULER_PRIORITY:
			task->para.schedPolicy = SCHED_PRIO;
			break;
		default:
			task->para.schedPolicy = SCHED_UNDEF;
			break;
		}
	}
}

void initTskPara(tskPara* buffer)
{
	buffer->arrTime = 0;
	buffer->exeTime = MAX_EXETIME;
	buffer->priority = 0;
	buffer->schedPolicy = SCHED_UNDEF;
}

void setTskPara(unsigned int option, unsigned int value, tskPara* buffer)
{
	switch (option)
	{
	case ARRTIME:
		buffer->arrTime = value;
		break;
	case EXETIME:
		buffer->exeTime = value;
		break;
	case PRIORITY:
		buffer->priority = value;
		break;
	case SCHED_POLICY:
		buffer->schedPolicy = value;
		break;
	default:
		break;
	}
}

void getTskPara(unsigned option, unsigned int* para, tskPara* buffer)
{
	switch (option)
	{
	case ARRTIME:
		*para = currentTsk->para.arrTime;
		break;
	case EXETIME:
		*para = currentTsk->para.exeTime;
		break;
	case PRIORITY:
		*para = currentTsk->para.priority;
		break;
	case SCHED_POLICY:
		*para = currentTsk->para.schedPolicy;
		break;
	default:
		break;
	}
}