/*******************************************************************************
* @file    TM1637.h
* @author  
* @date     
*******************************************************************************/
#ifndef TM1637_H
#define TM1637_H

#include "sys.h"
 
#define TM1637_OUT_DIO1           PCout(0)

#define TM1637_READ_DIO1           PCin(0)

//IO·½ÏòÉèÖÃ
#define TM1637_DIO_IN()     {GPIOC->CRL&=0XF000000F;GPIOC->CRL|=0X08888880;}
#define TM1637_DIO_OUT()    {GPIOC->CRL&=0XF000000F;GPIOC->CRL|=0X03333330;}

/** 
  * @brief  Configuration Dots's Mode enumeration  
  */
typedef enum
{ 
	DulDot = 0x00,
  LowDot= 0x01,
  HighDot= 0x2,
}DisDotSetting;
typedef enum
{ 
	sty = 0x00,
  Jep = 0x01, 
}DisDotMode;



void TM1637_Delay_us(unsigned  int Nus); 

void TM1637_Display_1(void);
void TM1637_DisplayChar_1(unsigned char ch,unsigned char p);
void time_1(void);

void TM1637_Display_2(void);
void TM1637_DisplayChar_2(unsigned char ch,unsigned char p);
void time_2(void);

void TM1637_Display_3(void);
void TM1637_DisplayChar_3(unsigned char ch,unsigned char p);
void time_3(void);

void TM1637_Display_4(void);
void TM1637_DisplayChar_4(unsigned char ch,unsigned char p);
void time_4(void);

void TM1637_Display_5(void);
void TM1637_DisplayChar_5(unsigned char ch,unsigned char p);
void time_5(void);

void TM1637_Display_6(void);
void TM1637_DisplayChar_6(unsigned char ch,unsigned char p);
void time_6(void);

void Digital_off_1(void);
void Digital_off_2(void);
void Digital_off_3(void);
void Digital_off_4(void);
void Digital_off_5(void);
void Digital_off_6(void);
#endif
