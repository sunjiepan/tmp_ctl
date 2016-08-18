/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "Buzzer.h"   
extern void Delay_1(__IO uint32_t nCount);
 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void BUZ_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOD和GPIOC的外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
			/*选择要控制的GPIOD引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	//编号D2

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOD0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);	
		
}
void buzzer(unsigned char data)
{
			switch(data)
				{
					case 1:		BUZ_1(ON);
										Delay_1(0xfffff);
										BUZ_1(OFF);
										data=0;
										break;
					case 2:		BUZ_1(ON);
										Delay_1(0xfffff);
										BUZ_1(OFF);
										break;
				}
}

/*********************************************END OF FILE**********************/
