#ifndef __TMP_CTL_H
#define	__TMP_CTL_H

#include "stm32f1xx_hal.h"

typedef struct 
{
  uint8_t start_flag;
	uint8_t cmd;
	uint16_t data;
	uint8_t crc;
	uint8_t end_flag;
}CustomProtocol_TypeDef;

#define CMD_SET_BUZZER	0X01
#define CMD_SET_TEMP			0X02
#define CMD_GET_TEMP			0X03
#define CMD_SWITCH_HART 0X04
#define CMD_GET_CHANNEL 0X05
#define CMD_SET_TIMER		0X06
#define CMD_GET_TIMER		0X07

#define BUZZER_MODEL0		0X00
#define BUZZER_MODEL1		0X01
#define BUZZER_MODEL2		0X02

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      6
/* Size of Reception buffer */
#define RXBUFFERSIZE                      1

#define COM_RX_BUF_SIZE         (32)
#define COM_TX_BUF_SIZE         (32)

#define START_CHAR	0xFF
#define END_CHAR		0xFF

#define MIN_ERROR_VALUE (-200)
#define MAX_ERROR_VALUE (200)
#define MAX_DUTY_CYCLE	999
#define MID_DUTY_CYCLE	499
#define MIN_DUTY_CYCLE	0

#define DELAY_100MS		100
#define DELAY_200MS		200
#define DELAY_250MS		250
#define DELAY_500MS		500
#define DELAY_1S			1000

typedef struct {      
	uint16_t setValue;                 
	uint16_t feedbackValue;          
	int16_t err;                   
	int16_t err_next;                
	int16_t err_last;                
	float Kp,Ki,Kd;
	int32_t out,outNext;
}PID_TypeDef;

typedef struct
{  
  void       (*Init)(void);
  void       (*SetBuzzer)(uint16_t);
	void			 (*BuzzerON)(void);
	void			 (*BuzzerOFF)(void);
	void			 (*BuzzerOutPut)(void);
  void       (*SetTemp)(uint16_t);
  uint16_t   (*GetTemp)(void);
	uint16_t   (*GetCurrentTemp)(void);
	uint16_t   (*ReadCurrentTemp)(void);
	void       (*SwitchOnOff)(uint8_t);
  uint8_t    (*GetChannel)(void);
  void       (*SetTimer)(uint16_t);
  uint8_t    (*GetTimer)(void);
	void			 (*PWMOutPut)(uint16_t duty_cycle);
	HAL_StatusTypeDef       (*UartTransmit)(uint8_t *);
	HAL_StatusTypeDef       (*UartReceive)(uint8_t *);
	uint16_t   (*PID_Algorithm)(uint16_t,uint16_t);
	uint8_t 	 (*getCrc8)(uint8_t *);
	CustomProtocol_TypeDef aTxBuffer;
	CustomProtocol_TypeDef aRxBuffer;
}TmpCtl_TypeDef;

uint32_t RxWrite(uint8_t *buffter, uint32_t writeLen);
uint32_t RxRead(uint8_t *buffter, uint32_t buffterSize);
uint32_t TxWrite(uint8_t *buffter, uint32_t writeLen);
uint32_t TxRead(uint8_t *buffter, uint32_t buffterSize);

extern TmpCtl_TypeDef weiyu_tmp_ctr;
extern UART_HandleTypeDef huart1;
#endif /*tmp_ctl.h*/
