#include "../include/task.h"
#include "../include/myPrintk.h"
#include "../include/uart.h"

void schedule(void);
void destroyTsk(int takIndex);

unsigned long** prevTSK_StackPtr;
unsigned long* nextTSK_StackPtr;

/**
 * 内部接口参考
 */
#define TSK_RDY 0
#define TSK_NEW 1
#define TSK_FREE 2

typedef struct rdyQueueFCFS {
	myTCB* head;
	myTCB* tail;
	myTCB* idleTsk;
} rdyQueueFCFS;

rdyQueueFCFS rqFCFS;          //队列

void rqFCFSInit(myTCB* idleTsk) {
	rqFCFS.head = (void*)0;
	rqFCFS.tail = (void*)0;
	rqFCFS.idleTsk = idleTsk;
}

int rqFCFSIsEmpty(void) {
	//if empty return 1
	return (((rqFCFS.head == (void*)0)) && (rqFCFS.tail == (void*)0));
}

myTCB* nextFCFSTsk(void) {
	if (rqFCFSIsEmpty()) return rqFCFS.idleTsk;
	else return rqFCFS.head;
}

/* tskEnqueueFCFS: insert into the tail node */
void tskEnqueueFCFS(myTCB* tsk) {
	if (rqFCFSIsEmpty()) {
		rqFCFS.head = tsk;
	}
	else rqFCFS.tail->next = tsk;
	rqFCFS.tail = tsk;
}

//********************************************

/* tskDequeueFCFS: delete the first node */
void tskDequeueFCFS(myTCB* tsk) {
	rqFCFS.head = rqFCFS.head->next;
	if (tsk == rqFCFS.tail) rqFCFS.tail = (void*)0;
}

// 用于初始化新创建的 task 的栈
// 这样切换到该任务时不会 stack underflow
void stack_init(unsigned long** stk, void (*task)(void)) {
	*(*stk)-- = (unsigned long)0x08;
	*(*stk)-- = (unsigned long)task;
	*(*stk)-- = (unsigned long)0x0202;

	*(*stk)-- = (unsigned long)0xAAAAAAAA;
	*(*stk)-- = (unsigned long)0xCCCCCCCC;
	*(*stk)-- = (unsigned long)0xDDDDDDDD;
	*(*stk)-- = (unsigned long)0xBBBBBBBB;

	*(*stk)-- = (unsigned long)0x44444444;
	*(*stk)-- = (unsigned long)0x55555555;
	*(*stk)-- = (unsigned long)0x66666666;
	*(*stk) = (unsigned long)0x77777777;
}

/**
 * 必须实现的外部接口
 */
void tskStart(myTCB* tsk) {

	tsk->state = TSK_RDY;
	tskEnqueueFCFS(tsk);
}

void tskEnd(void) {

	tskDequeueFCFS(currentTsk);
	destroyTsk(currentTsk->tcbIndex);
	schedule();
}

/* createTsk
 * tskBody():
 * return value: taskIndex or, if failed, -1
 */
int createTsk(void (*tskBody)(void)) {

	myTCB* tsk = firstFreeTsk;
	if (tsk == (void*)0)//没有空闲块
		return -1;           
	int i;
	tsk->state = TSK_NEW;           
	stack_init(&(tsk->stkTop), *tskBody);

	for (i = 1; i < TASK_NUM; i++) {
		if ((tcbPool[i].state) == TSK_FREE) {
			firstFreeTsk = &tcbPool[i];
			break;
		}
	}
	if (i == TASK_NUM)		//没有空闲块
		firstFreeTsk = (void*)0;    
	tskStart(tsk);
	return tsk->tcbIndex;            

}

/* destroyTsk
 * takIndex:
 * return value: void
 */
void destroyTsk(int takIndex) {

	tcbPool[takIndex].state = TSK_FREE;       //释放对应的TCB
	int i;
	for (i = 1; i < TASK_NUM; i++) {
		if (tcbPool[i].state == TSK_FREE) {
			firstFreeTsk = &tcbPool[i];
			return;
		}
	}

}


void context_switch(myTCB* prevTsk, myTCB* nextTsk) {
	CTX_SW(prevTsk->stkTop, nextTsk->stkTop);
}

void scheduleFCFS(void) {
	prevTSK_StackPtr = currentTsk->stkTop;            //切换到下一个任务
	currentTsk = nextFCFSTsk();
	nextTSK_StackPtr = currentTsk->stkTop;
	CTX_SW(prevTSK_StackPtr, nextTSK_StackPtr);

}

void schedule(void) {
	scheduleFCFS();
}

/**
 * idle 任务
 */
void tskIdleBdy(void) {       
	while (1) {
		schedule();
	}
}

unsigned long BspContextBase[STACK_SIZE];
unsigned long* BspContext;

//start multitasking
void startMultitask(void) {
	BspContext = BspContextBase + STACK_SIZE - 1;
	prevTSK_StackPtr = &BspContext;
	currentTsk = nextFCFSTsk();
	nextTSK_StackPtr = currentTsk->stkTop;
	CTX_SW(prevTSK_StackPtr, nextTSK_StackPtr);

}

void TaskManagerInit(void) {

	int i;
	myTCB* thisTCB;

	for (i = 0;i < TASK_NUM; i++) {
		thisTCB = &tcbPool[i];
		thisTCB->tcbIndex = i;
		thisTCB->state = TSK_FREE;      //任务块都空闲
		if (i == TASK_NUM - 1) thisTCB->next = (myTCB*)0;
		else thisTCB->next = &tcbPool[i + 1];
		thisTCB->stkTop = thisTCB->stack + STACK_SIZE - 1;
	}
	idleTsk = &tcbPool[0];
	stack_init(&(idleTsk->stkTop), tskIdleBdy);
	rqFCFSInit(idleTsk);
	firstFreeTsk = &tcbPool[1];
	createTsk(initTskBody);
	myPrintk(0x2, "start Multitask........");
	startMultitask();
	myPrintk(0x2, "stop Multitask........");

}
