#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h" 

void Adc_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(void); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
double CurrentADC(u8 times);

#endif
