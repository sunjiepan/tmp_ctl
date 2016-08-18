#ifndef _PID_H
#define _PID_H

#include "stm32f1xx_hal.h"

#define     max_value_error               50.0
#define     min_value_error              -50.0//控温区域

//extern volatile uint8_t  get_now_temp_flag;
//extern volatile uint8_t  pwm_con_time_flag;
//extern volatile uint8_t  pid_tune_flag;//初始为0 即pid阶段 采用默认的值    1 为自整定过程
//extern volatile uint8_t  enable_pid_sec_flag;
//extern volatile uint8_t  pid_self_sec_flag;
extern volatile uint8_t  Pid_Setok;
extern volatile uint8_t  pid_self_first_status_flag;
extern volatile uint8_t  once_add_1_flag;
extern volatile uint8_t  enable_calc_min_max_flag;
extern unsigned short time0_temp2;
extern float  KV_value;  //温度差值
extern int zero_across_counter;
extern long pid_self_time_sec;

extern int cool_ack_counter;
extern int hot_ack_counter;
extern int k_pid_self_counter;


extern float  Proportion;           //  比例常数 Proportional Const
extern float  Integral;           //  积分常数 Integral Const        
extern float  Derivative;           //  微分常数 Derivative Const
extern float  LastError;           //  Error[-1]
extern float  PrevError;           //  Error[-2]
extern float  SumError;           //  Sums of Errors
extern float  dError;
extern float  Error;

//pid
extern float           SV_value; //设定温度值
extern float           PV_value;  //用于参与计算的当前温度值
//extern float           PV_oldvalue;  //用于参与计算的当前温度值
//extern volatile float  P_value;  //比例带 比如56.3代表56.3%  0.0--200.0
//extern int             I_value;  //积分时间  秒  0-3600
//extern int             D_value;   //微分时间  秒  0-900

extern int  pid_result;                //DAC输出

extern float max_temp;  //初始温度等于0
extern float min_temp;//初始温度等于100
extern float sum_temp;  //初始温度等于0
extern float aver_temp ;

extern float  KC;  //临界比例系数  初始默认的值
extern int    TC;   //振荡周期      初始默认的值

extern float T_Hight;
extern float T_LOW; //温度
extern long TIME_Hight;
extern long TIME_LOW;	//具体的秒
extern float pid_self_calc_buffer[4];

void pid_pro(void);
void Pid_Sinceset(void);
void PidParameter_Sinceset(void);//PID自整定参数
void PidParameter_pro(void);//PID参数
void Preheat(void);// 预热
#endif /* _PID_H */