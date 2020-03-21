//实现myPrint功能，需要调用到格式化输出的function（vsprintf）
#include <stdarg.h>

extern void append2screen(char* str, int color);
extern void uart_put_chars(char* str);
extern int vsprintf(char* buf, const char* fmt, va_list args);
char kbuf[400];  //todo: fix me
int myPrintk(int color, const char* format, ...) {
	va_list args;
	int i;
	va_start(args, format);
	//将格式字符串写入到kbuf
	i = vsprintf(kbuf, format, args);
	//释放args
	va_end(args);
	append2screen(kbuf, color);
	uart_put_chars(kbuf);
	return i;
}

char ubuf[400];  //todo: fix me
int myPrintf(int color, const char* format, ...) {
	va_list args;
	int i;
	va_start(args, format);
	//将获取到的format格式字符串写入到buf这个缓存里去
	i = vsprintf(ubuf, format, args);
	//释放args
	va_end(args);
	append2screen(ubuf, color);
	uart_put_chars(ubuf);
	return i;
}
