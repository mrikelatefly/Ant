#ifndef __INTERFACE_H
#define __INTERFACE_H
#include"Config.h"
#include"Typedefine.h"
/****************************************************KEY***********************************************/

void KeyInit(void);
char GetPressedKey(void);


/****************************************************Usart**********************************************/
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void UsartInit(u32 bound);



/****************************************************LED**********************************************/
void LEDInit(void);
void LEDOn(void);	
void LEDOff(void);


/****************************************************Delay**********************************************/

void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);

/*****************************************************PS2**********************************************/
extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);//判断是否为红灯模式
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);		  //
u8 PS2_DataKey(void);		  //键值读取
u8 PS2_AnologData(u8 button); //得到一个摇杆的模拟量
void PS2_ClearData(void);	  //清除数据缓冲区
/*******************************************LCD****************************************************/

void LCDInit(void);
//void LCD_Clear(u16 Color);
void LCD_DrawPoint(u16 x,u16 y,u16 Color);
/******************************************MicroSD**************************************************/

char SD_Init(void);
char WriteSectorToMicroSD(long addr,char *buff);
char ReadSectorFromMicroSD(long sector,char *buffer);

char WriteSectorsToMicroSD(long addr,char *buff,int count);
char ReadSectorsFromMicroSD(long addr,char *buff,int count);


/*******************************************SPI******************************************************/

void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);

void SPI2_Init(void);
u8 SPI2_ReadWriteByte(u8 TxData);
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);


/*******************************************W25Q128  Flash***********************************************/



extern u16 W25QXX_TYPE;					//定义W25QXX芯片型号		   

#define	W25QXX_CS_SET 		GPIO_SetBits(GPIOB,GPIO_Pin_12); 		//W25QXX的片选信号
#define	W25QXX_CS_RESET 	GPIO_ResetBits(GPIOB,GPIO_Pin_12);  		//W25QXX的片选信号				 

 


void W25QXX_Init(void);
u16  W25QXX_ReadID(void);  	    		//读取FLASH ID
u8	 W25QXX_ReadSR(void);        		//读取状态寄存器 
void W25QXX_Write_SR(u8 sr);  			//写状态寄存器
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);		//写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25QXX_Erase_Chip(void);    	  	//整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25QXX_Wait_Busy(void);           	//等待空闲
void W25QXX_PowerDown(void);        	//进入掉电模式
void W25QXX_WAKEUP(void);				//唤醒









/*******************************************VS1053B***********************************************/

#define VS_RST      PBout(5) 	//RST
#define VS_XCS      PBout(6)  	//XCS
#define VS_DQ       PBin(7)  	//DREQ 
#define VS_XDCS     PBout(8)  	//XDCS 


__packed typedef struct 
{							  
	u8 mvol;		//主音量,范围:0~254
	u8 bflimit;		//低音频率限定,范围:2~15(单位:10Hz)
	u8 bass;		//低音,范围:0~15.0表示关闭.(单位:1dB)
	u8 tflimit;		//高音频率限定,范围:1~15(单位:Khz)
	u8 treble;		//高音,范围:0~15(单位:1.5dB)(原本范围是:-8~7,通过函数修改了);
	u8 effect;		//空间效果设置.0,关闭;1,最小;2,中等;3,最大.
	u8 speakersw;	//板载喇叭开关,0,关闭;1,打开 
	u8 saveflag; 	//保存标志,0X0A,保存过了;其他,还从未保存	   
}_vs10xx_obj;

extern _vs10xx_obj vsset;		//VS10XX设置



u16  VS_RD_Reg(u8 address);				//读寄存器
u16  VS_WRAM_Read(u16 addr);	    	//读RAM
void VS_WRAM_Write(u16 addr,u16 val);	//写RAM
void VS_WR_Data(u8 data);				//写数据
void VS_WR_Cmd(u8 address,u16 data);	//写命令
u8   VS_HD_Reset(void);			    	//硬复位
void VS_Soft_Reset(void);           	//软复位
u16 VS_Ram_Test(void);             		//RAM测试	    
void VS_Sine_Test(void);            	//正弦测试
													 
u8 	 VS_SPI_ReadWriteByte(u8 data);
void VS_SPI_SpeedLow(void);
void VS_SPI_SpeedHigh(void);
void VS_Init(void);						//初始化VS10XX	 
void VS_Set_Speed(u8 t);				//设置播放速度
u16  VS_Get_HeadInfo(void);     		//得到比特率
u32 VS_Get_ByteRate(void);				//得到字节速率
u16 VS_Get_EndFillByte(void);			//得到填充字节
u8 	 VS_Send_MusicData(u8* buf);		//向VS10XX发送32字节
void VS_Restart_Play(void);				//重新开始下一首歌播放	  
void VS_Reset_DecodeTime(void);			//重设解码时间
u16  VS_Get_DecodeTime(void);   		//得到解码时间

void VS_Load_Patch(u16 *patch,u16 len);	//加载用户patch
u8 	 VS_Get_Spec(u16 *p);       		//得到分析数据	   
void VS_Set_Bands(u16 *buf,u8 bands);	//设置中心频率
void VS_Set_Vol(u8 volx);				//设置主音量   
void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble);//设置高低音
void VS_Set_Effect(u8 eft);				//设置音效
void VS_SPK_Set(u8 sw);					//板载喇叭输出开关控制
void VS_Set_All(void);

void vs10xx_read_para(_vs10xx_obj * vs10xxdev);
void vs10xx_save_para(_vs10xx_obj * vs10xxdev);

#endif



