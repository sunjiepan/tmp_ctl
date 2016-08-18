#ifndef __XPT2046_SPI_H
#define __XPT2046_SPI_H
#include "stm32f10x_conf.h"
//XPT2046 与STM32107VCT6的连线方式
//MISO          PC11/SPI3_MISO REMAP
//MOSI          PC12/SPI3_MISO
//SCK          PC10/SPI3_SCK
//TP_CS         PC9
//INT          PC5

// A/D 通道选择命令字和工作寄存器
//#define	CHX 	0x90
//#define	CHY 	0xD0

#define TP_CS()  GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define TP_DCS() GPIO_SetBits(GPIOB,GPIO_Pin_12)

extern u16 X_Addata,Y_Addata;
//extern void delay_ms(u16 Nms);好像不延时也可以用哦
void XPT2046_Configuration(void);//配置GPIO/SPI3/EXTI中断 
u16 XPT2046_ReadX(void);
u16 XPT2046_ReadY(void);
#endif

