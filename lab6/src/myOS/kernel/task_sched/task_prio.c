#include "../../include/task_sched.h"

myTCB* rqPRIO;  //ready queue

void rqPRIOInit(void)
{
	rqPRIO = idleTsk;	// head <- idleTsk
	dLinkListInit((dLinkedList*)(&(rqPRIO->thisNode)));
}

myTCB* nextPRIOTsk(void)
{
	dLinkedList* head = (dLinkedList*)rqPRIO;
	dLinkedList* findnode = head;
	myTCB* nexttsk = (myTCB*)head;
	int i = 0;
	while ((findnode != head)||(i==0))//到链表尾部停止
	{
		findnode = findnode->next;//链表指针移动
		if ((((myTCB*)findnode)->para.priority > nexttsk->para.priority) || (i == 0))//指针指向优先级比nexttsk高
		{
			nexttsk = (myTCB*)findnode;//替换
			i++;
		}
	}
	return nexttsk;
}

void tskEnqueuePRIO(myTCB* tsk)
{
	dLinkInsertFifo((dLinkedList*)rqPRIO, (dLink_node*)tsk);
}

void tskDequeuePRIO(myTCB* tsk)
{
	dLinkDelete((dLinkedList*)rqPRIO, (dLink_node*)tsk);
}

void schedulerInit_PRIO(void)
{
	rqPRIOInit();
	/* default for all task except idleTsk*/
	defaultTskPara.schedPolicy = SCHED_PRIO;
	/* special for idleTsk*/
	_setTskPara(idleTsk, &defaultTskPara);
	idleTsk->para.schedPolicy = SCHED_IDLE;
}

struct scheduler scheduler_PRIO = {
	.type = SCHEDULER_PRIORITY,
	.nextTsk_func = nextPRIOTsk,
	.enqueueTsk_func = tskEnqueuePRIO,
	.dequeueTsk_func = tskDequeuePRIO,
	.schedulerInit_func = schedulerInit_PRIO,
	.createTsk_hook = NULL,
	.tick_hook = NULL
};

