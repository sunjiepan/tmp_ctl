#include "stm32f1xx_hal.h"
#include "wenyu_tmp_ctl_bsp.h"
#include "stm32f1xx_hal.h"
//#include "cmsis_os.h"

GPIO_TypeDef* GPIO_PORT[BUZZERn] = {BUZZER_GPIO_PORT};

const uint16_t GPIO_PIN[BUZZERn] = {BUZZER_PIN};

void BSP_BUZZER_Init(Buzzer_TypeDef Buzzer)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /* Enable the GPIO_BUZZER Clock */
  BUZZERx_GPIO_CLK_ENABLE(Buzzer);

  /* Configure the GPIO_BUZZER pin */
  gpioinitstruct.Pin    = GPIO_PIN[Buzzer];
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIO_PORT[Buzzer], &gpioinitstruct);

  /* Reset PIN to switch off the BUZZER */
  HAL_GPIO_WritePin(GPIO_PORT[Buzzer],GPIO_PIN[Buzzer], GPIO_PIN_RESET);
}

void BSP_BUZZER_On(Buzzer_TypeDef Buzzer)
{
  HAL_GPIO_WritePin(GPIO_PORT[Buzzer], GPIO_PIN[Buzzer], GPIO_PIN_SET); 
}

void BSP_BUZZER_Off(Buzzer_TypeDef Buzzer)
{
  HAL_GPIO_WritePin(GPIO_PORT[Buzzer], GPIO_PIN[Buzzer], GPIO_PIN_RESET); 
}

void BSP_BUZZER_Toggle(Buzzer_TypeDef Buzzer)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Buzzer], GPIO_PIN[Buzzer]);
}



void BSP_udelay(uint32_t nus)
{ 
	uint32_t ticks;
	uint32_t told,tnow,tcnt;
	uint32_t reload = SysTick->LOAD;
	uint32_t fac_us = HAL_RCC_GetHCLKFreq()/8000000;
	ticks = nus * fac_us;
	tcnt = 0;
	//vTaskSuspendAll();
	told = SysTick->VAL;       
	while(1)
	{
		tnow = SysTick->VAL; 
		if(tnow != told)
		{	   
			if(tnow < told)
				tcnt+= told - tnow;
			else 
				tcnt += reload - tnow + told;	   
			told = tnow;
			if(tcnt >= ticks)
				break; 
		}  
	} 
	//xTaskResumeAll(); 
}
//  
//void BSP_udelay(uint16_t nCount)  
//{  
//  __IO uint16_t TIMCounter = nCount;  
//    
//  uint16_t start = TIM2->CNT;  
//  uint16_t end = start + TIMCounter;  
//  __IO uint16_t read;  
//    
//  while (1) {  
//    read = TIM2->CNT;  
//    if(read < start)  
//      read = 0xFFFF - start + read;  
//    if (read >= end)  
//      break;  
//  }    
//} 

/*

 */
static void BSP_DS18B20_Mode_IPU(void)
{  
 	GPIO_InitTypeDef gpioinitstruct = {0};
	gpioinitstruct.Pin = DS18B20_PIN;
	gpioinitstruct.Mode = GPIO_MODE_INPUT;	
	HAL_GPIO_Init(DS18B20_GPIO_PORT, &gpioinitstruct);
}
/*

 */
static void BSP_DS18B20_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef gpioinitstruct = {0};
	gpioinitstruct.Pin = DS18B20_PIN;
	gpioinitstruct.Mode = GPIO_MODE_OUTPUT_OD;	
	gpioinitstruct.Speed  = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(DS18B20_GPIO_PORT, &gpioinitstruct);
}

static void BSP_DS18B20_Rst(void)
{
	BSP_DS18B20_Mode_Out_PP();
	BSP_DS18B20_DATA_OUT(LOW);
	BSP_udelay(750);
	BSP_DS18B20_DATA_OUT(HIGH);
	BSP_udelay(15);
}

static uint8_t BSP_DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	

	BSP_DS18B20_Mode_IPU();
	

	while(BSP_DS18B20_DATA_IN() && pulse_time < 100 )
	{
		pulse_time ++;
		BSP_udelay(1);
	}	

	if( pulse_time >=100)
		return 1;
	else
		pulse_time = 0;
	
	while(!BSP_DS18B20_DATA_IN() && pulse_time<240 )
	{
		pulse_time ++;
		BSP_udelay(1);
	}	
	if( pulse_time >= 240 )
		return 1;
	else
		return 0;
}

static uint8_t BSP_DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	BSP_DS18B20_Mode_Out_PP();
	BSP_DS18B20_DATA_OUT(LOW);
	BSP_udelay(10);
	BSP_DS18B20_Mode_IPU();
	//BSP_udelay(2);
	
	if( BSP_DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	BSP_udelay(45);
	
	return dat;
}

uint8_t BSP_DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = BSP_DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

void BSP_DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	BSP_DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* ?0??1????????60us */
		if (testb)
		{			
			BSP_DS18B20_DATA_OUT(LOW);
			/* 1us < ???? < 15us */
			BSP_udelay(8);
			
			BSP_DS18B20_DATA_OUT(HIGH);
			BSP_udelay(58);
		}		
		else
		{			
			BSP_DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			BSP_udelay(70);
			
			BSP_DS18B20_DATA_OUT(HIGH);			
			/* 1us < Trec(????) < ???*/
			BSP_udelay(2);
		}
	}
}

void BSP_DS18B20_Start(void)
{
	BSP_DS18B20_Rst();	   
	BSP_DS18B20_Presence();	 
	BSP_DS18B20_Write_Byte(0XCC);
	BSP_DS18B20_Write_Byte(0X44);		
}

uint8_t BSP_DS18B20_Init(void)
{
	//DS18B20_GPIO_Config();
	BSP_DS18B20_Rst();
	
	return BSP_DS18B20_Presence();
}

uint16_t BSP_DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	BSP_DS18B20_Rst();	   
	BSP_DS18B20_Presence();
	BSP_DS18B20_Write_Byte(0X33);
	
	BSP_DS18B20_Rst();	   
	BSP_DS18B20_Presence();	 
	BSP_DS18B20_Write_Byte(0XCC);				
	BSP_DS18B20_Write_Byte(0X44);

	BSP_udelay(2);
	
	BSP_DS18B20_Rst();
  BSP_DS18B20_Presence();
	BSP_DS18B20_Write_Byte(0XCC);				
  BSP_DS18B20_Write_Byte(0XBE);				
	
	tplsb = BSP_DS18B20_Read_Byte();		 
	tpmsb = BSP_DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		
		f_tem = (~s_tem+1) * 6.25;	
	else
		f_tem = s_tem * 6.25;
	
	return f_tem; 	
}

