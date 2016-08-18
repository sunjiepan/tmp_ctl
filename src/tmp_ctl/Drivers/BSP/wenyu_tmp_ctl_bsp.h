#ifndef __WENYU_TMP_CTL_BSP_H
#define __WENYU_TMP_CTL_BSP_H
#include "stm32f1xx_hal.h"

typedef enum 
{
  Buzzer = 0  
} Buzzer_TypeDef;

#define BUZZERn															1

#define BUZZER_PIN                         GPIO_PIN_11
#define BUZZER_GPIO_PORT                   GPIOB
#define BUZZER_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define BUZZER_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define BUZZERx_GPIO_CLK_ENABLE(__INDEX__)  do {BUZZER_GPIO_CLK_ENABLE(); } while(0)

#define BUZZERx_GPIO_CLK_DISABLE(__INDEX__) BUZZER_GPIO_CLK_DISABLE())

void BSP_BUZZER_Init(Buzzer_TypeDef Buzzer);
void BSP_BUZZER_On(Buzzer_TypeDef Buzzer);
void BSP_BUZZER_Off(Buzzer_TypeDef Buzzer);
void BSP_BUZZER_Toggle(Buzzer_TypeDef Buzzer);

typedef enum 
{
  DS18B20 = 0  
} DS18B20_TypeDef;

#define DS18B20n														1

#define DS18B20_PIN                         GPIO_PIN_2
#define DS18B20_GPIO_PORT                   GPIOA
#define DS18B20_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define DS18B20_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()  

#define DS18B20x_GPIO_CLK_ENABLE(__INDEX__)  do {DS18B20_GPIO_CLK_ENABLE(); } while(0)

#define DS18B20x_GPIO_CLK_DISABLE(__INDEX__) DS18B20_GPIO_CLK_DISABLE())
#define BSP_DS18B20_DATA_IN()										 HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_PIN)
#define LOW																		GPIO_PIN_RESET
#define HIGH																	GPIO_PIN_SET
#define BSP_DS18B20_DATA_OUT(X)									 HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_PIN, X)

uint8_t BSP_DS18B20_Init(void);
uint16_t BSP_DS18B20_Get_Temp(void);
void BSP_udelay(uint32_t);

#endif
