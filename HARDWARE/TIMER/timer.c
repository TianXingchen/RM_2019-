#include "bsp.h"
int testcnt=0;
void TIM14_Init(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///ʹ��TIM3ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //����ʱ��14�����ж�
	TIM_Cmd(TIM14,ENABLE); //ʹ�ܶ�ʱ��14
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //��ʱ��14�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	testcnt++;
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //����ж�
	{	
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //����жϱ�־λ		
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


