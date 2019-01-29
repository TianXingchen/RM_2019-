#include "bsp.h"

double power = 0;
static double energy = 60;

void TIM13_Init(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  
	
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //允许定时器13更新中断
	TIM_Cmd(TIM13,ENABLE); //使能定时器13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //定时器13中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update) == SET) //溢出中断
	{	
		TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //清除中断标志?
		power = __fabs(CurrentSensor_I * 24);
		if(power > 80)
		{
			energy -= (power-80) * 0.1;
		}
	}
}

