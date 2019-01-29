#include "bsp.h"

//��ʼ������
ChassisMotorMsg ChassisMotor={	
							 {0,0,0,0},
							 {0,0,0,0},
							 {0,0,0,0},
							 {0,0,0,0},	
							 {0,0,0},
							 {0,0,0,0}
                             };
/**********************************************************************
*���ܣ�Can1�ĳ�ʼ������
***********************************************************************/
void CAN1_Init(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    GPIO_InitTypeDef       GPIO_InitStructure;
    NVIC_InitTypeDef       NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);//���ù���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
    
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    
    CAN_InitStructure.CAN_TTCM = DISABLE;//��ʱ�䴥��ͨ��ģʽ
    CAN_InitStructure.CAN_ABOM = DISABLE;//����Զ����߹���
    CAN_InitStructure.CAN_AWUM = DISABLE;//˯��ģʽͨ��������ѣ����CAN->MCR��SLEEPλ)
    CAN_InitStructure.CAN_NART = DISABLE;//CAN Ӳ�����Զ��ط�����Ϣ��ֱ������ CAN ��׼��Ϣ���ͳɹ�
    CAN_InitStructure.CAN_RFLM = DISABLE;//FIFO���Ĳ������������µĸ��Ǿɵ�
    CAN_InitStructure.CAN_TXFP = ENABLE;//���ȼ�������˳��ʱ��˳��ȷ��
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
    CAN_InitStructure.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 0;//������0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//32λ
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0�յ���Ϣ�ж�����		   
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);//��������Ϊ���ж� 
}

void CAN1_TX_IRQHandler(void) //CAN TX
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
    }
}

void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
	
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);  
		
		//����������̨��·������ݣ�0x401
			if(RxMessage.StdId == 0x401)
			{
				ChassisMotor.component.VY = -(RxMessage.Data[0]<<8 | RxMessage.Data[1]);//�������
				ChassisMotor.component.VX = RxMessage.Data[2]<<8 | RxMessage.Data[3];//�������
				ChassisMotor.component.W0 = RxMessage.Data[4]<<8 | RxMessage.Data[5];//��ת����
			}
			
		//�������Ե��̵�������ݣ�0x201~0x204
			if(RxMessage.StdId == 0x201)
			{
				ChassisMotor.position.p1 = RxMessage.Data[0]<<8 | RxMessage.Data[1];//0~8191
				ChassisMotor.velocity.v1 = RxMessage.Data[2]<<8 | RxMessage.Data[3];//prm
			}
			if(RxMessage.StdId == 0x202)
			{
				ChassisMotor.position.p2 = RxMessage.Data[0]<<8 | RxMessage.Data[1];//0~8191
				ChassisMotor.velocity.v2 = RxMessage.Data[2]<<8 | RxMessage.Data[3];//prm
			}
			if(RxMessage.StdId == 0x203)
			{
				ChassisMotor.position.p3 = RxMessage.Data[0]<<8 | RxMessage.Data[1];//0~8191
				ChassisMotor.velocity.v3 = RxMessage.Data[2]<<8 | RxMessage.Data[3];//prm
			}
			if(RxMessage.StdId == 0x204)
			{
				ChassisMotor.position.p4 = RxMessage.Data[0]<<8 | RxMessage.Data[1];//0~8191
				ChassisMotor.velocity.v4 = RxMessage.Data[2]<<8 | RxMessage.Data[3];//prm
			}
    }
}

/**********************************************************************
*���ܣ�ͨ��can1�������ݸ����̵�����������Ƶ��̵��˶�
*���룺�ĸ�����ֵ����Χ-32768~32768  
***********************************************************************/
void Set_Current(int16_t Current_1,int16_t Current_2,int16_t Current_3,int16_t Current_4)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = 0x200;	
    TxMessage.RTR = CAN_RTR_DATA;		
    TxMessage.IDE = CAN_Id_Standard;	
    TxMessage.DLC = 8;	

    TxMessage.Data[0] = Current_1>>8;
    TxMessage.Data[1] = Current_1 & 0xff;
	TxMessage.Data[2] = Current_2>>8;
    TxMessage.Data[3] = Current_2 & 0xff;
	TxMessage.Data[4] = Current_3>>8;
    TxMessage.Data[5] = Current_3 & 0xff;
	TxMessage.Data[6] = Current_4>>8;
    TxMessage.Data[7] = Current_4 & 0xff;

    CAN_Transmit(CAN1, &TxMessage);
	
}

