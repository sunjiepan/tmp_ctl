/*******************************************************************************
* @file    TM1637.h
* @author  
* @date     
*******************************************************************************/
#ifndef TM1637_H
#define TM1637_H

#include "sys.h"
 
#define TM1637_OUT_DIO1           PCout(0)
#define TM1637_OUT_DIO2           PCout(1)  
#define TM1637_OUT_DIO3           PCout(2)  
#define TM1637_OUT_DIO4           PCout(3)  
#define TM1637_OUT_DIO5           PCout(4)  
#define TM1637_OUT_DIO6           PCout(5) 


#define TM1637_READ_DIO1           PCin(0)
#define TM1637_READ_DIO2           PCin(1)  
#define TM1637_READ_DIO3           PCin(2)  
#define TM1637_READ_DIO4           PCin(3)  
#define TM1637_READ_DIO5           PCin(4)  
#define TM1637_READ_DIO6           PCin(5) 

//IO·½ÏòÉèÖÃ
#define TM1637_DIO_IN()     {GPIOB->CRL&=0XF000000F;GPIOB->CRL|=0X08888880;}
#define TM1637_DIO_OUT()    {GPIOB->CRL&=0XF000000F;GPIOB->CRL|=0X03333330;}

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
