#ifndef		__FRAMEBOX_H
#define		__FRAMEBOX_H
#include "Font.h"
#include "Interface.h"
//#include <math.h>
/*
这里直接使用DrawPoint方法将会使得绘图的效率很低所以只要在这里将这个方法改进

*/


#define DrawPoint(x,y,color)	 LCD_DrawPoint(x,y,color)












/******************************************函数定义***********************************************/
void SetXLimitation(int Lx,int Hx);
void SetYLimitation(int Ly,int Hy);
int GetBackgroudColor(void);
int GetFrontColor(void);
void SetBackgroudColor(int color);
void SetFrontColor(int color);


void FrameDrawPoint(int x,int y);
void FrameClear(u16 Color);
void FrameFullAera(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawCircle(u16 x0,u16 y0,u8 r,u16 Color);
void FrameShowChar(u16 x,u16 y,u8 num,u8 size,u16 Color);
void FrameShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Color);

#endif




