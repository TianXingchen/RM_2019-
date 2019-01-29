#include "bsp.h"

double power = 0;
static double energy = 60;

void TIM13_Init(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  
	
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //����ʱ��13�����ж�
	TIM_Cmd(TIM13,ENABLE); //ʹ�ܶ�ʱ��13
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //��ʱ��13�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update) == SET) //����ж�
	{	
		TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //����жϱ�־?
		power = __fabs(CurrentSensor_I * 24);
		if(power > 80)
		{
			energy -= (power-80) * 0.1;
		}
	}
}

