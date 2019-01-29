#include "bsp.h"

//Vx��Vy��W0�ֱ��ǵ��̺��ơ����˺���ת�ķ���
void Speed_Solving(int16_t Vx,int16_t Vy,int16_t W0)
{
	ChassisMotor.SetSpeed.w1 =  (Vy + Vx + W0 * (L1 + L2)) /R;
	ChassisMotor.SetSpeed.w2 = -(Vy - Vx - W0 * (L1 + L2)) /R;
	ChassisMotor.SetSpeed.w3 = -(Vy + Vx - W0 * (L1 + L2)) /R;
	ChassisMotor.SetSpeed.w4 =  (Vy - Vx + W0 * (L1 + L2)) /R;
}



