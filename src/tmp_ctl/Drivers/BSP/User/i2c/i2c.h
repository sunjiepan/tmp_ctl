#ifndef __I2C_H
#define	__I2C_H

#include "stm32f10x.h"

//*********变量及IO口定义*********
typedef unsigned char uchar;
typedef unsigned int  uint;
#define		RTC_Address			0x64//RTC器件地址
//uchar   data1[8];  		// EEPROM数据
//uchar	Sram[8]={"01234567"};	//通用数据缓存器

typedef	struct
{
	uchar second;
	uchar	minute;
	uchar	hour;
	uchar	week;
	uchar	day;
	uchar	month;
	uchar	year;
}S_Time;	
	

/***********************************************************************
***PB7 SDA
***********************************************************************/
//#define SDA_IN	    PBin(7)     //输入
#define SDA_OUT	      PBout(7)    //输出

/***********************************************************************
***PB6 SCL
***********************************************************************/
#define SCL_IN	    PBin(6)	//输入
#define SCL_OUT	    PBout(6) 	//输出


#define  SDA_IN	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态

/* 定义控制IO的宏 */
#define SDA_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_7)
#define SDA_H					digitalHi(GPIOB,GPIO_Pin_7)
#define SDA_L					digitalLo(GPIOB,GPIO_Pin_7)

#define SCL_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_7)
#define SCL_H					digitalHi(GPIOB,GPIO_Pin_7)
#define SCL_L					digitalLo(GPIOB,GPIO_Pin_7)

#define		RTC_Address			0x64//RTC器件地址
//EEPROM/NVSRAM地址定义。尾缀为A的内部不带EEPROM存储器
#define		EE_FirstPage_Address		0xA0//第一页（尾缀B、C、D、E、F型有）
#define		EE_SecondPage_Address		0xA2//第二页（尾缀B、C型有）
#define		EE_ThirdPage_Address		0xA4//第三页（尾缀C型）
#define		EE_FourthPage_Address		0xA6//第四页（尾缀C型）
#define		EE_FifthPage_Address		0xA8//第五页（尾缀C型）
#define		EE_SixthPage_Address		0xAA//第六页（尾缀C型）
#define		EE_SeventhPage_Address		0xAC//第七页（尾缀C型）
#define		EE_EighthPage_Address		0xAE//第八页（尾缀C型）

#define		true  			1
#define 	false 			0

/**********************************************************************
*返回值定义
***********************************************************************/
#define true  1
#define false 0
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C1 有关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
    
  /* PB6-I2C1_SCL、PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//************************************************************
// 实时时钟SD24XX读写C51演示程序
// AT89S52   11.0592MHz
// E-mail:   FAE@whwave.com.cn
// TEL：		 0755-83114387
// Last update:                   2014/11/14
//************************************************************

void SD2405_GPIO_Config(void);
/*********I2C延时4us***********/
void I2CWait(void);
/********开启SD24XX的I2C总线********/
uchar I2CStart(void);
/********关闭SD24XX的I2C总线*******/
void I2CStop(void);
/*********发送 ACK*********/
void I2CAck(void);
/*********发送NO ACK*********/
void I2CNoAck(void);
/*********读取ACK信号*********/
uchar I2CWaitAck(void);
/************MCU向SD24XX发送一个字节*************/
void I2CSendByte(uchar demand);
/*********MCU从SD24XX读入一字节*********/
uchar I2CReceiveByte(void);
/******I2C写一个字节******/
uchar I2CWriteOneByte(uchar DeviceAddress,uchar add, uchar date);
/******I2C读一个字节程序******/
uchar I2CReadOneByte(uchar DeviceAddress,uchar add);
/******写SD24XX允许程序******/
uchar WriteTimeOn(void);
/******写SD24XX禁止程序******/
uchar WriteTimeOff(void);
/******读SD24XX实时数据寄存器******/
uchar I2CReadDate(S_Time	*psRTC);
/******写SD24XX实时数据寄存器******/
uchar I2CWriteDate(S_Time	SetRTC);
/******设置SD24XX报警中断演示程序演示******/
void WriteALARM(void);
/******设置SD24XX倒计时中断演示******/
void SetDjs(void);
/******关闭SD24XX倒计时中断程序******/
void ClrDjs(void) ;
/******设置SD24XX频率中断演示******/
void SetFrq(void);
/******关闭SD24XX频率中断******/
void ClrFrq(void);


#endif /* __I2C_H */
