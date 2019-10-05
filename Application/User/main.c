#include "Interface.h"
#include "FrameBox.h"
#include "StringOperation.h"
#include "Init.h"


/***************************************************************************************
由于这里的硬件的实验单项都测试过了所以这里将进行所有的实验以及文字库的集成fat的集成这里也将会是
一个漫长的过程
硬件的列表如下：
LED
LCD
time
uart
key
W25Q128
MicroSD
VS1053B

集成Fat功能
需要
对应接口

1.flash以及MicroSD 			接口测试正常
2.修改ffconf的配置文件 	配置跟随正点原子
3.测试是否有问题 				读写检查完毕

以上功能基本实现
2019 9 30

实现音乐的播放

1.文件的读取
2.数据传输
3.界面设置
4.播放功能的实现



主界面描述

KEY_Up			0x00	音量加
KEY_Down		0x01	音量减
KEY_Left		0x02	上一首 长按快进
KEY_Right		0x03	下一首 长按快退
KEY_Ok			0x04	设置	最后实现
KEY_Back		0x05	音效	最后实现

****************************************************************************************/
u16 color=0x000;
extern char MicroSDDataBuff[512];

#define LED		PCout(13) 




/***************************************************************************************

mp3读取步骤
硬件初始化
fat初始化

打开文件
f_opendir(&mp3dir,"0:/MUSIC")
 得到总有效文件数
totmp3num=mp3_get_tnum("0:/MUSIC");
//为长文件缓存区分配内存
mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	

vs1053b复位
VS_HD_Reset();
VS_Soft_Reset();

获取歌曲信息然后播放
mp3_index_show(curindex+1,totmp3num);
key=mp3_play_song(pname); 

发送数据
VS_Send_MusicData(databuf+i)==0	


***************************************************************************************/
char MusicNameBuffer[4][50]={
"Wannabe.mp3",
"500Miles.mp3",
"Spongebob.mp3",
"Sailing.mp3"
};

void MusicPlay()
{
	//int musicIndex=0;//用于表示播放的
	//char isPaly=0x00;
	FRESULT res;
	FIL file1;     //文件1
	u8 buffer[4096];
	u32 counter=0;
	
	res=f_open(&file1, "0:/Spongebob.mp3", FA_OPEN_EXISTING|FA_READ);
	printf("Opening music 0:/music/Spongebob.mp3  \n\n");//Spongebob.mp3
	delay_ms(10);
	VS_HD_Reset();
	VS_Soft_Reset();
		
	VS_Set_Vol(220);
	VS_Restart_Play();  					//重启播放 
	VS_Set_All();        					//设置音量等信息 			 
	VS_Reset_DecodeTime();					//复位解码时间 
	VS_SPI_SpeedHigh();	//高速
	
	while(1)
	{
		if(res==0x00){
			
			counter=0;
			res=f_read(&file1,buffer,4096,&br);
			do{ //主播放循环
			   if(VS_Send_MusicData(buffer+counter)==0)//给VS10XX发送音频数据
				 {
					 counter+=32;
					 //LED=~LED;
				 }
			}while(counter<4096);//循环发送4096个字节 
			if(br!=4096||res!=0){
					f_close(&file1);
					printf("Music played over!!!\n\n");
					break;//读完了.		  
			}
		}else{
			printf("Open music failed\n\n");
			break;
		}
			
		
	}
	
}


/*******变量定义*****/
FRESULT res;
void FatTest(){
	FIL file1;     //文件1
	char buffer[512];
	res=f_open(&file1, "0:/message.txt", FA_OPEN_EXISTING|FA_READ);
	res=f_read (&file1,buffer,512,&br);
	f_close(&file1);
  printf("buffer:%s",buffer);
	
}


/**************************
留下的问题
1.文件读取的问题 一旦打开文件夹 或者有两种不同的格式在一个文件夹就会打不开的情况

2.文件名的获取
3.文件名显示问题
4.歌曲切换的问题

**************************/
int main(void) 
{  
  u32 total,free;
  //char arr[100]="this is a test about frameveiw and you will see this display about in this windows!";
	FRESULT res;
	FATFS fs0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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
	
 	//f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	//f_mount(fs[1],"1:",1); 		//挂载FLASH.
	res= f_mount(&fs0,"0:",1);    /* Mount a logical drive */;
	
	//f_mkfs(0,1,4096);
  res=exf_getfree((u8 *)"0:",&total,&free) ;//得到SD卡的总容量和剩余容量
	printf(" Error Code:%d   \n",res);
  printf(" 总容量：%d MB,剩余容量：%d MB     \n\n",total>>10,free>>10);
	//FrameStart();
	//FatTest();
	MusicPlay();
	//mp3_play();
	//卸载内存卡
  res= f_mount(NULL,(TCHAR*)1,1);
  while(1);
}



