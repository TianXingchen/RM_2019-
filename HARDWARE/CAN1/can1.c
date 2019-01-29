#include "bsp.h"

//初始化参数
ChassisMotorMsg ChassisMotor={	
							 {0,0,0,0},
							 {0,0,0,0},
							 {0,0,0,0},
							 {0,0,0,0},	
							 {0,0,0},
							 {0,0,0,0}
                             };
/**********************************************************************
*功能：Can1的初始化函数
***********************************************************************/
void CAN1_Init(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    GPIO_InitTypeDef       GPIO_InitStructure;
    NVIC_InitTypeDef       NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);//复用功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
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
    
    CAN_InitStructure.CAN_TTCM = DISABLE;//非时间触发通信模式
    CAN_InitStructure.CAN_ABOM = DISABLE;//软件自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE;//睡眠模式通过软件唤醒（清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART = DISABLE;//CAN 硬件将自动重发送消息，直到根据 CAN 标准消息发送成功
    CAN_InitStructure.CAN_RFLM = DISABLE;//FIFO报文不锁定，满后新的覆盖旧的
    CAN_InitStructure.CAN_TXFP = ENABLE;//优先级由请求顺序（时间顺序）确定
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
    CAN_InitStructure.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 0;//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//32位
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0收到消息中断允许		   
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);//发送邮箱为空中断 
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
		
		//接收来自云台电路板的数据，0x401
			if(RxMessage.StdId == 0x401)
			{
				ChassisMotor.component.VY = -(RxMessage.Data[0]<<8 | RxMessage.Data[1]);//纵向分量
				ChassisMotor.component.VX = RxMessage.Data[2]<<8 | RxMessage.Data[3];//横向分量
				ChassisMotor.component.W0 = RxMessage.Data[4]<<8 | RxMessage.Data[5];//旋转分量
			}
			
		//接收来自底盘电调的数据，0x201~0x204
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
*功能：通过can1发送数据给底盘电调，进而控制底盘的运动
*输入：四个电流值，范围-32768~32768  
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

