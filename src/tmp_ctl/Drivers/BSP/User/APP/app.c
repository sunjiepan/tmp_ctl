/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "includes.h"
extern struct Data_Time timer;  //定义一个时间结构体变量

int ExTick;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void AppTaskCreate(void);
static void AppTaskStart(void *p_arg);

static void AppTaskUserIF(void *p_arg);
static void AppTaskKbd(void *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

void MainTask(void);

void LED_Init(void)
{
      GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	  GPIO_Init(GPIOG, &GPIO_InitStructure);
}

int  main (void)
{
    INT8U  err;
	/* Set the Vector Table base location at 0x08000000 */ 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
    OSInit();                          /* Initialize "uC/OS-II, The Real-Time Kernel"              */
    /* Create the start task */
    OSTaskCreateExt(AppTaskStart,(void *)0,(OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE-1],APP_TASK_START_PRIO,APP_TASK_START_PRIO,(OS_STK *)&AppTaskStartStk[0],APP_TASK_START_STK_SIZE,(void *)0,OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);

    #if (OS_TASK_NAME_SIZE > 13)
    OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);
    #endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
  (void)p_arg;

   BSP_Init();                                                 /* Initialize BSP functions                                 */
   GUI_Init();
   #if (OS_TASK_STAT_EN > 0)
   OSStatInit();                                               /* Determine CPU capacity                                   */
   #endif
   AppTaskCreate();                                            /* Create application tasks                                 */
   LED_Init();
   while(DEF_TRUE)
   {  
	     /* Task body, always written as an infinite loop. */
	     OSTaskSuspend(OS_PRIO_SELF);
   }
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate(void)
{
   INT8U  err;

   OSTaskCreateExt(AppTaskUserIF,(void *)0,(OS_STK *)&AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE-1],APP_TASK_USER_IF_PRIO,APP_TASK_USER_IF_PRIO,(OS_STK *)&AppTaskUserIFStk[0],
                    APP_TASK_USER_IF_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);


	#if (OS_TASK_NAME_SIZE > 8)
	  OSTaskNameSet(APP_TASK_USER_IF_PRIO, "User I/F", &err);
	#endif
	
	
	  OSTaskCreateExt(AppTaskKbd,(void *)0,(OS_STK *)&AppTaskKbdStk[APP_TASK_KBD_STK_SIZE-1],APP_TASK_KBD_PRIO,APP_TASK_KBD_PRIO,(OS_STK *)&AppTaskKbdStk[0],
	                    APP_TASK_KBD_STK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	
	#if (OS_TASK_NAME_SIZE > 8)
	  OSTaskNameSet(APP_TASK_KBD_PRIO, "Keyboard", &err);
	#endif
}


/*
*********************************************************************************************************
*                                         USER INTERFACE TASK
*
* Description : This task updates the LCD screen based on messages passed to it by AppTaskKbd().
*
* Arguments   : p_arg   is the argument passed to 'AppStartUserIF()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

static  void  AppTaskUserIF (void *p_arg)
{
  (void)p_arg;
  OSTimeDlyHMSM(0,0,0,200);
  OSTimeDlyHMSM(0,0,0,200);
  GUI_DispStringAt("RTC Init", 80, 30);
  Init_RTC();						//时钟初始化
  //Time_Update(2012,8,8,8,8,8);	//设置时钟，年月日 时分秒
  GUI_DispStringAt("RTC Test Code", 80, 50);
  GUI_DispStringAt("More CODE Please See:  http://boqiankeji.tmall.com", 20, 220);
  while(DEF_TRUE) 
    {
      Time_Get(); 	//获取时钟
	  GUI_DispDecAt(timer.w_year,80,80,4);
	  GUI_DispString(".");
	  GUI_DispDec(timer.w_month,2);
      GUI_DispString(".");
      GUI_DispDec(timer.w_date,2);  	  

	  GUI_DispDecAt(timer.hour,150,80,2);
      GUI_DispString(":");
      GUI_DispDec(timer.min,2);
      GUI_DispString(":");
      GUI_DispDec(timer.sec,2);
	  GUI_DispString("  "); 
	  OSTimeDlyHMSM(0,0,0,200);
    }
}


/*
*********************************************************************************************************
*                                    KEYBOARD RESPONSE TASK
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Arguments   : p_arg   is the argument passed to 'AppStartKbd()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

static  void  AppTaskKbd (void *p_arg)
{
   u8 tick=0;
  (void)p_arg;
   while(DEF_TRUE) 
     {
	 	  tick++;
          OSTimeDlyHMSM(0,0,0,10); 
	      GUI_TOUCH_Exec(); 
		  if(tick&0x10)
		  {
		    GPIO_SetBits(GPIOG,GPIO_Pin_14);
		  }
		  else
		  {
		 	GPIO_ResetBits(GPIOG,GPIO_Pin_14);
		  }
     }
}
