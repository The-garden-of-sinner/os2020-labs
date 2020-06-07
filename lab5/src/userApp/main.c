#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"
extern void tskEnd(void);
extern int createTsk(void (*tskBody)(void));


void wallClock_hook_main(void) {
	int _h, _m, _s;
	char hhmmss[] = "hh:mm:ss\0\0\0\0";

	getWallClock(&_h, &_m, &_s);
	sprintf(hhmmss, "%02d:%02d:%02d", _h, _m, _s);
	put_chars(hhmmss, 0x7E, 24, 72);
}

void myTsk0(void);
void myTsk1(void);
void myTsk2(void);
void startShell(void);

// init task 入口
void myMain(void) {
     clear_screen();

	 setWallClock(18, 59, 59);		//set time 18:59:59
	 setWallClockHook(&wallClock_hook_main);

     createTsk(myTsk0);
     createTsk(myTsk1);
     createTsk(myTsk2);

//#error "TODO: 初始化 shell 并创建 shell task"
	 initShell();
	 memTestCaseInit();

	 createTsk(startShell);
     tskEnd();
}
