#ifndef		__FRAMEBOX_H
#define		__FRAMEBOX_H

#include "Interface.h"
/*
时间:2018 6 9
作者：latefly
功能:一个界面窗口的练习尝试

重新的方式隔绝底层的限制直接调用底层的有限个方法
这里需要底层的方法
drawpoint
setcursor

*/


typedef struct
{
	int X;
	int Y;
	int height;
	int width;
	int HeadHight;
	int BottomHight;
	char title[10];
	int id;
	
}__FrameBox;

typedef struct
{
	int X;
	int Y;
	int height;
	int width;
	int owner;
	
}__FrameVeiw;


//此处的方法需要底层的实现
#define FrameDrawPoint(Color) LCD_WR_DATA(Color)
#define FrameSetCursor(Xpos,Ypos) LCD_SetCursor(Xpos,Ypos)

/*
每个frame都需要id来进行区别
到时候每个组件都是以id来进行加入的

此时的Frame还未加入事件加入事件以后还会有一个跟进的一个
操作跟随的优先级指针

*/
//内置的调用方法方便测试所以这里列出来
void FrameClear(u16 Color);
void FrameFull(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawCircle(u16 x0,u16 y0,u8 r,u16 Color);
void FrameFont16(unsigned int x,unsigned int y,unsigned char index);
void FrameFont32(unsigned int x,unsigned int y,unsigned char index);
void FrameShowChar(u16 x,u16 y,u8 num,u8 size,u16 Color);
void FrameShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Color);

//组件以及容器的方法
void FrameBoxInit(void);
char FrameBox(void);
char FrameStart(void);
char FramePlay(void);
char FrameSeting(void);
char FrameTest(void);

//这个视图窗口先为文字准备着以后再为其他的控件准备
char AddMessage(char *message);
void ShowMessage(void);

#endif




