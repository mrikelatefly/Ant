#include "Interface.h"
#include "Init.h"
#include "malloc.h"
/*********************************************************

由于汇编的问题这里没有能够在主函数执行之前完成硬件的初始化所以这里需要重新定义
一个头文件来完成硬件的初始化的工作。








*******************************************************/
typedef struct{
	void *Function;
	char Info[50];
	void *CallBack;
	char CallBackInfo[50];
}__InitStep;
#define InitStepSize	10


__InitStep InitStep[InitStepSize]={
	{UsartInit,"Uart has Init!\n\r",0,0},
	{delay_init,"delay_init!\n\r",0,0},
	{SPI1_Init,"LED has Init!\n\r",0,0},
	{SPI2_Init,"LED has Init!\n\r",0,0},
	{MicroSDInit,"MicroSD has Init!\n\r",0,0},
	{VS_Init,"LED has Init!\n\r",0,0},
	{KeyInit,"KEY has Init!\n\r",0,0},
	{LEDInit,"LED has Init!\n\r",0,0,},
	//{LCDInit,"LED has Init!\n\r",0,0},
};
/*

	UsartInit(115200);
	LEDInit();
	delay_init();
	SPI2_Init();
	SPI1_Init();
	
	//LCDInit();
	SD_Init();
	VS_Init();
	W25QXX_Init();
	KeyInit();
	//my_mem_init(SRAMIN);
*/
char Init(void)
{
	char isInit=0;
//	char Step=0;
//	for(Step=0;Step<InitStepSize;Step++)
//	{
//		if(InitStep[Step].Function!=0)
//		{
//			(*(void(*)())InitStep[Step].Function)();
//			printf("%s",InitStep[Step].Info);
//				if(InitStep[Step].CallBack!=0)
//				{
//					(*(void(*)())InitStep[Step].CallBack)();
//					printf("%s",InitStep[Step].CallBackInfo);
//				}
//		}
		
//	}
	SPI2_Init();
	SPI1_Init();
	UsartInit(115200);
	delay_init();
	KeyInit();
	LEDInit();		  			//初始化与LED连接的硬件接口
	KeyInit();					//初始化按键
	LCD_Init();			   	//初始化LCD     
	W25QXX_Init();				//初始化W25Q128
 	VS_Init();	  				//初始化VS1053 
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
	//my_mem_init(SRAMIN);
	return isInit;
}


