#include "Interface.h"
#include "Init.h"
#include "malloc.h"
/*********************************************************

由于汇编的问题这里没有能够在主函数执行之前完成硬件的初始化所以这里需要重新定义
一个头文件来完成硬件的初始化的工作。








*******************************************************/
char Init(void)
{
	char isInit=0;
	LEDInit();
	delay_init();
	UsartInit(115200);
	
	PS2_Init();
	SPI2_Init();
	LCDInit();
	SD_Init();
	W25QXX_Init();
	my_mem_init(SRAMIN);
	return isInit;
}


