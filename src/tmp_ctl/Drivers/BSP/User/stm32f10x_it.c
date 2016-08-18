/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "exti.h"
#include "SysTick.h"
#include "Buzzer.h" 
#include "spi_flash.h"
#include "led_seg.h"

extern unsigned char Enable[6];
extern signed char channel;
extern __IO u32 Total_time;
extern int16_t data[7];
extern int16_t cache[7];
extern unsigned char G[5];
extern unsigned char K1,K2,K3;
extern void TimingDelay_Decrement(void);
extern void Writeseg_Date(unsigned char saddr,int data);
extern volatile u32 time,time_1,time_2;
unsigned char mark=0;
extern unsigned char flag;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
static void Delay_1(unsigned  int Nus)
{
 for(;Nus>0;Nus--)
	{
	  __nop();
	}
}
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	time++;
	TimingDelay_Decrement();
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
	{
					K2=1;
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)&&(channel==0))
					{
						Enable[0]=1;
						mark=1;
						G[0]=0;
					}
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0)||(G[0]==1))
					{
						cache[0]=data[0];
						Enable[0]=0;
						mark=0;
						K1=0;
						G[0]=0;
					}
							
						
				EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
	}  
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //确保是否产生了EXTI Line中断
	{
					K2=1;
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)&&(channel==0))
					{
						Enable[1]=1;
						mark=1;
						G[1]=0;
					}
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)||(G[1]==1))
					{
						cache[1]=data[1];
						Enable[1]=0;
						mark=0;
						K1=0;
						G[1]=0;
					}
		EXTI_ClearITPendingBit(EXTI_Line1);     //清除中断标志位
	}  
}
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //确保是否产生了EXTI Line中断
	{
					K2=1;
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==1)&&(channel==0))
					{
						Enable[2]=1;
						mark=1;
						G[2]=0;
					}
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0)||(G[2]==1))
					{
						cache[2]=data[2];
						Enable[2]=0;
						mark=0;
						K1=0;
						G[2]=0;
					}
		EXTI_ClearITPendingBit(EXTI_Line2);     //清除中断标志位
	}  
}
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //确保是否产生了EXTI Line中断
	{
					K2=1;
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==0)&&(channel==0))
					{
						Enable[3]=1;
						mark=1;
						G[3]=0;
					}
					if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==1)||(G[3]==1))
					{
						cache[3]=data[3];
						Enable[3]=0;
						mark=0;
						K1=0;
						G[3]=0;
					}
		EXTI_ClearITPendingBit(EXTI_Line3);     //清除中断标志位
	}  
}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //确保是否产生了EXTI Line中断
	{
					K2=1;
					if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)==1)&&(channel==0))
					{
						Enable[4]=1;
						mark=1;
						G[4]=0;
					}
					if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)==0)||(G[4]==1))
					{
						cache[4]=data[4];
						Enable[4]=0;
						mark=0;
						K1=0;
						G[4]=0;
					}
		EXTI_ClearITPendingBit(EXTI_Line4);     //清除中断标志位
	}  
}
void EXTI9_5_IRQHandler(void)

{
		/*按键左*/	
	
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
			{
				static u8 button = 0;
				if(!button)
					{  
						button = 1;
						Delay_1(10000);
						if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==1)||(mark==1)) goto end_8;
						channel--;
						if(channel<=0)
						{
							channel=7;
						}
//						if(K3>=1)
//						{
//								write_data();
//								K3=0;
//						}
						end_8: button = 0;
						}  
				EXTI_ClearITPendingBit(EXTI_Line8);
			}
			/*按键下*/
			if(EXTI_GetITStatus(EXTI_Line9) != RESET)
			{
			  static u8 button = 0;
				EXTI->IMR &= ~(EXTI_Line9); //屏蔽外部中断9
				if(!button)
					{  
						button = 1;
						Delay_1(10000);
						if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)==1)&&(channel==0)) goto end_9;
						Delay_1(10000);
						if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)==1)&&(channel==0)) goto end_9;
							switch(channel)
							{
								case 1:	data[5]+=60;
												if(data[5]>5940)
												{
													data[5]=60;
												}
												
												cache[5]=data[5];
												break;
								case 2:	data[4]+=60;
												if(data[4]>5940)
												{
													data[4]=60;
												}
												
												cache[4]=data[4];
												break;
								case 3:	data[3]+=60;
												if(data[3]>5940)
												{
													data[3]=60;
												}
												
												cache[3]=data[3];
												break;
								case 4:	data[2]+=60;
												if(data[2]>5940)
												{
													data[2]=60;
												}
												
												cache[2]=data[2];
												break;
								case 5:	data[1]+=60;
												if(data[1]>5940)
												{
													data[1]=60;
												}
												cache[1]=data[1];
												break;
								case 6:	data[0]+=60;
												if(data[0]>5940)
												{
													data[0]=60;
												}
												
												cache[0]=data[0];
												break;
							}							
						
						

						end_9: button = 0;
					}
				EXTI->IMR|= EXTI_Line9;
       EXTI_ClearITPendingBit(EXTI_Line9);
			 
			}
}			
void EXTI15_10_IRQHandler(void)

{
	/*按键确定*/
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
			{
				static u8 button = 0;
				if(!button)
					{  
						button = 1;
						Delay_1(10000);
						if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==1)&&(channel==0)) goto end_10;
						write_data();
						K2=1;
						mark=0;
						channel=0;
						end_10: button = 0;
					}
				EXTI_ClearITPendingBit(EXTI_Line10);
			}
	/*按键上*/
		if(EXTI_GetITStatus(EXTI_Line11) != RESET)
			{
				static u8 button = 0;
				EXTI->IMR &= ~(EXTI_Line11); //屏蔽外部中断11
				if(!button)
					{  
						button = 1;
						Delay_1(10000);
						if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==1)&&(channel==0)) goto end_11;
							//K3=1;
						switch(channel)
							{
								case 1:	data[5]-=60;
												if(data[5]<60)
												{
													data[5]=5940;
												}
												
												cache[5]=data[5];
												break;
								case 2:	data[4]-=60;
												if(data[4]<60)
												{
													data[4]=5940;
												}
												
												cache[4]=data[4];
												break;
								case 3:	data[3]-=60;
												if(data[3]<60)
												{
													data[3]=5940;
												}
												
												cache[3]=data[3];
									break;
								case 4:	data[2]-=60;
												if(data[2]<60)
												{
													data[2]=5940;
												}
												
												cache[2]=data[2];
												break;
								case 5:	data[1]-=60;
												if(data[1]<60)
												{
													data[1]=5940;
												}
												
												cache[1]=data[1];
									break;
								case 6:	data[0]-=60;
												if(data[0]<60)
												{
													data[0]=5940;
												}
												
												cache[0]=data[0];
									break;
							}
					
						end_11: button = 0;
					}
				EXTI->IMR|= EXTI_Line11;
				EXTI_ClearITPendingBit(EXTI_Line11);
				
			}
	/*右*/
if(EXTI_GetITStatus(EXTI_Line12) != RESET)
			{
				static u8 button = 0;
				if(!button)
					{  
						button = 1;
						Delay_1(10000);
						if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==1)||(mark==1)) goto end_12;
						channel++;
						if(channel>7)
						{
							channel=1;
						}
//						if(K3>=1)
//						{
//								write_data();
//								K3=0;
//						}
						
						end_12: button = 0;
					}
				EXTI_ClearITPendingBit(EXTI_Line12);
			}
			//GPIO_SetBits(GPIOB,GPIO_Pin_5);			
}

void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		flag+=1;
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//	CanRxMsg RxMessage;
//	vu8 i=0;
////	RxMessage.StdId=0x00;
////	RxMessage.ExtId=0x00;
////	RxMessage.IDE=0;
////	RxMessage.DLC=0;
////	RxMessage.FMI=0;
////	RxMessage.Data[0]=0x00;
////	RxMessage.Data[1]=0x00;
////  RxMessage.Data[2]=0x00;
////	RxMessage.Data[3]=0x00;
////	RxMessage.Data[4]=0x00;
////	RxMessage.Data[5]=0x00;
////	RxMessage.Data[6]=0x00;
////	RxMessage.Data[7]=0x00;
//	if( CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET )
//  {
//    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
//    received=1;
//    if((RxMessage.ExtId==0x1314)  && 
//       (RxMessage.IDE==CAN_ID_EXT)&& 
//       (RxMessage.DLC==8))
//    {
//      command_level=RxMessage.StdId;
//			for(i=0;i <8;i++)
//			{
//			Can1_Recv_Buf[i] =  RxMessage.Data[i];
//			}
//			received=1;
//			flag = 1; 					       //接收成功
//    }
//    else
//    {
//       flag = 0xff; 									//接收失败
//    }
// }
//}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
