#include "FrameBox.h"
#include "Font.h"
/*
时间:2018 6 13
1.先作为一个普通的界面进行页面描述
2.写入一些简单的功能
	例如是一个文本域，或者一个列表框
3.底层提供的方法不应该超过三个
	这里想到的时setCuror drawPoint 
*/
#define LCD_H_MAX 220
#define LCD_W_MAX 176

#define FrameMaxValue 10
u16 BackgroundColor=0x00;




/**********************************************基础方法***********************************************/
void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //?????? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //?????? 
	else if(delta_x==0)incx=0;//??? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//??? 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //????????? 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//???? 
	{  
		FrameSetCursor(uRow,uCol);
		FrameDrawPoint(Color);//?? 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}




void FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	FrameDrawLine(x1,y1,x2,y1,Color);
	FrameDrawLine(x1,y1,x1,y2,Color);
	FrameDrawLine(x1,y2,x2,y2,Color);
	FrameDrawLine(x2,y1,x2,y2,Color);
}
void FrameDrawCircle(u16 x0,u16 y0,u8 r,u16 Color)
{
	
	int a,b;
	int di;
	r*=2;
	a=0;b=r;  
	di=3-(r<<1);             //??????????
	while(a<=b)
	{
		FrameSetCursor(x0+a,y0-b);
		FrameDrawPoint(Color); 		//5
		FrameSetCursor(x0+b,y0-a);
 		FrameDrawPoint(Color);             //0 
		FrameSetCursor(x0+b,y0+a);
		FrameDrawPoint(Color);             //4    
		FrameSetCursor(x0+a,y0+b);		
		FrameDrawPoint(Color);             //6 
		FrameSetCursor(x0-a,y0+b);
		FrameDrawPoint(Color);             //1   
		FrameSetCursor(x0-b,y0+a);    
 		FrameDrawPoint(Color);  
		FrameSetCursor(x0-a,y0-b);		
		FrameDrawPoint(Color);             //2  
		FrameSetCursor(x0-b,y0-a);		
  	FrameDrawPoint(Color);             //7   
		a++;
		//??Bresenham????     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}
//直接不带背景显示
void FrameShowChar(u16 x,u16 y,u8 num,u8 size,u16 Color)
{  							  
  u8 temp,t1,t;
	u16 y0=y;      			     
	//????		   
	num=num-' ';//???????
	
	for(t=0;t<size;t++)
	 {   
				if(size==12)temp=asc2_1206[num][t];  //??1206??
				else temp=asc2_1608[num][t];		 //??1608?? 	                          
				for(t1=0;t1<8;t1++)
				{			    
						if(temp&0x80)
						{
							FrameSetCursor(x,y);
							FrameDrawPoint(Color);
						}
						 
						temp<<=1;
						y++;
						if(y>=320)return;//????
						if((y-y0)==size)
						{
							y=y0;
							x++;
							if(x>=240)return;//????
							break;
						}
				}
		}			
}   

void FrameShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Color)
{         
	u8 x0=x;
	width+=x;
	height+=y;
	FrameFull(x,y,width,height,WHITE);
    while((*p<='~')&&(*p>=' '))//?????????!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//??
        FrameShowChar(x,y,*p,size,Color);
        x+=size/2;
        p++;
    }  
}






void FrameClear(u16 Color)
{
	u32 index=0;      
	u32 totalpoint=240;
	totalpoint*=320; 	//?????
	FrameSetCursor(0x0000,0x0000);	//?????? 
	//LCD_WriteRAM_Prepare();     //????GRAM	 	
	BackgroundColor=Color;  
	for(index=0;index<totalpoint;index++)
	{
		FrameDrawPoint(Color);
	}
}
void FrameFull(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	FrameSetCursor(sx,i);
		for(j=0;j<xlen;j++)
		FrameDrawPoint(color);
	}
}
/*********************************************创建的调用方法***************************************************/
char FrameBox()
{
	
	//int x,int y,int width,int height,char *title
	char isCreate=1;
	
	return isCreate;
}
char FrameStart(void){
	FrameClear(BLACK);
	FrameDrawLine(44,34,88,110,WHITE);
	FrameDrawLine(44,34,176,110,WHITE);

	FrameDrawLine(44,186,88,110,WHITE);
	FrameDrawLine(44,186,176,110,WHITE);
	FrameShowString(88,200,20,16,16,(u8 *)"-latefly",WHITE);
	return 0;
}

char FramePlay(void){
		FrameClear(YELLOW);
//	FrameFull(0,0,LCD_W_MAX,10,YELLOW);//状态条
//	FrameFull(0,10,10+20,30,BLUE);//设置
//	FrameFull(30,10,170,30,BRED);//标题
//	FrameFull(0,0,LCD_W_MAX,10,RED);//音效
	
	FrameDrawLine(0,10,176,10,BLACK);//状态栏
	FrameDrawRectangle(0, 10, 30, 30,BLACK);//设置
	FrameDrawRectangle(30, 10, 146, 30,BLACK);//标题
	FrameDrawRectangle(146, 10, 146, 30,BLACK);//音效
	
	FrameDrawRectangle(0,152,176,160,BLACK);//主窗口
	
	FrameDrawRectangle(0,160,176,180,BLACK);//工具条
	
	FrameDrawRectangle(0,180,176,220,BLACK);//播放按钮的位置
	FrameDrawLine(58,180,58,220,BLACK);//分割线
	FrameDrawLine(118,180,118,220,BLACK);//分割线
	//FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
	//FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
	return 0;
}
char FrameSeting(void){
	
	return 0;
}
char FrameTest(void){
	FrameClear(BLACK);
	FrameShowString(0,0,20,16,16,(u8 *)"This is a test frame!!!",WHITE);
	FrameShowString(88,175,20,16,16,(u8 *)"-latefly",WHITE);
	ShowMessage();
	return 0;
}







/*******************MessageBox ***********************/
/***********************************************
date:2019 6 19
author:latefly
description:
version:0.0.02
这里将会重写利用queue的形式同时这里也将会是调试的接口的开始界面

***********************************************/

#define Max_Message 13
#define Max_Character   50


//tips:last byte to mark is used.
char Message[Max_Message][Max_Character]={0};//Init array.
char StatePointer=0;
char LimitedTimes=10;//to show 10 message on screen onece.
char FormatSart[20]={0};


void ClearMessage()
{
		FrameClear(BLACK);
}

void ShowMessage()
{
		int location=0;
		location=(StatePointer-1)*20;
		FrameShowString(0,location,240,16,16,(u8 *)Message[StatePointer-1],WHITE);
		printf("StatePointer:%d\n",StatePointer);
}

char AddMessage(char *message)
{
    char i;
		if(StatePointer==0)ClearMessage();
	  printf("StatePointer:%d\n Message:%s\n",StatePointer,message);
    for(i=0;i<Max_Character-1;i++)
    {
        Message[StatePointer][i]=*message;
        message++;
    }
    Message[StatePointer][i]=0x01;//mark used.
    StatePointer++;
    if(StatePointer==Max_Message)StatePointer=0;
    ShowMessage();
    return StatePointer;
}
