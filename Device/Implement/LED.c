#include "Interface.h"



void LEDInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  //PB13/14/15上拉
}

void LEDOn()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);  //PB13/14/15上拉
}
void LEDOff()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  //PB13/14/15上拉
}
