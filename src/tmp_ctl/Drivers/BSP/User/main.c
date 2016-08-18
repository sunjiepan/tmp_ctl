/******************** (C) COPYRIGHT 2015 sunrise **************************
 * 文件名  ：main.c
 * 描述    ：双CAN通讯实验，这是主机部分，向从机发送ABCD         
 * 实验平台:
 * 库版本  ：ST3.5.0
 *
 * 作者    :sunrise
 * 公司    :
 **********************************************************************************/
#include "stm32f10x.h"
#include "Buzzer.h" 
#include "SysTick.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "exti.h"
#include "spi_flash.h"
#include "led_seg.h"
#include "TiMbase.h" 
#include "ds18b20.h"
//#include "i2c.h"
 __IO u32 Total_time=0,k=0;
int16_t data[7]={0};//{38,180,240,95,465,567};

unsigned char NumDis[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0X6F,0X40};//0123456789 :
int16_t cache[7]={0};
int16_t temp[2]={0};
volatile u32 time = 0,time_1=0,time_2=0; // ms 计时变量
unsigned char Enable[6]={0};//光耦标志位
unsigned char G[6]={0};//光耦标志位
unsigned char flag=0;//标志位
signed char channel=0;
unsigned char K1=0,K2=0,K3=0,add=0;
extern unsigned char x;
extern unsigned char mark;
extern void Writeseg_Date(unsigned char saddr,int data);
extern void Writetemp_Date(unsigned char saddr,int data);
char i=0, saddr=0x00,y=0,y_1=0;
void Delay_1(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/* 
 * 函数名：main
 * 描述  : “从机”的主函数
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
{
	unsigned char i;
	
	/* LED config */
	SysTick_Init();
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//SD2405_GPIO_Config();
	BUZ_GPIO_Config();
	EXTI_GPIO_Config();
	SPI_FLASH_Init();
	SPI_SEG_Config();
	TM1681Init();
  TIM2_NVIC_Configuration();
  TIM2_Configuration();
	Delay_us(100);
	read_data();
		for(i=0;i<6;i++)
	{
		cache[i]=data[i];
	}
	K2=1;
	WriteSameDate(0x00, 0x00, 64);
  while(DS18B20_Init());//初始化DS18B20,兼检测18B20
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
while(1)
	{
	
//			  if(flag==1)  {BUZ_1(ON);}
//		    if(flag==2)  {flag=0; BUZ_1(OFF);}
			  
		  switch(flag)
					{
						case 1:
							{
								Writeseg_Date(0x00,cache[0]);
								Delay_us(10);
								Writeseg_Date(0x08,cache[1]);
								Delay_us(10);
								Writeseg_Date(0x10,cache[2]);
								Delay_us(10);
								Writeseg_Date(0x18,cache[3]);
								Delay_us(10);
								Writeseg_Date(0x20,cache[4]);
								Delay_us(10);
								Writeseg_Date(0x28,cache[5]);
								temp[0]=DS18B20_Get_Temp();
								Writetemp_Date(0X30,temp[0]);
								x=0;
							}		
				       break;
					case 2:
						{
						 	flag=0;
							if(channel==1)
								 {
									 Writeclose(0x28);
								 }
						 else 
								 {
									 Writeseg_Date(0x28,cache[5]);
									 if((Enable[5]==1)&&(channel==0))
									 {
										 G[5]=1;
										 cache[5]-=1;
										 if(cache[5]<0)
											{
												if((-cache[5])>599)
												{
													Enable[5]=0;
												}
												K1=1;				
											}	
										 }
									 }
						 if(channel==2)
								 {
									 Writeclose(0x20);
								 }
						 else
									 {
										 Writeseg_Date(0x20,cache[4]);
										 if((Enable[4]==1)&&(channel==0))
										 {
											 G[4]=1;
											 cache[4]-=1;
												 if(cache[4]<0)
														{
															if((-cache[4])>599)
															{
																Enable[4]=0;
															}
															K1=1;				
														}	
										 }
									 }
						 if(channel==3)
								 {
									 Writeclose(0x18);
								 }
						 else
								 {
									 Writeseg_Date(0x18,cache[3]);
										if((Enable[3]==1)&&(channel==0))
										 {
											 G[3]=1;
											 cache[3]-=1;
											 if(cache[3]<0)
												{
													if((-cache[3])>599)
													{
														Enable[3]=0;
													}
													K1=1;				
												}	
										 }
								 }
						 if(channel==4)
								 {
									 Writeclose(0x10);
								 }
						 else
								 {
									 Writeseg_Date(0x10,cache[2]);
										if((Enable[2]==1)&&(channel==0))
										 {
											 G[2]=1;
											 cache[2]-=1;
											 if(cache[2]<0)
												{
													if((-cache[2])>599)
													{
														Enable[2]=0;
													}
													K1=1;				
												}	
										 }
								 }
						 if(channel==5)
								 {
									 Writeclose(0x08);
								 }
						 else
								 {
									 Writeseg_Date(0x08,cache[1]);
										if((Enable[1]==1)&&(channel==0))
										 {
											 G[1]=1;
											 cache[1]-=1;
											 if(cache[1]<0)
												{
													if((-cache[1])>599)
													{
														Enable[1]=0;
													}
													K1=1;				
												}	
										 }
								 }
						 if(channel==6)
								 {
									 Writeclose(0x00);
								 }
						 else
								 {
									 Writeseg_Date(0x00,cache[0]);
										if((Enable[0]==1)&&(channel==0))
										 {
											 G[0]=1;
											 cache[0]-=1;
											 if(cache[0]<0)
												{
													if((-cache[0])>599)
													{
														Enable[0]=0;
													}
													K1=1;				
												}	
										 }
								 }
						 if(channel==7)
								 {
									 Writeclose(0x30);
								 }
						 else
								 {
										temp[0]=DS18B20_Get_Temp();
										Writetemp_Date(0X30,temp[0]);
								 }
							x=1;
					}	
	      }
//       	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0)&&(channel==0)&&(y_1==0)) 
//					 {
//						Delay_us(1000);
//						if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0)&&(channel==0)&&(y_1==0))    
//							 {						
//									{
//									 K2=1;
//									 Enable[5]=1;
//									 mark=1;
//									 G[5]=0;
//									 y=1;
//									 y_1=1;
//									}
//								}
//							}
//						if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==1)&&(y==1)) 
//						 {
//							 Delay_us(1000);
//							 if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==1)&&(y==1))
//								{
//								K2=1;
//								y=0;
//								y_1=0;
//								cache[5]=data[5];
//								Enable[5]=0;
//								mark=0;
//								K1=0;
//								G[5]=0;
//								}
//							}

      if((K1==1)&&(K2==0))
				{
					BUZ_1(ON);
					Delay_us(50000);
					BUZ_1(OFF);
				}
				if(K2==1)
				{
					BUZ_1(ON);
					Delay_us(50000);
					BUZ_1(OFF);
					K2=0;
				}					
}						
}



/*********************************************END OF FILE**********************/
