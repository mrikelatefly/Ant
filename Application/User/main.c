#include "Interface.h"
#include "FrameBox.h"
#include "StringOperation.h"
#include "Init.h"
#include "ff.h"	
#include "malloc.h"
int x=0,y=0;

void drawParten(void);
u8 exfuns_init(void);
extern char MicroSDDataBuff[512];//一个扇区的大小
u16 color=0x000;
extern char MicroSDDataBuff[512];

FATFS *fs[_VOLUMES];//逻辑磁盘工作区.	 
FIL *file;	  		//文件1
FIL *ftemp;	  		//文件2.
UINT br,bw;			//读写变量
FILINFO fileinfo;	//文件信息
DIR dir;  			//目录

u8 *fatbuf;			//SD卡数据缓存区


void MicroSDTest()
{
	//这是在内存卡初始化以后的测试进行读写如果不成功还是从读写序列号开始吧
	
	 //WriteSectorToMicroSD(2,"Hello I here did you find me!!!");
	 //delay_ms(200);
	 ReadSectorFromMicroSD(2,MicroSDDataBuff);
   printf("MicroSD read is:%s!!!!\n\n",MicroSDDataBuff);
	
}

void W25Q128Test()
{
	char arr[100]={0};
	W25QXX_Read((u8 *)arr,1024,sizeof(arr));
	printf("W25QRead:%s\n\n",arr);
}

void FrameTest()
{
	/*
void FrameClear(u16 Color);
void FrameFullAera(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawCircle(u16 x0,u16 y0,u8 r,u16 Color);
void FrameShowChar(u16 x,u16 y,u8 num,u8 size,u16 Color);
void FrameShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Color);
	*/
	 FrameClear(YELLOW);
	 FrameFullAera(50,50,100,100,BLUE);
	 FrameDrawRectangle(25,25, 125, 125,RED);
	 FrameDrawCircle(100,100,50,BLACK);
	 FrameShowString(0,200,240,16,16,(u8 *)"this is Frame Test!!!",BLACK);
	
}
int main()
{
	
	int times=0;
	FIL *fp;
	Init();
	delay_ms(200);
	printf("I am working now!\n\r");
	if(!PS2_RedLight()){
		printf("Gamehandle is aredly in RedLight\n\r");
	}else{
		printf("not in game mode\n\r");
	}
	MicroSDTest();
	W25Q128Test();
	FrameTest();
	/*
	FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);			
	FRESULT f_close (FIL* fp);											
	FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);		
	FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	
	*/
	exfuns_init();
	
	f_open (fp, "/aaa.txt", 1);	
	f_write (fp, "this is test",0, 0);
	f_close (fp);
	while(1)
	{
			times++;
		  drawParten();
			delay_ms(20);
		
			if(times<100)
			{
				LEDOn();
			}else if(times<200){
				LEDOff();
			}else
				times=0;
		
	}
	
}

void drawParten(void)
{
	char key;
	PS2_ClearData();  //清除缓存
	PS2_ReadData();	   //读数据
	key=PS2_DataKey();
	if(key!=0)
	{
		switch(key){
			case PSB_CIRCLE:FrameClear(YELLOW);break;
			case PSB_PAD_UP:y-=5;break;
			case PSB_PAD_DOWN:y+=5;break;
			case PSB_PAD_LEFT:x-=5;break;
			case PSB_PAD_RIGHT:x+=5;break;
		}
		if(x==0)x=240;
		if(x>=240)x=0;
		if(y==0)y=320;
		if(y>320)y=0;
		FrameFullAera(x,y,x+10,y+10,0x0f0f);
	}
}



u8 exfuns_init(void)
{
	u8 i;
	for(i=0;i<_VOLUMES;i++)
	{
		fs[i]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//为磁盘i工作区申请内存	
		if(!fs[i])break;
	}
	file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为file申请内存
	ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为ftemp申请内存
	fatbuf=(u8*)mymalloc(SRAMIN,512);				//为fatbuf申请内存
	if(i==_VOLUMES&&file&&ftemp&&fatbuf)return 0;  //申请有一个失败,即失败.
	else return 1;	
}

