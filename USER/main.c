#include "bsp.h"

//double CurrentSensor_I;

int main(void)
{
//	char p[15];
	bsp_Init();
	while(1)
	{
		
		//VisualScope(USART1,ChassisMotor.velocity.v2,0,0,ChassisMotor.SetSpeed.w2);
		delay_ms(100);
		printf("VX:%d",ChassisMotor.component.VX);
		printf("VY:%d",ChassisMotor.component.VY);
		printf("W0:%d\r\n",ChassisMotor.component.W0);
//		printf("chassisPower:%f\r\n",PowerHeatData.chassisPower);	
//		if(USART3_RX_STA&0x8000){
////改变p
//			if(USART3_RX_BUF[0]=='p'){
//				if(USART3_RX_BUF[1]=='+'){
//					ChassisPID_2.Kp=ChassisPID_2.Kp+0.05;
//					sprintf((char*)p,"ok！Kp=%04f\r\n",ChassisPID_2.Kp);
//					USART3_SendString(p);
//					USART3_RX_STA=0;
//				}
//				else if(USART3_RX_BUF[1]=='-'){
//					ChassisPID_2.Kp=ChassisPID_2.Kp-0.01;
//					sprintf((char*)p,"ok！Kp=%04f\r\n",ChassisPID_2.Kp);
//					USART3_SendString(p);
//					USART3_RX_STA=0;
//				}
//			}
////改变i			
//			if(USART3_RX_BUF[0]=='i'){
//				if(USART3_RX_BUF[1]=='+'){
//					ChassisPID_2.Ki=ChassisPID_2.Ki+0.02;
//					sprintf((char*)p,"ok！Ki=%04f\r\n",ChassisPID_2.Ki);
//					USART3_SendString(p);
//					USART3_RX_STA=0;
//				}
//				else if(USART3_RX_BUF[1]=='-'){
//					ChassisPID_2.Ki=ChassisPID_2.Ki-0.01;
//					sprintf((char*)p,"ok！Ki=%04f\r\n",ChassisPID_2.Ki);
//					USART3_SendString(p);
//					USART3_RX_STA=0;
//				}
//			}
////改变d
//			if(USART3_RX_BUF[0]=='d'){
//				if(USART3_RX_BUF[1]=='+'){
//					ChassisPID_2.Kd=ChassisPID_2.Kd+0.02;
//					sprintf((char*)p,"ok！Kd=%04f\r\n",ChassisPID_2.Kd);
//					USART3_SendString(p);
//					USART3_RX_STA=0;
//				}
//				else if(USART3_RX_BUF[1]=='-'){
//					ChassisPID_2.Kd=ChassisPID_2.Kd-0.01;
//					sprintf((char*)p,"ok！Kd=%04f\r\n",ChassisPID_2.Kd);
//					USART3_SendString(p);
//					USART3_RX_STA=0;
//				}
//			}
//		 
//		   if(USART3_RX_BUF[0]=='c'){
//				ChassisPID_2.Kp=0;
//			    ChassisPID_2.Ki=0;
//			    ChassisPID_2.Kd=0;
//			   USART3_SendString("clear all!");
//			   USART3_RX_STA=0;
//		   }
//		}
	}

}
