#include "bsp.h"
int testcnt=0;
void TIM14_Init(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///使能TIM3时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器14更新中断
	TIM_Cmd(TIM14,ENABLE); //使能定时器14
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //定时器14中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	testcnt++;
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //溢出中断
	{	
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位		
		ControlLoop();
		send_data_to_gimbal();
		
		if(testcnt==3000)
		{
//		ChassisMotor.component.VX = 0;
//		ChassisMotor.component.VY = 0;
//		ChassisMotor.component.W0 = 0;
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		}else if(testcnt==6000)
		{
		testcnt=0;
//		ChassisMotor.component.VX =1000;
//		ChassisMotor.component.VY = 0;
//		ChassisMotor.component.W0 = 0;
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		}
	}
	
}


