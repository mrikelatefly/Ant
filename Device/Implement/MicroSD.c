#include "Interface.h"


#define MicroSD_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define MicroSD_CS_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_0)

char MicroSDDataBuff[512]={0};//一个扇区的大小
typedef struct
{
	char CID[16];
	char RCA[2];
	char DSR[2];
	char CSD[16];
	char SCR[8];
	char OCR[4];
	char SSR[64];//(SD Status)
	char CSR[4];//Card Status)
	
}__MicroSDInfo;

__MicroSDInfo  MicroSDInfo={0};



char SentCommandToMicroSD(char cmd,int data,char crc)
{
	char result=0,times=0;
	
	MicroSD_CS_SET;//禁止SD卡片选 同步时钟
	SPI1_ReadWriteByte(0xff);   
	MicroSD_CS_RESET;//开始传输
	SPI1_ReadWriteByte(cmd);
	for(times=0;times<4;times++)    
	{    
      SPI1_ReadWriteByte((data>>24)&0xff);
			data<<=8;
  }

  SPI1_ReadWriteByte(crc);
	SPI1_ReadWriteByte(0xff); //八个时钟 
	times=0;
	do    
	{  //读取后8位   
		result = SPI1_ReadWriteByte(0xFF);   
		times++;   
	}
	while((result==0xff)&&(times<200));
	return result;
}

//初始化化不是很稳定也就是说明传输数据可能不是很稳定
char SD_Init(void)
{
	char i,temp=0;
	//char CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
	int retry=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB0
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	//初始化SPI1
	SPI1_Init();
	
	MicroSD_CS_SET;
	//发送至少74个时钟信号
	for (i=0;i<0x2f;i++){SPI1_ReadWriteByte(0xff);} 
	//为了能够成功写入CMD0,在这里写200次
	do  
	{   
		temp=SentCommandToMicroSD(0x40,0,0x95);   
		retry++;   
		if(retry>800)
		{ //超过200次   
		//CMD0 Error!	return(INIT_CMD0_ERROR); 
			//printf("Init MicroSD CMD0 Error!!!Back:%d\n\n",temp);
			return 0;
		}   
	}    
	while(temp!=0x01); //回应01h，停止写入
	//printf("Reset MicroSD successfully!!!times:%d\n\n",retry);
	//发送CMD1到SD卡 
	retry=0;   
	do  
	{
		//为了能成功写入CMD1,写100次   
		temp=SentCommandToMicroSD(0x41,0,0xff);
		retry++;   
		if(retry>800)    
		{ 
			//超过100次
			//printf("Init 1MicroSD CMD1 Error!!!Back:%d\n\n",temp);
			return 0;
		}
	}
	while(temp!=0x00);//回应00h停止写入    
	MicroSD_CS_SET;  //片选无效
	//printf("Init MicroSD sent CMD1 successfully!!!times:%d\n\n",retry);
	
	//更换更快的SPI速率
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);
	
	return 0;
}






char WriteSectorToMicroSD(long addr,char *buff) 
{     
   int tmp,retry;   
   unsigned int i;   
   addr = addr << 9; //addr = addr * 512 
   //写命令24到SD卡中去   
   retry=0;   
   do  
   {  //为了可靠写入，写100次   
      tmp=SentCommandToMicroSD(0x58,addr,0xff);   
      retry++;   
      if(retry>800)    
      {    
				//printf("Write CMD58 Error!!!\n\r");
        return 1; //send commamd Error!   
      }   
   }   
   while(tmp!=0);
	 
   //在写之前先产生100个时钟信号   
   for (i=0;i<100;i++)   
   {   
      SPI1_ReadWriteByte(0xff);   
   }   
   //写入开始字节   
   SPI1_ReadWriteByte(0xfe);    
    
   //现在可以写入512个字节   
   for (i=0;i<512;i++)   
   {   
      SPI1_ReadWriteByte(*buff);
			buff++;
   }
   //CRC-Byte    
   SPI1_ReadWriteByte(0xFF); //Dummy CRC   
   SPI1_ReadWriteByte(0xFF); //CRC Code
	 
   tmp=SPI1_ReadWriteByte(0xff);   // read response   
   if((tmp & 0x1F)!=0x05) // 写入的512个字节是未被接受   
   {   
     MicroSD_CS_SET;
		 //printf("Write data didn't accept by MicroSD\n\r");
     return 1; //Error!   
   }   
   //等到SD卡不忙为止   
	 //因为数据被接受后，SD卡在向储存阵列中编程数据   
   while (SPI1_ReadWriteByte(0xff)!=0xff);
	 
   //禁止SD卡 写入成功
   MicroSD_CS_SET;
   return 0;   
}

char ReadSectorFromMicroSD(long sector,char *buffer)   
{     
   int retry;   
   //命令16  
	int times=0;
  //unsigned char CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF};    
  unsigned char temp;   
   
   //地址变换   由逻辑块地址转为字节地址   
   sector = sector << 9; //sector = sector * 512
   //将命令16写入SD卡   
   retry=0;   
   do  
   {  //为了保证写入命令  一共写100次   
      temp=SentCommandToMicroSD(0x51,sector,0xff);   
      retry++;   
      if(retry>800)    
      {   
				//printf("Read sector from MicroSD is failed!!\n\r");
        return 1; //block write Error!   
      }   
   }   
   while(temp!=0);  
   //Read Start Byte form MMC/SD-Card (FEh/Start Byte)   
   //Now data is ready,you can read it out.   
  while (SPI1_ReadWriteByte(0xff) != 0xfe);
	for(times=0;times<512;times++)
	{
		MicroSDDataBuff[times]=SPI1_ReadWriteByte(0xff);
	}
	//禁止SD卡 读出完成
   MicroSD_CS_SET;
	return 0;
} 

char WriteSectorsToMicroSD(long addr,char *buff,int count)
{
	WriteSectorToMicroSD(addr,buff);
	return 0;
}
char ReadSectorsFromMicroSD(long addr,char *buff,int count)
{
	ReadSectorFromMicroSD(addr,buff);
	return 0;
}

