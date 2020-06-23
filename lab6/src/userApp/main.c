#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"

void wallClock_hook_main(void)
{
	int _h, _m, _s;
	char hhmmss[] = "hh:mm:ss\0\0\0\0";

	getWallClock(&_h, &_m, &_s);
	sprintf(hhmmss, "%02d:%02d:%02d", _h, _m, _s);
	put_chars(hhmmss, 0x7E, 24, 72);
}

// should be named as scheduler_hook_main
// Be executed very early
void scheduler_hook_main(void)
{
	//prior settings
	//setSysScheduler(SCHEDULER_FCFS);
	//setSysScheduler(SCHEDULER_SJF);
	setSysScheduler(SCHEDULER_PRIORITY);
}

void doSomeTestBefore(void)
{
	setWallClock(18, 59, 59);		//set time 18:59:59
	setWallClockHook(&wallClock_hook_main);
}

void myTSK0(void)
{
	int j = 1;
	while (j <= 10)
	{
		myPrintf(0x7, "myTSK0::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
	tskEnd();   //the task is end
}

void myTSK1(void)
{
	int j = 1;
	while (j <= 10)
	{
		myPrintf(0x7, "myTSK1::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
	tskEnd();   //the task is end
}

void myTSK2(void)
{
	int j = 1;
	while (j <= 10)
	{
		myPrintf(0x7, "myTSK2::%d    \n", j);
		busy_n_ms(120);
		j++;
	}
	tskEnd();   //the task is end
}

void testScheduler(void)
{  

/*	if (getSysScheduler() != SCHEDULER_FCFS)
	{
		myPrintf(0x3, "NEED scheduler: SCHEDULER_FCFS!!!!!!STOPED!!!!!!!!");
		return;
	}
*/
	tskPara tskParas[4];
	int i;
	for (i = 0; i < 4; i++) initTskPara(&tskParas[i]);
	switch (getSysScheduler())
	{
	case(SCHEDULER_SJF): {
		myPrintf(0x3, "SJF\n");

		setTskPara(EXETIME, 100, &tskParas[0]);
		createTsk(myTSK0, &tskParas[0]);

		setTskPara(EXETIME, 0, &tskParas[1]);
		createTsk(myTSK1, &tskParas[1]);

		setTskPara(EXETIME, 50, &tskParas[2]);
		createTsk(myTSK2, &tskParas[2]);

		initShell();
		memTestCaseInit();
		setTskPara(EXETIME, 120, &tskParas[3]);
		createTsk(startShell, &tskParas[3]);
	}break;
	case(SCHEDULER_FCFS): {
		myPrintf(0x3, "FCFS\n");
		setTskPara(ARRTIME, 50, &tskParas[0]);
		createTsk(myTSK0, &tskParas[0]);

		setTskPara(ARRTIME, 100, &tskParas[1]);
		createTsk(myTSK1, &tskParas[1]);

		setTskPara(ARRTIME, 0, &tskParas[2]);
		createTsk(myTSK2, &tskParas[2]);

		initShell();
		memTestCaseInit();
		setTskPara(ARRTIME, 120, &tskParas[3]);
		createTsk(startShell, &tskParas[3]);
	}break;
	case(SCHEDULER_PRIORITY): {
		myPrintf(0x3, "PRIO\n");
		setTskPara(PRIORITY, 0, &tskParas[0]);
		createTsk(myTSK0, &tskParas[0]);

		setTskPara(PRIORITY, 2, &tskParas[1]);
		createTsk(myTSK1, &tskParas[1]);

		setTskPara(PRIORITY, 4, &tskParas[2]);
		createTsk(myTSK2, &tskParas[2]);

		initShell();
		memTestCaseInit();
		setTskPara(PRIORITY, 1, &tskParas[3]);
		createTsk(startShell, &tskParas[3]);
	}break;
	default: {
		myPrintf(0x3, "else\n");
		setTskPara(ARRTIME, 50, &tskParas[0]);
		createTsk(myTSK0, &tskParas[0]);

		setTskPara(ARRTIME, 100, &tskParas[1]);
		createTsk(myTSK1, &tskParas[1]);

		setTskPara(ARRTIME, 0, &tskParas[2]);
		createTsk(myTSK2, &tskParas[2]);

		initShell();
		memTestCaseInit();
		setTskPara(ARRTIME, 120, &tskParas[3]);
		createTsk(startShell, &tskParas[3]);
	}

		break;
	}

//    startShell();
}

void myMain(void)
{    //main is our init task
	clear_screen();

	doSomeTestBefore();
	myPrintf(0x7, "********INIT START\n\n");
	testScheduler();
	myPrintf(0x7, "********INIT END\n\n");

	tskEnd();  // init end	
}