#include "../include/dLinkList.h"
#include "../include/tcb.h"

/*

本框架中的task使用双向链表链接起来，链表的定义及操作可参考./lib/dLinkList.c
task按照arrTime从小到大排列
*/

/* for task arriving */
typedef struct myTCB myTCB;
extern void tskStart(myTCB* tsk);

extern int getTick(void);

/* time unit: tick */
/* zero arriving time: x ticks*/
unsigned int arrTimeBase = 0x0;

typedef struct arrivingNode
{
	/* node should be here*/
	dLink_node theNode;
	/* node body */
	unsigned int arrTime; //ralative to arrTimeBase
	myTCB* theTCB;
}arrNode;
arrNode arrPool[TASK_NUM];
arrNode* tcb2Arr(myTCB* tsk)
{
	return &arrPool[tsk->tcbIndex];
}

dLinkedList arrList;

void initArrList(void)
{
	dLinkListInit(&arrList);
}

/* arrTime: small --> big */
void ArrListEnqueue(myTCB* tsk)
{
	arrNode* arrnode = tcb2Arr(tsk);
	arrnode->arrTime = tsk->para.arrTime;
	arrnode->theTCB = tsk;
	dLinkedList* head = &arrList;
	dLinkedList* findnode = &arrList;
	arrNode* arr;
	findnode = findnode->next;
	arr = (arrNode*)findnode;
	while (findnode != head)//若是链表的队尾则结束循环
	{
		if (arrnode->arrTime <= arr->arrTime)
		{
			dLinkInsertBefore(head, findnode, (dLinkedList*)arrnode);
			return;
		}
		findnode = findnode->next;//指针后移
		arr = (arrNode*)findnode;
	}
	dLinkInsertBefore(head, findnode, (dLinkedList*)arrnode);
}

void tskStartDelayed(myTCB* tsk)
{
	//disable_interrupt();
	ArrListEnqueue(tsk);
	//enable_interrupt();
}

void tick_hook_arr(void)
{
	if (dLinkListIsEmpty(&arrList)) return;
	else
	{
		arrNode* tmp = (arrNode*)dLinkListFirstNode(&arrList);
		while (tmp->arrTime + arrTimeBase <= getTick())
		{
			dLinkDeleteFifo(&arrList);
			tskStart(tmp->theTCB);
			if (dLinkListIsEmpty(&arrList)) break;
			tmp = (arrNode*)dLinkListFirstNode(&arrList);
		}
	}
}