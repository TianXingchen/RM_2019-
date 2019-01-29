#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h" 

void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(void); 				//获得某个通道值 
u16 Get_Adc_Average(u8 times);//得到某个通道给定次数采样的平均值  
double CurrentADC(u8 times);

#endif
