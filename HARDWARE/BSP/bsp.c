#include "bsp.h"

void bsp_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	USART1_Init(115200);	//���ڳ�ʼ��������Ϊ115200
	Uart3_Init(115200);		//�������ݸ���̨
	Judge_Init();
	LED_Init();
	CAN1_Init();
	//Dbus_Init();
	TIM14_Init();
	//TIM13_Init();
	//Adc_Init();

}
