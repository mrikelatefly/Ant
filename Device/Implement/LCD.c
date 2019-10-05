#include "Interface.h"


//可自由更改接口IO配置，使用任意最少4 IO即可完成本款液晶驱动显示
/***************************************************************************************
#define LCD_CS_SET		GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LCD_CS_RESET	GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LCD_RST_SET		GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LCD_RST_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define LCD_DC_SET		GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LCD_DC_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

CS	PA8
DC	PC0
RST	PC1

**********************************************************************************************/
/**************************************替代管脚操作****************************************/

#define LCD_CS_SET		GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LCD_CS_RESET	GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LCD_DC_SET		GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LCD_DC_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define LCD_RST_SET		GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LCD_RST_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_1)




//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


/****************************************************************************
* 名    称：void LCD_GPIO_Init(void)
* 功    能：STM32_模拟SPI所用到的GPIO初始化
* 入口参数：无
* 出口参数：无
* 说    明：初始化模拟SPI所用的GPIO
****************************************************************************/
void LCD_GPIOInit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	/*CS	PA8
		DC	PC0
		RST	PC1
	*/
	//配置SPI2管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
}


/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void LCD_WR_REG(u8 Index)
{
   LCD_CS_RESET;
   LCD_DC_RESET;
	 delay_us(10);
   SPI_WriteByte(SPI2,Index);
	 delay_us(10);
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
   LCD_CS_RESET;
   LCD_DC_SET;
	 delay_us(10);
   SPI_WriteByte(SPI2,Data);
	 delay_us(10); 
	 LCD_CS_SET;
}


//******************************************************************
//函数名：  LCD_WR_DATA_16Bit
//功能：    8位总线下如何写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA(u16 data)
{	
   LCD_CS_RESET;
   LCD_DC_SET;
   SPI_WriteByte(SPI2,data>>8);
   SPI_WriteByte(SPI2,0xff&data);
   LCD_CS_SET;
}

//******************************************************************
//函数名：  LCD_WriteReg
//作者：    ronview
//日期：    2015-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);  		 
}	


//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：    ronview
//日期：    2015-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
	//LCD_WR_REG(0x2c);
}	 

//******************************************************************
//函数名：  LCD_DrawPoint
//作者：    ronview
//日期：    2015-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 
}

//******************************************************************
//函数名：  LCD_Clear
//作者：    ronview
//日期：    2015-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);  
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);    
	}
} 

//******************************************************************
//函数名：  LCD_Init
//作者：    ronview
//日期：    2015-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{      										 

	LCD_GPIOInit();
	LCD_RST_SET;
	delay_us(100);
	LCD_RST_RESET;
	delay_ms(200);	
	LCD_RST_SET;
	delay_ms(200);
	//LCD Init For 2.0inch LCD Panel with ILI9225G.	
//************* Start Initial Sequence **********//		
//ILI9225G
	LCD_WR_REG(0x0028);
	LCD_WR_DATA(0x00CE);//SOFTWVAE RESET
	delay_ms(100);
	LCD_WR_REG(0x0028);
	LCD_WR_DATA(0x01CE);
    
  LCD_WR_REG(0x0000);
	LCD_WR_DATA(0x0001);
	LCD_WR_REG(0x0001);  //Driver output control
	LCD_WR_DATA(0x011C);	
	LCD_WR_REG(0x0002);  //LCD Driving waveform control
	LCD_WR_DATA(0x0100);	
	LCD_WR_REG(0x0003);  //Entry Mode
	LCD_WR_DATA(0x1030);		
		
  LCD_WR_REG(0x0008);	//set BP and FP
	LCD_WR_DATA(0x0303);		

	LCD_WR_REG(0x000C);	//18bit RGB
	LCD_WR_DATA(0x0000);		
	LCD_WR_REG(0x000F);	//osc control
	LCD_WR_DATA(0x0601);		
	LCD_WR_REG(0x0020);
	LCD_WR_DATA(0x0000); 
	LCD_WR_REG(0x0021);
	LCD_WR_DATA(0x0000); 
	delay_ms(100);
//=========POWER CONTROL================	
	LCD_WR_REG(0x0010);	//SET SAP,DSTB,STB
	LCD_WR_DATA(0x0A00);			
	LCD_WR_REG(0x0011);	//SET APON,PON,AON,VI1EN,VC
	LCD_WR_DATA(0x1037);		
	LCD_WR_REG(0x0012);  //DDVDH,VCL,VGH,VGL
	LCD_WR_DATA(0x5121);		
	LCD_WR_REG(0x0013);	//SOURCE VOLTAGE
	LCD_WR_DATA(0x0066);
  delay_ms(100);	
	LCD_WR_REG(0x0014); //Vcom , VCOMLamplitude
	LCD_WR_DATA(0x5a62);	   //Vcomh

	delay_ms(100);
	
	LCD_WR_REG(0x0030);//GATE SCAN CONTROL
	LCD_WR_DATA(0x0000); //
	
	LCD_WR_REG(0x0031);// Vertical scrol control(end)
	LCD_WR_DATA(0x00DB);//
	
  LCD_WR_REG(0x0032);//Vertical scrol control(stard)
	LCD_WR_DATA(0x0000);//
	
	LCD_WR_REG(0x0033);  //
	LCD_WR_DATA(0x0000);  //
	
	LCD_WR_REG(0x0036); //
	LCD_WR_DATA(0x00AF); //
	
  LCD_WR_REG(0x0037);//
	LCD_WR_DATA(0x0000);//	
	
  LCD_WR_REG(0x0038);//
	LCD_WR_DATA(0x00DB);//
	
	LCD_WR_REG(0x0039);//
	LCD_WR_DATA(0x0000);//
		
  //==========gamma
	LCD_WR_REG(0x0050);//
	LCD_WR_DATA(0x0000);//pv8;v1
	LCD_WR_REG(0x0051);//
	LCD_WR_DATA(0x050C);//pv43,v20
	LCD_WR_REG(0x0052);//
	LCD_WR_DATA(0x0C09);//pv62,v55
	LCD_WR_REG(0x0053);//
	LCD_WR_DATA(0x0204);//
	LCD_WR_REG(0x0054);//
	LCD_WR_DATA(0x090C);//nv55,v62
	LCD_WR_REG(0x0055);//
	LCD_WR_DATA(0x0C05);//nv20,v43
  LCD_WR_REG(0x0056);//
	LCD_WR_DATA(0x0404);//nv1,v8
	LCD_WR_REG(0x0057);//
	LCD_WR_DATA(0x0402);//
	LCD_WR_REG(0x0058);//
	LCD_WR_DATA(0x0A00);//
	LCD_WR_REG(0x0059);//
	LCD_WR_DATA(0x000A);//
	//=====display on
	LCD_WR_REG(0x0007);
	LCD_WR_DATA(0x0012);	
	delay_ms(100);
	LCD_WR_REG(0x0007);
	LCD_WR_DATA(0x1017);  
	delay_ms(200);
	//LCD_LED_SET;//点亮背光	 
	LCD_SetParam();//在这里进行参数的初始化
	LCD_Clear(BLACK);
}
  		  
/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
#if USE_HORIZONTAL
	LCD_WriteReg(0x38,xEnd);
	LCD_WriteReg(0x39,xStar);
	LCD_WriteReg(0x36,yEnd);
	LCD_WriteReg(0x37,yStar);
#else	
	LCD_WriteReg(0x36,xEnd);
	LCD_WriteReg(0x37,xStar);
	LCD_WriteReg(0x38,yEnd);
	LCD_WriteReg(0x39,yStar);
#endif
	LCD_WriteReg(lcddev.setxcmd,xStar);
	LCD_WriteReg(lcddev.setycmd,yStar);
	
	LCD_WriteRAM_Prepare();				
} 

/*************************************************
函数名：LCD_SetCursor
功能：设置光标位置
入口参数：xy坐标
返回值：无
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);  //HS
	LCD_WR_DATA(Xpos); 		 
	LCD_WR_REG(lcddev.setycmd);  //VS
	LCD_WR_DATA(Ypos);
	LCD_WriteRAM_Prepare();	//开始写入GRAM	
} 

//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x22;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=220;
	lcddev.height=176;
	lcddev.setxcmd=0x21;
	lcddev.setycmd=0x20;
	LCD_WriteReg(0x01, 0x001C);	
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0x2700);

#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=176;
	lcddev.height=220;
	lcddev.setxcmd=0x20;
	lcddev.setycmd=0x21;
	LCD_WriteReg(0x01, 0x011C);
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0xA700);
#endif
}	  

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
	}
}



