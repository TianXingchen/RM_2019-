#include "bsp.h"

//Dbus口为PA3，是串口2的RX口
volatile unsigned char Dbus_rx_buffer[25];

//初始化参数
RC_Ctl_t RC_Ctl={
				{1024,1024,1024,1024,0,0},
				{0,0,0,0,0},
				{0}
				};


void Dbus_Init()
{
    //GPIO端口设置
    GPIO_InitTypeDef	GPIO_InitStructure;
    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef	NVIC_InitStructure;

    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);

    USART_DeInit(USART2);  //复位串口2

    //USART2_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = 100000;//波特率100k
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验
    USART_InitStructure.USART_Mode = USART_Mode_Rx;	//接收模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_Init(USART2, &USART_InitStructure); //初始化串口1参数
	
    USART_Cmd(USART2, ENABLE);                    //使能串口

    //USART1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
}

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
    {
        DMA_InitTypeDef		DMA_InitStructure;
        USART_InitTypeDef	USART_InitStructure;
        NVIC_InitTypeDef	NVIC_InitStructure;
        USART_DeInit(USART2);  //复位串口1

        delay_ms(3);
        RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE);		//使能DMA2时钟

        //USART1 初始化设置
        USART_InitStructure.USART_BaudRate = 100000;//波特率100k
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
        USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验
        USART_InitStructure.USART_Mode = USART_Mode_Rx;	//接收模式
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
        USART_Init(USART2, &USART_InitStructure); //初始化串口2参数

        USART_Cmd(USART2, ENABLE);                    //使能串口
        USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);//开启串口DMA

        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        DMA_DeInit(DMA1_Stream5);
        DMA_InitStructure.DMA_Channel = DMA_Channel_4;//通道选择
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);//DMA外设地址
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Dbus_rx_buffer;//DMA存储器0地址
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//存储区到外设模式
        DMA_InitStructure.DMA_BufferSize = 18;//数据传输量
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储区增量模式
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度：8位
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度：8位
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//使用循环模式
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//最高优先级
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//不使用FIFO
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//
        DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;//DMA_MemoryBurst_Single？？？？？值相同，貌似要写这个
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
        DMA_Init(DMA1_Stream5,&DMA_InitStructure);
		
        DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA1_Stream5,ENABLE);
    }
}

void DMA1_Stream5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))//查询数据流5传输是否完成
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
