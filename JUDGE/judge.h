#ifndef _JUDGE_H
#define _JUDGE_H

#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "stm32f4xx_conf.h"
#include "usart3.h"
#include "string.h"

typedef struct{
	u8 	SOF;			//����֡��ʼ�ֽڣ��̶�ֵΪ0xA5;
	u16 DataLength;		//����֡��Data����;
	u8	Seq;			//�����;
	u8	CRC8;			//֡ͷCRC8;
}FrameHeader_t;

typedef enum {
	CmdID_1		=	0x0001,			//����������״̬��10HzƵ�����ڷ���;
	CmdID_2		=	0x0002,			//�˺����ݣ��յ��˺�ʱ����;
	CmdID_3		=	0x0003,			//ʵʱ������ݣ����䵯��ʱ����;
	CmdID_4		=	0x0004,			//ʵʱ���ʺ��������ݣ�50HzƵ�����ڷ���;
	CmdID_5		=	0x0005,			//ʵʱ���ؽ������ݣ�50HzƵ�����ڷ���;
	CmdID_6		=	0x0006,			//����������ݣ���������ʱ����;
	CmdID_7		=	0x0007,			//��ȡ��buff,������غ���һ��;
	CmdID_8		=	0x0100 			//�������Զ������ݣ�������ʾ�ڲ�������;
}CmdID_t;

typedef  __packed struct  {
    uint16_t remainTime;  			//����ʣ��ʱ��
    uint16_t remainLifeValue;  		//ʣ��Ѫ��
	uint8_t CurrentLevel;			//��ǰ�ȼ�
	uint16_t MaxLifeValue;			//���Ѫ��
}tStudentGameProcessInfo;  

typedef  __packed struct  {
    uint8_t strikeId:4;  
    uint8_t type:4;  				//Ѫ���仯����
    uint16_t data;  				//Ѫ���仯ֵ
}tLifeChangedInfo;  

typedef __packed struct  {
	//new
	uint8_t bulletType;				//�ӵ�����
	uint8_t bulletFreq;				//��Ƶ
	float	bulletSpeed;			//����
	//old
//	float realBulletShootSpeed;  	//�ӵ�����
//	float realBulletShootFreq;  	//�ӵ���Ƶ
//	float realGolfShootSpeed;  		//�߶����ӵ�����
//	float realGolfShootFreq;  		//�߶����ӵ���Ƶ
}tStudentInterRealShootData;  

typedef struct {
	u8 	validFlage;					//λ�á��Ƕ���Ϣ��Ч��־λ-----0:��Ч;1:��Ч;
	u32	x;							//λ��X����ֵ;
	u32	y;							//λ��Y����ֵ;
	u32	z;							//λ��Z����ֵ;
	u32	yaw;						//ǹ�ڳ���Ƕ�ֵ;
}position_t;

typedef struct{
	u16 stageRemianTime;			//��ǰ�׶�ʣ��ʱ�䣬��λs;
	u8	gameProgress;				//��ǰ�����׶�;
									//0:δ��ʼ����;
									//1:׼���׶�;
									//2:�Լ�׶�;
									//3:5s����ʱ;
									//4:��ս��;
									//5:����������;
	u8	robotLevel;					//�����˵�ǰ�ȼ�;
	u16	remainHP;					//�����˵�ǰѪ��;
	u16	maxHP;						//��������Ѫ��;
	position_t	position;			//λ�ã��Ƕ���Ϣ;
}extGameRobotState_t;

typedef struct{
	u8	armorType:	4;			//���ݵ�0-3bits:���仯����Ϊװ���˺�����ʶװ��ID��
															//0x0:0��װ�ף�ǰ ��;
															//0x1:1��װ�ף��� ��;
															//0x2:2��װ�ף��� ��;
															//0x3:3��װ�ף��� ��;
															//0x4:4��װ�ף���1��;
															//0x5:5��װ�ף���2��;
															//��������;
	u8	hurtType:	4;			//���ݵ�4-7bits:Ѫ���仯���ͣ�
															//0x0:װ���˺����ܵ�������;
															//0x1:ģ�����;
															//0x2:���賬��;
															//0x3:���賬Ƶ;
															//0x4:ǹ�ڳ�����;
															//0x5:���̳�����;
}extRobotHurt_t;

typedef struct{
	u8	bulletType;				//��������;
								//0:17mm����;
								//1:42mm����;
	u8	bulletFreq;				//������Ƶ;
	u32	bulletSpeed;			//��������;
	u32	reseved;				//����;
}extShootData_t;

typedef struct{
	float	chassisVolt;		//���������ѹ;
	float	chassisCurrent;		//�����������;
	float	chassisPower;		//�����������;
	float	chassisPowerBuffer;	//���̹��ʻ���;
	uint16_t	shooterHeat17;	//17mmǹ������;
	uint16_t	shooterHeat42;	//42mmǹ������;
}extPowerHeatData_t;

typedef struct{
	u8	cardType;				//������;
								//0:�����ӳɿ�;
								//1:�����ӳɿ�;
								//2:�췽��Ѫ��;
								//3:������Ѫ��;
								//4:�췽���������ؿ�;
								//5:�������������ؿ�;
	u8	cardIdx;				//�������ţ����������ֲ�ͬ����
}extRfidDetect_t;

typedef struct{
	u8	winner;					//�������;
								//0:ƽ��;
								//1:�췽ʤ;
								//2:����ʤ;
}extGameResult_t;

typedef struct{
	u8	buffType;				//Buff����;
								//0:�����ӳ�;
								//1:�����ӳ�;
								//2:��ô���������;
	u8	buffAddition;			//�ӳɰٷֱȣ�����10����10%��
}extGetBuff_t;

typedef struct{
	float	data1;					//�Զ�������1;
	float	data2;					//�Զ�������2;
	float	data3;					//�Զ�������3;
	uint8_t	mask;					//�Զ�������4;
}extShowData_t;

typedef struct{
	u16	GameProcessInfo_number1;					//  �ķ��ʹ���
	u16	PowerHeatData_number2;					//  �ķ��ʹ���
	u16	number3;					//  �ķ��ʹ���
	u16	number4;					//  �ķ��ʹ���;
} transmission_number_t;

typedef struct{
	FrameHeader_t FrameHeader;
	CmdID_t	CmdID;
	union{
		extGameRobotState_t	extGameRobotState;			//0x0001:����������״̬;
		extRobotHurt_t		extRobotHurt;				//0x0002:�˺�����;
		extShootData_t		extShootData;				//0x0003:ʵʱ�����Ϣ;
		extPowerHeatData_t	extPowerHeatData;			//0x0004:ʵʱ������������;
		extRfidDetect_t		extRfidDetect;				//0x0005:���ؽ�������;
		extGameResult_t		extGameResult;				//0x0006:����ʤ������;
		extGetBuff_t		extGetBuff;					//0x0007:Buff��ȡ����;
		extShowData_t		extShowData;				//0x0008:�������Զ�������;
	}Data;
	u16	FrameTail;
}DMA1_Stream1_buffer_t;

extern tStudentGameProcessInfo GameProcessInfo;
extern tLifeChangedInfo LifeChangedInfo;
extern tStudentInterRealShootData RealShootData;
extern extPowerHeatData_t PowerHeatData;
extern int shootingcounter;
extern DMA1_Stream1_buffer_t DMA1_Stream1_buffer;

u8 Get_CRC8_Check_Sum( u8 *pchMessage, u32 dwLength, u8 ucCRC8 );
u16 Verify_CRC8_Check_Sum( u8 *pchMessage, u16 dwLength );
void Append_CRC8_Check_Sum( u8 *pchMessage, u32 dwLength );
u16 Get_CRC16_Check_Sum( u8 *pchMessage, u32 dwLength, uint16_t wCRC );
u32 Verify_CRC16_Check_Sum( u8 *pchMessage, u32 dwLength );
void Append_CRC16_Check_Sum( u8 * pchMessage, u32 dwLength );

float storage2float(uint8_t *storage);
void Judge_Init(void);
void TIM8_UP_TIM13_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
void send_data_to_gimbal(void);
	
uint16_t JUDGE_GET_Infantry_HeatData( void );
uint8_t JUDGE_GET_Infantry_LevelData( void );

/** 
  * @brief  frame header structure definition
  */
//typedef __packed struct
//{
//  uint8_t  sof;
//  uint16_t data_length;
//  uint8_t  seq;
//  uint8_t  crc8;
//} frame_header_t;

#define COMPUTER_FRAME_BUFLEN 5
#define UP_REG_ID 0xA5
#define HEADER_LEN 5
#define CMD_LEN 2
#define CRC_LEN 2

void data_pack_handle(uint8_t *p_data, uint16_t len);
void TIM3_Configuration(void);

void Judge_Send_Data_Process(void);
void Set_Judge_Send_Data( uint8_t i, float data);
void Set_Judge_Send_Flag( uint8_t i, uint8_t data );

void Set_Flag_Shoot(uint8_t flag);
uint8_t Get_Flag_Shoot(void);
#endif




