#include "bsp.h"

//Dbus��ΪPA3���Ǵ���2��RX��
volatile unsigned char Dbus_rx_buffer[25];

//��ʼ������
RC_Ctl_t RC_Ctl={
				{1024,1024,1024,1024,0,0},
				{0,0,0,0,0},
				{0}
				};


void Dbus_Init()
{
    //GPIO�˿�����
    GPIO_InitTypeDef	GPIO_InitStructure;
    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef	NVIC_InitStructure;

    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);

    USART_DeInit(USART2);  //��λ����2

    //USART2_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = 100000;//������100k
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��
    USART_InitStructure.USART_Mode = USART_Mode_Rx;	//����ģʽ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_Init(USART2, &USART_InitStructure); //��ʼ������1����
	
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���

    //USART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
}

void USART2_IRQHandler(void)                	//����2�жϷ������
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
    {
        DMA_InitTypeDef		DMA_InitStructure;
        USART_InitTypeDef	USART_InitStructure;
        NVIC_InitTypeDef	NVIC_InitStructure;
        USART_DeInit(USART2);  //��λ����1

        delay_ms(3);
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE);		//ʹ��DMA2ʱ��

        //USART1 ��ʼ������
        USART_InitStructure.USART_BaudRate = 100000;//������100k
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
        USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��
        USART_InitStructure.USART_Mode = USART_Mode_Rx;	//����ģʽ
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
        USART_Init(USART2, &USART_InitStructure); //��ʼ������2����

        USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
        USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);//��������DMA

        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        DMA_DeInit(DMA1_Stream5);
        DMA_InitStructure.DMA_Channel = DMA_Channel_4;//ͨ��ѡ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);//DMA�����ַ
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Dbus_rx_buffer;//DMA�洢��0��ַ
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//�洢��������ģʽ
        DMA_InitStructure.DMA_BufferSize = 18;//���ݴ�����
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ��ȣ�8λ
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ��ȣ�8λ
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ʹ��ѭ��ģʽ
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//������ȼ�
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//��ʹ��FIFO
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//
        DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;//DMA_MemoryBurst_Single����������ֵ��ͬ��ò��Ҫд���
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
        DMA_Init(DMA1_Stream5,&DMA_InitStructure);
		
        DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA1_Stream5,ENABLE);
    }
}

void DMA1_Stream5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))//��ѯ������5�����Ƿ����
    {
        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
        RC_Ctl.rc.ch0 = (Dbus_rx_buffer[0]| (Dbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
        RC_Ctl.rc.ch1 = ((Dbus_rx_buffer[1] >> 3) | (Dbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
        RC_Ctl.rc.ch2 = ((Dbus_rx_buffer[2] >> 6) | (Dbus_rx_buffer[3] << 2) | //!< Channel 2
                         (Dbus_rx_buffer[4] << 10)) & 0x07ff;
        RC_Ctl.rc.ch3 = ((Dbus_rx_buffer[4] >> 1) | (Dbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
        RC_Ctl.rc.s1 = ((Dbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
        RC_Ctl.rc.s2 = ((Dbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right
        RC_Ctl.mouse.x = Dbus_rx_buffer[6] | (Dbus_rx_buffer[7] << 8); //!< Mouse X axis
        RC_Ctl.mouse.y = Dbus_rx_buffer[8] | (Dbus_rx_buffer[9] << 8); //!< Mouse Y axis
        RC_Ctl.mouse.z = Dbus_rx_buffer[10] | (Dbus_rx_buffer[11] << 8); //!< Mouse Z axis
        RC_Ctl.mouse.press_l = Dbus_rx_buffer[12]; //!< Mouse Left Is Press ?
        RC_Ctl.mouse.press_r = Dbus_rx_buffer[13]; //!< Mouse Right Is Press ?
        RC_Ctl.key.v = Dbus_rx_buffer[14] | (Dbus_rx_buffer[15] << 8); //!< KeyBoard value
    }
}
