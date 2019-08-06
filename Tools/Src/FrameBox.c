#include "FrameBox.h"
//#define DrawPoint(x,y,color)	 LCD_DrawPoint(u16 x,u16 y,u16 Color)

#define		FrameHight	320
#define		FrameWidth	240
typedef struct
{
	int Height;
	int	Width;
	int FrontColor;
	int BackgroundColor;
	
}__Frame;
typedef struct
{
	int	Lx,Hx;
	int Ly,Hy;
}__DrawLimitation;


__DrawLimitation DrawLimitation={
	0,240.,0,320
};
__Frame Frame[5]={
	FrameHight,FrameWidth,0x0000,0x0000,
};




extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];




void SetXLimitation(int Lx,int Hx)
{
	DrawLimitation.Lx=Lx;
	DrawLimitation.Hx=Hx;
}

void SetYLimitation(int Ly,int Hy)
{
	DrawLimitation.Ly=Ly;
	DrawLimitation.Hy=Hy;
	
}
int GetBackgroudColor(void)
{
	return Frame[0].BackgroundColor;
}
int GetFrontColor(void)
{
	return Frame[0].FrontColor;
}

void SetBackgroudColor(int color)
{
	Frame[0].BackgroundColor=color;
}
void SetFrontColor(int color)
{
	Frame[0].FrontColor=color;
}




void FrameDrawPoint(int x,int y)
{
	
	if(x>=DrawLimitation.Lx&&x<=DrawLimitation.Hx)
	{	
		if(y>=DrawLimitation.Ly&&y<=DrawLimitation.Hy)
			DrawPoint(x,y,Frame[0].FrontColor);
	}
}

void FrameClear(u16 Color)
{

	int i,j;
	SetFrontColor(Color);
	for(j=0;j<=Frame[0].Height;j++)
	{
		for(i=0;i<=Frame[0].Width;i++)
		FrameDrawPoint(i,j);
	}
}

void FrameFullAera(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	int i,j;
	SetFrontColor(color);
	for(i=sx;i<ex;i++)
	{
		for(j=sy;j<ey;j++)
		{
			FrameDrawPoint(i,j);
		}
	}
}

void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //?????? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1;
	SetFrontColor(Color);
	
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
		FrameDrawPoint(uRow,uCol);
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
	SetFrontColor(Color);
	while(a<=b)
	{
		
		FrameDrawPoint(x0+a,y0-b);
		FrameDrawPoint(x0+b,y0-a);
		FrameDrawPoint(x0+b,y0+a); 
		FrameDrawPoint(x0+a,y0+b);
		FrameDrawPoint(x0-a,y0+b);
		FrameDrawPoint(x0-b,y0+a);    
		FrameDrawPoint(x0-a,y0-b); 
		FrameDrawPoint(x0-b,y0-a);		
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
	SetFrontColor(Color);
	for(t=0;t<size;t++)
	 {   
				if(size==12)temp=asc2_1206[num][t];  //??1206??
				else temp=asc2_1608[num][t];		 //??1608?? 	                          
				for(t1=0;t1<8;t1++)
				{			    
						if(temp&0x80)
						{
							FrameDrawPoint(x,y);
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
    while((*p<='~')&&(*p>=' '))//?????????!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//??
        FrameShowChar(x,y,*p,size,Color);
        x+=size/2;
        p++;
    }  
}


#define Max_Message 16
#define Max_Character   30


//tips:last byte to mark is used.
char Message[Max_Message][Max_Character]={0};//Init array.
char StatePointer=0;
char LimitedTimes=10;//to show 10 message on screen onece.
char FormatSart[20]={0};



