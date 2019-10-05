#include "Interface.h"



//定义内存卡信息这里在初始化的时候就进行初始化
__MicroSDInfo MicroSDInfo={0};

/*
PB0 CS
PA5 SCK
PA6 MISO
PA7 MOSI
SPI模式初始化完成以后就不需要crc校验了
*/
#define MicroSD_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define MicroSD_CS_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_0)


char MicroSDDataBuff[512]={0};//一个扇区的大小


void MicroSDGPIOInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB0
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}


/*********************************一下是基础方法的改写*********************************************/
void MicroSDDisSelect(void)
{
	MicroSD_CS_SET;
  SPI1_ReadWriteByte(0xff);//提供额外的8个时钟
}
//等待卡准备好
//返回值:0,准备好了;其他,错误代码
u8 MicroSDWaitReady(void)
{
	u32 t=0;
	do
	{
		if(SPI1_ReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0xFFFF);//等待 
	return 1;
}
//返回值:0,成功;1,失败;
u8 MicroSDSelect(void)
{
	MicroSD_CS_RESET;
	if(MicroSDWaitReady()==0)return 0;//等待成功
	MicroSDDisSelect();
	return 1;//等待失败
}
//等待SD卡回应
//Response:要得到的回应值
//返回值:0,成功得到了该回应值
u8 MicroSDGetResponse(u8 Response)
{
	u16 Count=0xFFFF;//等待次数	   						  
	while ((SPI1_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}
//从sd卡读取一个数据包的内容
//buf:数据缓存区
//len:要读取的数据长度.
//返回值:0,成功;其他,失败;	
u8 MicroSDRecvData(u8*buf,u16 len)
{			  	  
	if(MicroSDGetResponse(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SPI1_ReadWriteByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SPI1_ReadWriteByte(0xFF);
    SPI1_ReadWriteByte(0xFF);									  					    
    return 0;//读取成功
}
//向sd卡写入一个数据包的内容 512字节
//buf:数据缓存区
//cmd:指令
//返回值:0,成功;其他,失败;	
u8 MicroSDSendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	if(MicroSDWaitReady())return 1;//等待准备失效
	
	SPI1_ReadWriteByte(cmd);
	if(cmd!=0XFD)//不是结束指令
	{
		for(t=0;t<512;t++)SPI1_ReadWriteByte(buf[t]);//提高速度,减少函数传参时间
	    SPI1_ReadWriteByte(0xFF);//忽略crc
	    SPI1_ReadWriteByte(0xFF);
			t=SPI1_ReadWriteByte(0xFF);//接收响应
			if((t&0x1F)!=0x05)
				return 2;//响应错误									  					    
	}						 									  					    
    return 0;//写入成功
}

//向SD卡发送一个命令
//输入: u8 cmd   命令 
//      u32 arg  命令参数
//      u8 crc   crc校验值	   
//返回值:SD卡返回的响应															  
u8 MicroSDSendCmd(u8 cmd, u32 data, u8 crc)
{
  int result=0,times=0;
	
	MicroSD_CS_SET;//禁止SD卡片选 同步时钟
	SPI1_ReadWriteByte(0xff);   
	MicroSD_CS_RESET;//开始传输
	SPI1_ReadWriteByte(cmd|0x40);
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
	return (char)result;
}






//初始化SD卡
char MicroSDInit(void)
{
  u8 r1;      // 存放SD卡的返回值
  u16 retry;  // 用来进行超时计数
  u8 buf[4];  
	u16 i;
	u32 SDSize=0;
	MicroSDGPIOInit();
	MicroSD_CS_SET;
 	for(i=0;i<100;i++)SPI1_ReadWriteByte(0XFF);//发送最少74个脉冲
	retry=20;
	do
	{
		r1=MicroSDSendCmd(CMD0,0,0x95);//进入IDLE状态
	}while((r1!=0X01) && retry--);
	
	if(r1==0X01)
	{
		if(MicroSDSendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SPI1_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					MicroSDSendCmd(CMD55,0,0X01);	//发送CMD55
					r1=MicroSDSendCmd(CMD41,0x40000000,0X01);//发送CMD41
				}while(r1&&retry--);
				if(retry&&MicroSDSendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++)buf[i]=SPI1_ReadWriteByte(0XFF);//得到OCR值
					if(buf[0]&0x40)MicroSDInfo.MicroSDType=SD_TYPE_V2HC;    //检查CCS
					else MicroSDInfo.MicroSDType=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			MicroSDSendCmd(CMD55,0,0X01);		//发送CMD55
			r1=MicroSDSendCmd(CMD41,0,0X01);	//发送CMD41
			if(r1<=1)
			{		
				MicroSDInfo.MicroSDType=SD_TYPE_V1;
				retry=0XFFFE;
				do //等待退出IDLE模式
				{
					MicroSDSendCmd(CMD55,0,0X01);	//发送CMD55
					r1=MicroSDSendCmd(CMD41,0,0X01);//发送CMD41
				}while(r1&&retry--);
			}else//MMC卡不支持CMD55+CMD41识别
			{
				MicroSDInfo.MicroSDType=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //等待退出IDLE模式
				{											    
					r1=MicroSDSendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||MicroSDSendCmd(CMD16,512,0X01)!=0)MicroSDInfo.MicroSDType=SD_TYPE_ERR;//错误的卡
		}
	}
	MicroSDDisSelect();//取消片选
  /*****************************下面需要将内存卡的信息读取出来*******************************/
	
	SDSize=MicroSDGetSectorCount();
	printf("MicroSD init completed the Size is:%d       ",SDSize);
	printf("MicroSD type is:%d       ",MicroSDInfo.MicroSDType);
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);
	if(MicroSDInfo.MicroSDType)return 0;
	else if(r1)return r1; 	   
	return 0xaa;//其他错误
}







//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
u8 MicroSDReadDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(MicroSDInfo.MicroSDType!=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
	if(cnt==1)
	{
		r1=MicroSDSendCmd(CMD17,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=MicroSDRecvData(buf,512);//接收512个字节	   
		}
	}else
	{
		r1=MicroSDSendCmd(CMD18,sector,0X01);//连续读命令
		do
		{
			r1=MicroSDRecvData(buf,512);//接收512个字节	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		MicroSDSendCmd(CMD12,0,0X01);	//发送停止命令
	}   
	MicroSDDisSelect();//取消片选
	return r1;//
}
//写SD卡
//buf:数据缓存区
//sector:起始扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
u8 MicroSDWriteDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(MicroSDInfo.MicroSDType!=SD_TYPE_V2HC)sector *= 512;//转换为字节地址
	if(cnt==1)
	{
		r1=MicroSDSendCmd(CMD24,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=MicroSDSendBlock(buf,0xFE);//写512个字节	   
		}
	}else
	{
		if(MicroSDInfo.MicroSDType!=SD_TYPE_MMC)
		{
			MicroSDSendCmd(CMD55,0,0X01);	
			MicroSDSendCmd(CMD23,cnt,0X01);//发送指令	
		}
 		r1=MicroSDSendCmd(CMD25,sector,0X01);//连续读命令
		if(r1==0)
		{
			do
			{
				r1=MicroSDSendBlock(buf,0xFC);//接收512个字节	 
				buf+=512;  
			}while(--cnt && r1==0);
			r1=MicroSDSendBlock(0,0xFD);//接收512个字节 
		}
	}   
	MicroSDDisSelect();//取消片选
	return r1;//
}	






/***************************************************获取信息的方法的*********************************************************/

//获取SD卡的CID信息，包括制造商信息
//输入: u8 *cid_data(存放CID的内存，至少16Byte）	  
//返回值:0：NO_ERR
//		 1：错误														   
u8 MicroSDGetCID(u8 *cid_data)
{
    u8 r1;	   
    //发CMD10命令，读CID
    r1=MicroSDSendCmd(CMD10,0,0x01);
    if(r1==0x00)
		{
			r1=MicroSDRecvData(cid_data,16);//接收16个字节的数据	 
    }
	MicroSDDisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}																				  
//获取SD卡的CSD信息，包括容量和速度信息
//输入:u8 *cid_data(存放CID的内存，至少16Byte）	    
//返回值:0：NO_ERR
//		 1：错误														   
u8 MicroSDGetCSD(u8 *csd_data)
{
    u8 r1;	 
    r1=MicroSDSendCmd(CMD9,0,0x01);//发CMD9命令，读CSD
    if(r1==0)
	{
    	r1=MicroSDRecvData(csd_data, 16);//接收16个字节的数据 
    }
	MicroSDDisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}  
//获取SD卡的总扇区数（扇区数）   
//返回值:0： 取容量出错 
//       其他:SD卡的容量(扇区数/512字节)
//每扇区的字节数必为512，因为如果不是512，则初始化不能通过.														  
u32 MicroSDGetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
		u16 csize;  					    
	//取CSD信息，如果期间出错，返回0
    if(MicroSDGetCSD(csd)!=0) return 0;	    
    //如果为SDHC卡，按照下面方式计算
    if((csd[0]&0xC0)==0x40)	 //V2.00的卡
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;//得到扇区数	 		   
    }else//V1.XX的卡
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9);//得到扇区数   
    }
    return Capacity;
}














