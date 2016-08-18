//#include "Dac.h"
//#include "Max6675.h"
//#include "Modbus.h"
#include "Pid.h"
//#include "Flash.h"

//#include "Process.h"

#define MIN_ERROR_VALUE 2
#define MAX_ERROR_VALUE 60

//volatile uint8_t  get_now_temp_flag   = 0;
//volatile uint8_t  pwm_con_time_flag = 0;
//volatile uint8_t  pid_tune_flag = 1;//³õÊ¼Îª0 ¼´pid½×¶Î ²ÉÓÃÄ¬ÈÏµÄÖµ    1 Îª×ÔÕû¶¨¹ı³Ì
//volatile uint8_t  enable_pid_sec_flag = 0;
//volatile uint8_t  pid_self_sec_flag   = 0;
volatile uint8_t  pid_self_first_status_flag = 0;
volatile uint8_t  once_add_1_flag     = 0;
volatile uint8_t  enable_calc_min_max_flag = 0;
volatile uint8_t  Pid_Setok = 0;

uint16_t time0_temp2=0;
int32_t zero_across_counter = 0;
int64_t pid_self_time_sec = 0;

int32_t cool_ack_counter    = 0;
int32_t hot_ack_counter     = 0;
int32_t k_pid_self_counter = 0;




float  Proportion  = 0.0;           //  ±ÈÀı³£Êı Proportional Const
float  Integral    = 0.0;           //  »ı·Ö³£Êı Integral Const        
float  Derivative  = 0.0;           //  Î¢·Ö³£Êı Derivative Const

float  LastError   = 0.0;           //  Error[-1]
float  PrevError   = 0.0;           //  Error[-2]
float  SumError    = 0.0;           //  Sums of Errors
float  dError      = 0.0;
float  Error       = 0.0;
//pid
float           SV_value           = 50.0; //Éè¶¨ÎÂ¶ÈÖµ
float           PV_value           = 0.0;  //ÓÃÓÚ²ÎÓë¼ÆËãµÄµ±Ç°ÎÂ¶ÈÖµ
float           KV_value           = 0.0;  //ÎÂ¶È²îÖµ
//float           PV_oldvalue        = 0.0;  //ÓÃÓÚ²ÎÓë¼ÆËãµÄµ±Ç°ÎÂ¶ÈÖµ
//volatile float  P_value            = 0.0;  //±ÈÀı´ø ±ÈÈç56.3´ú±í56.3%  0.0--200.0
//int             I_value            = 0;  //»ı·ÖÊ±¼ä  Ãë  0-3600
//int             D_value            = 0;   //Î¢·ÖÊ±¼ä  Ãë  0-900
int32_t  pid_result = 0;                //DACÊä³ö

float max_temp  = 0.0 ;  //³õÊ¼ÎÂ¶ÈµÈÓÚ0
float min_temp  = 100.0 ;//³õÊ¼ÎÂ¶ÈµÈÓÚ100
float sum_temp  = 0.0 ;  //³õÊ¼ÎÂ¶ÈµÈÓÚ0
float aver_temp	= 0.0 ;

float  KC = 1.0;  //ÁÙ½ç±ÈÀıÏµÊı  ³õÊ¼Ä¬ÈÏµÄÖµ
int32_t    TC = 40;   //Õñµ´ÖÜÆÚ      ³õÊ¼Ä¬ÈÏµÄÖµ

float T_Hight = 0.0;
float T_LOW   = 100.0; //ÎÂ¶È
int64_t TIME_Hight = 0;
int64_t TIME_LOW   = 0;	//¾ßÌåµÄÃë
float pid_self_calc_buffer[4]={0.0,0.0,0.0,0.0};


//0 Ëã³ö ÁÙ½çÔöÒæ KC ¼° Õñµ´ÖÜÆÚ TC
  // KC = (4*d)/(3.14*A)  ---> d = 5(Êä³ö·ùÖµ) ; A = ¼ÇÂ¼µÄÎÂ¶È×î¸ßÖµÓë×îµÍÖµµÄ²îÖµµÄ0.5±¶ ¼´£º(T_Hight - T_LOW)*0.5
  // KC = (4*5)/(3.14*((T_Hight - T_LOW)*0.5)) = 40/3.14/(T_Hight - T_LOW) =  12.7/(T_Hight - T_LOW)
  // KC = 12.7/(T_Hight - T_LOW)
  // TC = 2 * (TIME_Hight - TIME_LOW) ---> 2 * ( ¸ßµãÎÂ¶È¶ÔÓ¦Ê±¼ä - µÍµãÎÂ¶È¶ÔÓ¦Ê±¼ä )
  // TC = 2 * (TIME_Hight - TIME_LOW)
  //1 Ëã³ö ¾ßÌåµÄ±ÈÀıÏµÊı »ı·ÖÃëÊı Î¢·ÖÃëÊı
  //Proportion = 0.6*KC
  //I_value    = 0.5*TC
  //D_value    = 0.125*TC
  //2 Ëã³ö¾ßÌåµÄ ±ÈÀı´ø »ı·ÖÏµÊı Î¢·ÖÏµÊı 
  //P_value     = (1/Proportion)*100
  //Integral	  = Proportion/I_value = (0.6*KC)/(0.5*TC)
  //Derivative  = Proportion*D_value = (0.6*KC)*(0.125*TC)  
  //3ÏÔÊ¾ÓÃµÄ3¸ö±äÁ¿µÄÖµ
  //P_value     = (1/Proportion)*100  °Ù·Ö±È
  //I_value     = 0.5*TC				Ãë 
  //D_value     = 0.125*TC			Ãë 
  //4pid¼ÆËãÓÃµÄ3¸ö±äÁ¿µÄÖµ
  //Proportion  = 0.6*KC
  //Integral	  = Proportion/I_value = (0.6*KC)/(0.5*TC)
  //Derivative  = Proportion*D_value = (0.6*KC)*(0.125*TC)  
  
  //KC = 21.4;//test 
  //TC = 471;//test 
void pid_pro(void)//pid ¿ØÖÆÊä³ö ppppppppppppppppppppppppppppp
{
  int temp_pid;
  Error = SV_value - PV_value;                 // Æ«²î
  if(( Error < max_value_error  ) && ( Error > (min_value_error)  ))//Ö»ÓĞÔÚÒ»¶¨µÄÎÂ²î·¶Î§ÄÚ²Åpid¼ÆËã
  {    
    SumError += Error;
    dError    = LastError - PrevError;   // µ±Ç°Î¢·Ö
    PrevError = LastError;
    LastError = Error;
    temp_pid  =  (int)((Proportion * Error) + (Integral * SumError) + (Derivative * dError));    
    //temp_pid  =  (int)(temp_pid * 0.5) ;//Êä³ö±ÈÀı¿ØÖÆ
  }
  else//Ö»ÓĞ¿ª¹Ø×÷ÓÃ
  {
    if( Error >= max_value_error )//Ô¶´óÓÚµ±Ç°ÎÂ¶È£¬¼ÓÈÈ
    {
      temp_pid = 100;
      //temp_pid = 80;
    }
    else if( Error <= (min_value_error) )//Ô¶Ğ¡ÓÚµ±Ç°ÎÂ¶È£¬²»¼ÓÈÈ
    {
      temp_pid = 0;
    }
  }
  if( temp_pid < 0 )
  {
    temp_pid = 0;
  }
  else if( temp_pid > 100 )
  {
    temp_pid = 100;
  } 
  Dac_Out(40 + (uint8_t)(temp_pid * 1.97) );//¸ù¾İÉÏÒ»²½µÄ½á¹û¿ØÖÆÊä³ö 
}


//PID×ÔÕû¶¨
void Pid_Sinceset(void)
{
    pid_self_time_sec++;
    if(pid_self_time_sec > (3600*3)) // Èç¹û×ÜµÄ×ÔÕû¶¨Ê±¼ä´óÓÚÁË3/5=0.6¸öĞ¡Ê±£¬ÔòËµÃ÷Õû¶¨Ê§°Ü
    {
      pid_self_time_sec = 0;			
//      pid_tune_flag = 0;//ÄÇÃ´½«×Ô¶¯ÍË³ö×ÔÕû¶¨¹ı³Ì Í¬Ê±²ÉÓÃÄ¬ÈÏÖµ  ½øÈëpid½×¶Î
      KC = 1.0;//ÁÙ½ç±ÈÀıÏµÊı  ³õÊ¼Ä¬ÈÏµÄÖµ
      TC = 40; //Õñµ´ÖÜÆÚ    ³õÊ¼Ä¬ÈÏµÄÖµ 
      memory[PIDSET_ADR]=0;     //¹Ø±ÕPID×ÔÕû¶¨
      Pid_Setok=0;   //PID×ÔÕû¶¨Ê§°Ü
    }
    if(( pid_self_first_status_flag == 1) || ( pid_self_first_status_flag == 0))//0 Éè¶¨ÎÂ¶È µÍÓÚ µ±Ç°ÎÂ¶È  //1Éè¶¨ÎÂ¶È ¸ßÓÚ »òÕß µÈÓÚ  µ±Ç°ÎÂ¶È  Æô¶¯¼ÓÈÈ
    {
      //»ù±¾on/off¿ØÖÆ
      if( SV_value >= PV_value )//Æô¶¯¼ÓÈÈ
      {
        cool_ack_counter = 0;
        hot_ack_counter++;
        if(hot_ack_counter > 3)//Á¬Ğø3´Î¶¼ÊÇÒ»ÑùµÄ½á¹û ËµÃ÷È·¶¨ SV_value >= PV_value
        {
          Dac_Out(237);//È«ËÙ¼ÓÈÈ
          if(once_add_1_flag == 0)
          {
            once_add_1_flag = 1;
            zero_across_counter++; 
            if(zero_across_counter == 3 )
            {
              TIME_LOW = pid_self_time_sec - 3;//´ËÊ±µÄÊ±¼ä²»ÊÇ×îµÍÎÂ¶È¶ÔÓ¦µÄÊ±¼ä
            }
          }
        }
      }
      else//µ±Ç°ÎÂ¶È ´óÓÚ Éè¶¨ÎÂ¶È Í£Ö¹¼ÓÈÈ
      { 
        hot_ack_counter = 0;
        cool_ack_counter++;
        if(cool_ack_counter > 3)
        {
          Dac_Out(40);//²»¼ÓÈÈ
          if(once_add_1_flag == 1)
          {
            once_add_1_flag = 0;
            zero_across_counter++;
            if(zero_across_counter == 3 )
            {
              TIME_LOW = pid_self_time_sec - 3;//´ËÊ±µÄÊ±¼ä²»ÊÇ×îµÍÎÂ¶È¶ÔÓ¦µÄÊ±¼ä
            }
          }
        }
      }
      
      //×îµÍÎÂ¶È ³öÏÖÔÚ zero_across_counter = 3 µÄ½×¶Î
      //×î¸ßÎÂ¶È ³öÏÖÔÚ zero_across_counter = 4 µÄ½×¶Î
      if((zero_across_counter == 3 ) || (zero_across_counter == 4 ))
      {				
        pid_self_calc_buffer[k_pid_self_counter] = PV_value;
        k_pid_self_counter++;
        if(k_pid_self_counter > 3)//0--3 ¹²4¸öÔªËØ
        {
          k_pid_self_counter = 0;
          enable_calc_min_max_flag = 1;
        }
        if(enable_calc_min_max_flag == 1)//Ö»ÒªÓĞ4¸öÖµ£¬¾Í¿ÉÒÔ¼ÆËãÁË ºóÃæÀ´µÄÖµ¸²¸ÇÁËÇ°ÃæµÄÖµ 
        {
          //È¥µô×îĞ¡Öµ ×î´óÖµ È¡Ê£ÏÂ2¸öÖµµÄÆ½¾ùÖµ 
          sum_temp = 0.0;  //ÏÈÇå0
          min_temp = 1024.0;
          max_temp = 0.0;
          
          for(uint8_t k_max_min = 0; k_max_min < 4; k_max_min++ )
          {						
            if(pid_self_calc_buffer[k_max_min] <= min_temp)
            {
              min_temp = pid_self_calc_buffer[k_max_min];
            }
            if(pid_self_calc_buffer[k_max_min] >= max_temp)
            {
              max_temp = pid_self_calc_buffer[k_max_min];
            }						
            sum_temp = (sum_temp + pid_self_calc_buffer[k_max_min]);
          }
          sum_temp =  sum_temp - min_temp - max_temp ;
          
          
          //pid_self_first_status_flag = 1 Ê± ×îµÍÎÂ¶È³öÏÖÔÚ3½×¶Î
          //pid_self_first_status_flag = 0 Ê± ×îµÍÎÂ¶È³öÏÖÔÚ4½×¶Î
          if(pid_self_first_status_flag == 1)
          {
            if(zero_across_counter == 3 )//×îµÍÎÂ¶È
            {
              aver_temp = (sum_temp/2.0);					
              if( aver_temp <= T_LOW )
              {
                T_LOW = aver_temp;
              }				
            }
            else if(zero_across_counter == 4 )//×î¸ßÎÂ¶È
            {
              aver_temp = (sum_temp/2.0);
              if( aver_temp >= T_Hight )
              {
                T_Hight = aver_temp;
              }
            }
          }
          else if(pid_self_first_status_flag == 0)
          {
            if(zero_across_counter == 4 )//×îµÍÎÂ¶È
            {
              aver_temp = (sum_temp/2.0);					
              if( aver_temp <= T_LOW )
              {
                T_LOW = aver_temp;
              }				
            }
            else if(zero_across_counter == 3 )//×î¸ßÎÂ¶È
            {
              aver_temp = (sum_temp/2.0);
              if( aver_temp >= T_Hight )
              {
                T_Hight = aver_temp;
              }
            }
          }
        }
      }
      else if(zero_across_counter == 5 )//4´Î¹ı0 ÔòËµÃ÷³öÏÖÁËÕñµ´ Õû¶¨³É¹¦
      {
        zero_across_counter = 0;				
        //        pid_tune_flag = 0;//½øÈëpid½×¶Î
        //pid_tune_flag = 1;//test
        TIME_Hight = pid_self_time_sec - 3;//´ËÊ±µÄÊ±¼ä²»ÊÇ×î¸ßÎÂ¶È¶ÔÓ¦µÄÊ±¼ä
        //¼ÆËã T_Hight T_LOW TIME_Hight TIME_LOW Õâ4¸öÖµ 
        //¸ù¾İÒÔÉÏ4¸öÖµ  KC Óë TC µÄÖµ±ã»á¼ÆËã³öÀ´    
        KC = 12.7/(T_Hight - T_LOW);
        KC = 5.0 * KC;//ÒòÎªÊÇ0.2sÒ»´Î ËùÒÔÀ©´ó5±¶
        TC = 1 * (TIME_Hight - TIME_LOW);//Èç¹û¼ÇÂ¼ÁË ×îµÍÎÂ¶È Óë ×î¸ßÎÂ¶È¶ÔÓ¦µÄÊ±¼ä ÄÇÃ´ÑØÓÃÕâ¸ö¹«Ê½£ºTC = 2 * (TIME_Hight - TIME_LOW);
       
        memory[PIDSET_ADR]=0;     //¹Ø±ÕPID×ÔÕû¶¨
        Pid_Setok=1;   //PID×ÔÕû¶¨³É¹¦
      }
    }
}

void PidParameter_Sinceset(void)//PID×ÔÕû¶¨²ÎÊı
{
  //¼ÇÂ¼´Ë¿ÌµÄ×´Ì¬ ¼´Éè¶¨ÎÂ¶ÈÊÇ·ñ ¸ßÓÚ»òµÈÓÚ µ±Ç°ÎÂ¶È 
  PV_value = read_max6675_temper();
  if( SV_value >= PV_value )//Éè¶¨ÎÂ¶È ¸ßÓÚ »òÕß µÈÓÚ  µ±Ç°ÎÂ¶È  Æô¶¯¼ÓÈÈ
  {
    pid_self_first_status_flag = 1;
    once_add_1_flag = 0;
  }
  else//Éè¶¨ÎÂ¶È µÍÓÚ µ±Ç°ÎÂ¶È
  {
    pid_self_first_status_flag = 0;
    once_add_1_flag = 1;
  } 
  zero_across_counter = 0;
  pid_self_time_sec = 0;      
  k_pid_self_counter = 0;
  enable_calc_min_max_flag = 0;
  max_temp = 0.0 ;  //³õÊ¼ÎÂ¶ÈµÈÓÚ0
  min_temp = 1024.0 ;//³õÊ¼ÎÂ¶ÈµÈÓÚ1024
  sum_temp = 0.0 ;  //³õÊ¼ÎÂ¶ÈµÈÓÚ0
  aver_temp = 0.0 ;
  T_Hight  = 0.0;
  T_LOW    = 1024.0; //ÎÂ¶È
  TIME_Hight = 0;
  TIME_LOW   = 0;	//¾ßÌåµÄ0.2s 
}

void PidParameter_pro(void)//PID²ÎÊı
{
  
 KC=*((float *)(&memory[PIDKC_ADR]));
 TC=*((int *)(&memory[PIDTC_ADR]));
  if(KC > 1666.0 )
  {
    KC = 1666.0;//¶ÔÓ¦ ±ÈÀı´øÎª 0.1%
  }
  else if(KC < 0.5 )
  {
    KC = 0.5;//¶ÔÓ¦ ±ÈÀı´øÎª 200.0%
  }
  if(TC > 7200 )
  {
    TC = 7200;
  }
  else if(TC < 8 )
  {
    TC = 8;
  }   
  Proportion  = 0.6*KC;
  Integral	= (0.6*KC)/(0.5*TC);
  Derivative  = (0.6*KC)*(0.125*TC); 
}


void Preheat(void)// Ô¤ÈÈ
{
  uint16_t preheat_cnt=0;
  uint8_t  preheat_out=40;
  while(preheat_cnt<240) //2·Ö
  {
    preheat_cnt++;
    if(preheat_out<237)
      preheat_out++;
    else
      preheat_out=237;
     Dac_Out(preheat_out);
    if(PV_value>=(SV_value-50))
      break;
    HAL_Delay(500);
  }
}

uint16_t PID_Algorithm(float setValue, float feedbackValue)
{
	int16_t temp_pid;
	float errorValue;
	uint16_t duty_cycle = 0;	
	errorValue = setValue - feedbackValue;                 // Æ«²î
	
  if(errorValue > MIN_ERROR_VALUE && errorValue < MAX_ERROR_VALUE )//Ö»ÓĞÔÚÒ»¶¨µÄÎÂ²î·¶Î§ÄÚ²Åpid¼ÆËã
  {    
    SumError += Error;
    dError    = LastError - PrevError;   // µ±Ç°Î¢·Ö
    PrevError = LastError;
    LastError = Error;
    temp_pid  =  (int)((Proportion * Error) + (Integral * SumError) + (Derivative * dError));    
    //temp_pid  =  (int)(temp_pid * 0.5) ;//Êä³ö±ÈÀı¿ØÖÆ
  }
  else if(errorValue >= MAX_ERROR_VALUE)//Ö»ÓĞ¿ª¹Ø×÷ÓÃ
  {
		
  }
	else
	{
		
	}
  if( temp_pid < 0 )
  {
    temp_pid = 0;
  }
  else if( temp_pid > 100 )
  {
    temp_pid = 100;
  } 
  Dac_Out(40 + (uint8_t)(temp_pid * 1.97) );//¸ù¾İÉÏÒ»²½µÄ½á¹û¿ØÖÆÊä³
	
	
	return duty_cycle;
}
