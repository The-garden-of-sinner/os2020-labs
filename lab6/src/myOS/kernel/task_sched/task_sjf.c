#include "../../include/task_sched.h"

myTCB* rqSJF;  //ready queue

void rqSJFInit(void)
{
	rqSJF = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList*)(&(rqSJF->thisNode)));
}

myTCB* nextSJFTsk(void)
{
	dLinkedList* head = (dLinkedList*)rqSJF;
	dLinkedList* findnode = head;
	myTCB* nexttsk = (myTCB*)head;
	int i = 0;
	while ((findnode != head) || (i == 0))//到链表尾部停止
	{
		findnode = findnode->next;//链表指针移动
		if ((((myTCB*)findnode)->para.exeTime < nexttsk->para.exeTime) || (i == 0))//剩余时间变化
		{
			nexttsk = (myTCB*)findnode;
			i++;
		}
	}
	return nexttsk;
}

void tskEnqueueSJF(myTCB* tsk)
{
	dLinkInsertFifo((dLinkedList*)rqSJF, (dLink_node*)tsk);
}

void tskDequeueSJF(myTCB* tsk)
{
	dLinkDelete((dLinkedList*)rqSJF, (dLink_node*)tsk);
}

void schedulerInit_SJF(void)
{
	rqSJFInit();
	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_SJF;
	/* special for idleTsk*/
	_setTskPara(idleTsk, &defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;
}

struct scheduler scheduler_SJF = {
	.type = SCHEDULER_SJF,
	.nextTsk_func = nextSJFTsk,
	.enqueueTsk_func = tskEnqueueSJF,
	.dequeueTsk_func = tskDequeueSJF,
	.schedulerInit_func = schedulerInit_SJF,
	.createTsk_hook = NULL,
	.tick_hook = NULL
};

