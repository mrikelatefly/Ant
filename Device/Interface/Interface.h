#ifndef __INTERFACE_H
#define __INTERFACE_H
#include"Config.h"
#include"Typedefine.h"


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

#endif



