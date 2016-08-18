#ifndef __BUZZER_H
#define	__BUZZER_H

#include "stm32f10x.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  1
#define OFF 0

/* 带参宏，可以像内联函数一样使用 */
#define BUZ_1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)
					
#define TM1637_CLK(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)
	

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define BUZ_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_5)
#define BUZ_H		digitalHi(GPIOB,GPIO_Pin_5)
#define BUZ_L			digitalLo(GPIOB,GPIO_Pin_5)

#define LED2_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_7)
#define LED2_H		digitalHi(GPIOF,GPIO_Pin_7)
#define LED2_L			digitalLo(GPIOF,GPIO_Pin_7)

#define LED3_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_8)
#define LED3_H		digitalHi(GPIOF,GPIO_Pin_8)
#define LED3_L			digitalLo(GPIOF,GPIO_Pin_8)

void BUZ_GPIO_Config(void);
void buzzer(unsigned char data);

#endif /* __BUZZER_H */
