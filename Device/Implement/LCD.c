#include "Interface.h"

/*
SPI2
只会用到三根线 
MOSI PB15
MISO PB14
SCK PB13 

CS PB7
RESET PB8
D/C PB9

这里准备重新写入SPI
防止频繁的更改这里提供原始的设置信息
#define LCD_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LCD_CS_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define LCD_RESET_SET GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define LCD_RESET_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define LCD_DC_SET GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LCD_DC_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_9)

void LCD_Pin_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_8);  //上拉所有
}
*/

#define LCD_CS_SET GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LCD_CS_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LCD_RESET_SET GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define LCD_RESET_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define LCD_DC_SET GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define LCD_DC_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_11)


void LCD_Pin_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12);  //上拉所有
}

//这里就是要改写的方法
void SentBit(char data)
{
	SPI_WriteByte(SPI2,data);
}
void LCD_WR_CMD(char cmd)
{
	LCD_CS_RESET;//cs=0
	LCD_DC_RESET;//dc=0 cmd
	SentBit(cmd);
	LCD_CS_SET;//cs=1
	
}
void LCD_WR_DATA8(char data)
{
	LCD_CS_RESET;//cs=0
	LCD_DC_SET;//dc=1 date
	SentBit(data);
	LCD_CS_SET;//cs=1
}
void LCD_WR_DATA(u16 data)
{
	LCD_CS_RESET;//cs=0
	LCD_DC_SET;//dc=0 cmd
	SentBit(data>>8);
	SentBit(0x00ff&data);
	LCD_CS_SET;//cs=1
}

void LCD_WR_CMD_DATA(char LCD_CMD, u16 LCD_DATA)
{
	LCD_WR_CMD(LCD_CMD);
	LCD_WR_DATA(LCD_DATA);
}

void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_CMD(0x2c);  
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_CMD(0x2a); 
	LCD_WR_DATA8(Xpos>>8); 
	LCD_WR_DATA8(Xpos&0XFF);	 
	LCD_WR_CMD(0x2b); 
	LCD_WR_DATA8(Ypos>>8); 
	LCD_WR_DATA8(Ypos&0XFF);
	LCD_WriteRAM_Prepare();	
}


void LCD_DrawPoint(u16 x,u16 y,u16 Color)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(Color);//此处默认为深蓝色
	//LCD_WR_CMD(0x2c);
} 
void LCDInit(void)
{
	LCD_Pin_Init();
	delay_us(100);
	LCD_RESET_RESET;		//reset=0 
 	delay_ms(50); // delay 20 ms 
	LCD_RESET_SET;		 
 	delay_ms(20); // delay 20 ms 
/*
	SPILCD_RST_RESET ;	//LCD_RST=0	 //SPI接口复位
	delay_ms(20); // delay 20 ms 
	SPILCD_RST_SET ;	//LCD_RST=1		
	delay_ms(20);	
	
		lcddev.width=240;
		lcddev.height=320;
		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B; 	
*/

		
				LCD_WR_CMD(0xCB);  
        LCD_WR_DATA8(0x39); 
        LCD_WR_DATA8(0x2C); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x34); 
        LCD_WR_DATA8(0x02); 

        LCD_WR_CMD(0xCF);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0XC1); 
        LCD_WR_DATA8(0X30); 
 
        LCD_WR_CMD(0xE8);  
        LCD_WR_DATA8(0x85); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x78); 
 
        LCD_WR_CMD(0xEA);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_CMD(0xED);  
        LCD_WR_DATA8(0x64); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0X12); 
        LCD_WR_DATA8(0X81); 

        LCD_WR_CMD(0xF7);  
        LCD_WR_DATA8(0x20); 
  
        LCD_WR_CMD(0xC0);    //Power control 
        LCD_WR_DATA8(0x23);   //VRH[5:0] 
 
        LCD_WR_CMD(0xC1);    //Power control 
        LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WR_CMD(0xC5);    //VCM control 
        LCD_WR_DATA8(0x3e); //对比度调节
        LCD_WR_DATA8(0x28); 
 
        LCD_WR_CMD(0xC7);    //VCM control2 
        LCD_WR_DATA8(0x86);  //--
 
        LCD_WR_CMD(0x36);    // Memory Access Control 
        LCD_WR_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_WR_CMD(0x3A);    
        LCD_WR_DATA8(0x55); 

        LCD_WR_CMD(0xB1);    
        LCD_WR_DATA8(0x00);  
        LCD_WR_DATA8(0x18); 
 
        LCD_WR_CMD(0xB6);    // Display Function Control 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x82);
        LCD_WR_DATA8(0x27);  
 
        LCD_WR_CMD(0xF2);    // 3Gamma Function Disable 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_CMD(0x26);    //Gamma curve selected 
        LCD_WR_DATA8(0x01); 
 
        LCD_WR_CMD(0xE0);    //Set Gamma 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x2B); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x4E); 
        LCD_WR_DATA8(0xF1); 
        LCD_WR_DATA8(0x37); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x10); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x09); 
        LCD_WR_DATA8(0x00); 

        LCD_WR_CMD(0XE1);    //Set Gamma 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x14); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x11); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0xC1); 
        LCD_WR_DATA8(0x48); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x36); 
        LCD_WR_DATA8(0x0F); 
 
        LCD_WR_CMD(0x11);    //Exit Sleep 
        delay_ms(20);
				
        LCD_WR_CMD(0x29);    //Display on 
        LCD_WR_CMD(0x2c); 
				//LCD_Clear(WHITE);
}

/*

void LCD_DisplayOn(void)
{
	LCD_WR_CMD(0x11);    //Exit Sleep 
}
void LCD_DisplayOff(void)
{
	LCD_WR_CMD(0x29);    //Display on 
}
*/






