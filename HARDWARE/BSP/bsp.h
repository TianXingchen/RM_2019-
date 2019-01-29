#ifndef	__BSP_H__
#define	__BSP_H__

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "math.h"
#include "stdio.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "can1.h"
#include "solver.h"
//#include "dbus.h"
#include "chassispid.h"
#include "visualscope.h"
#include "timer.h"
#include "adc.h"
#include "controlloop.h"
#include "judge.h"
#include "usart3.h"

#define LED0 						PCout(1) 
#define LED1 						PCout(0)	
#define BEEP 						PCout(2) 

#define MaxInc 						100		//�����޷�
#define MaxCurrent 					4000	//��������޷�
#define coefficient 				0.3		//��������ϵ��
#define Imax 						2.3		//�������ٽ�������
#define Imin 						-2.3	//�������ٽ���С����
#define L1							0.177	//RM2018����������������ߴ�/2(m)
#define L2							0.172	//RM2018�����������̺���ߴ�/2(m)
#define R							1.52	//����ֱ��(dm)
/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN 			((uint16_t)364 )
#define RC_CH_VALUE_OFFSET 			((uint16_t)1024)
#define RC_CH_VALUE_MAX 			((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP 					((uint16_t)1)
#define RC_SW_MID 					((uint16_t)3)
#define RC_SW_DOWN 					((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W 		((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S 		((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A 		((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D 		((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q 		((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E 		((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT 	((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL 	((uint16_t)0x01<<7)

typedef struct
{
	struct
	{
		int16_t  p1;
		int16_t  p2;
		int16_t  p3;
		int16_t  p4;
	}position;				//���̵�����ص�λ��ֵ	
	struct
	{
		int16_t  v1;
		int16_t  v2;
		int16_t  v3;
		int16_t  v4;
	}velocity;				//���̵�����ص��ٶ�ֵ	
	struct
	{
		int16_t  c1;
		int16_t  c2;
		int16_t  c3;
		int16_t  c4;
	}current;				//���̵���������ֵ
	struct
	{
		int16_t  w1;
		int16_t  w2;
		int16_t  w3;
		int16_t  w4;
	} SetSpeed;				//���̵���ٶȸ���ֵ
	struct
	{
		int16_t  VX;
		int16_t  VY;
		int16_t  W0;
		
	} component;			//���̵������������
	struct
	{
		int16_t  I1;
		int16_t  I2;
		int16_t  I3;
		int16_t  I4;
	} CurrentFeedBack;		//���̵����������������
} ChassisMotorMsg;

typedef struct
{
    struct
    {
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        uint8_t s1;
        uint8_t s2;
    } rc;					//ң�����ĸ�ͨ�����������뿪��
    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;				//����϶������������Ҽ����
    struct
    {
        uint16_t v;			
    } key;					//���̰���
} RC_Ctl_t;

typedef struct
{
	float Kp;				//����
	float Ki;				//����
	float Kd;				//΢��
	float error_now;		//�������
	float error_last;		//�ϴ����
	float error_last_last;	//���ϴ����
	int16_t pid_out;		//PID���
}Inc_PID;					//����ʽPID

extern Inc_PID ChassisPID_1;
extern Inc_PID ChassisPID_2;
extern Inc_PID ChassisPID_3;
extern Inc_PID ChassisPID_4;
extern volatile unsigned char Dbus_rx_buffer[25];	//DBUS���ջ�����
extern  RC_Ctl_t RC_Ctl;
extern ChassisMotorMsg ChassisMotor;
extern double CurrentSensor_I;						//�������������ֵ

void bsp_Init(void);

#endif
