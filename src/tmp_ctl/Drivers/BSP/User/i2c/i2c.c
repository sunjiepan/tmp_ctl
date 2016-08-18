
#include "i2c.h"
#include "SysTick.h"
/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */

S_Time  S_RTC={0x55,0x59,0x14,0x01,0x12,0x11,0x14};//初始化时间结构体变量（设置时间：2014年11月12日 14:59:55  星期一）
//           55秒 59分 14时 周一 10日 11月 14年

 void SD2405_GPIO_Config(void)
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

static void SD2405_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*选择要控制的DS18B20_PORT引脚*/	
	  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;;

	   /*设置引脚模式为浮空输入模式*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	

	  /*调用库函数，初始化DS18B20_PORT*/
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


static void SD2405_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*选择要控制的DS18B20_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化DS18B20_PORT*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/*********I2C延时4us***********/
void I2CWait(void)//4us
{	
	Delay_us(4);
}

/********开启SD24XX的I2C总线********/
uchar I2CStart(void)
{
	SDA_H;
	SCL_H;
	I2CWait();
	SD2405_Mode_IPU();
	if(!SDA_IN)return false;	//SDA线为低电平则总线忙,退出
	SD2405_Mode_Out_PP();
	SDA_L;
	I2CWait();
	SD2405_Mode_IPU();
	while(!SDA_IN)return false;	//SDA线为高电平则总线出错,退出
	SCL_L;
	I2CWait();
	return true;
}

/********关闭SD24XX的I2C总线*******/
void I2CStop(void)
{
	SD2405_Mode_Out_PP();
	SDA_L;
	SCL_L;
	I2CWait();
	SCL_H;
	I2CWait();
	SDA_H;
}

/*********发送 ACK*********/
void I2CAck(void)
{	
	SD2405_Mode_Out_PP();
	SDA_L;
	SCL_L;
	I2CWait();
	SCL_H;
	I2CWait();
	SCL_L;
}

/*********发送NO ACK*********/
void I2CNoAck(void)
{	
	SD2405_Mode_Out_PP();
	SDA_H;
	SCL_L;
	I2CWait();
	SCL_H;
	I2CWait();
	SCL_L;
}

/*********读取ACK信号*********/
uchar I2CWaitAck(void) 	 //返回为:1=有ACK,0=无ACK
{
	SD2405_Mode_Out_PP();
	SCL_L;
	SDA_H;		//设置SDA为输入（其它类型的单片机需要配置IO输入输出寄存器）
	I2CWait();
	SCL_H;
	I2CWait();
	SD2405_Mode_IPU();
	while(SDA_IN)
	{
		SCL_L;
		return false;
	}
	SCL_L;
	return true;
}

/************MCU向SD24XX发送一个字节*************/
void I2CSendByte(uchar demand) //数据从高位到低位//
{
	uchar i=8;                       
	                                 
	
	while(i--)
	{
		SD2405_Mode_Out_PP();
		SCL_L;
		Delay_us(1);
		if(demand&0x80)	{SDA_H;}	else {SDA_L;}
		demand<<=1;
		I2CWait();
		SCL_H;
		I2CWait();
	}
	SCL_L;
}

/*********MCU从SD24XX读入一字节*********/
uchar I2CReceiveByte(void)      //数据从高位到低位//
{
	uchar i=8;
	uchar ddata=0;
	SDA_H;			//设置SDA为输入（其它类型的单片机需要配置IO输入输出寄存器）
	while(i--)
	{
		ddata<<=1;      //数据从高位开始读取
		SCL_L;
		I2CWait();
		SCL_H;
		I2CWait();	//从高位开始 ddata|=SDA;ddata<<=1
		if(SDA_IN)
		{
			ddata|=0x01;
		}
	}
	SCL_L;
	return ddata;
}

/******I2C写一个字节******/
uchar I2CWriteOneByte(uchar DeviceAddress,uchar add, uchar date)
{		
	if(!I2CStart())return false;
	I2CSendByte(DeviceAddress);      
	I2CWaitAck();   
	I2CSendByte(add);		//设置写地址      
	I2CWaitAck();	
	I2CSendByte(date);		//写数据
	I2CWaitAck();	
	I2CStop(); 
	return	true;
}

/******I2C读一个字节程序******/
uchar I2CReadOneByte(uchar DeviceAddress,uchar add)
{		
	uchar dat;
	if(!I2CStart())return false;
	I2CSendByte(DeviceAddress);      
	if(!I2CWaitAck()){I2CStop(); return false;}
  	I2CSendByte(add);		//设置要读的地址
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(DeviceAddress+1);	
	I2CWaitAck();	
	dat=I2CReceiveByte();		//读数据
	I2CNoAck();
	I2CStop(); 
	return  dat;
}

/******写SD24XX允许程序******/
uchar WriteTimeOn(void)
{		
	if(!I2CWriteOneByte(RTC_Address,0x10,0x80))return false;
	I2CWriteOneByte(RTC_Address,0x0f,0x84);
	return	true;
}

/******写SD24XX禁止程序******/
uchar WriteTimeOff(void)
{		
	if(!I2CWriteOneByte(RTC_Address,0x0f,0))return false;
	I2CWriteOneByte(RTC_Address,0x10,0);
	return	true;
}

/******读SD24XX实时数据寄存器******/
uchar I2CReadDate(S_Time	*psRTC)
{
	
	if(!I2CStart())return false;
	I2CSendByte(RTC_Address+1); 
    if(!I2CWaitAck()){I2CStop(); return false;}
	psRTC->second=I2CReceiveByte();
	I2CAck();
	psRTC->minute=I2CReceiveByte();
	I2CAck();
	psRTC->hour=I2CReceiveByte();
	I2CAck();
	psRTC->week=I2CReceiveByte();
	I2CAck();
	psRTC->day=I2CReceiveByte();
	I2CAck();
	psRTC->month=I2CReceiveByte();
	I2CAck();
	psRTC->year=I2CReceiveByte();
	I2CNoAck();		//读时间完成，发送NoAck
	I2CStop();
	return	true;
}

/******写SD24XX实时数据寄存器******/
uchar I2CWriteDate(S_Time	SetRTC)	//写时间操作要求一次对实时时间寄存器(00H~06H)依次写入，
{                               //不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位. 
                                //要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据.
	S_Time 	*psRTC;
    psRTC=&SetRTC;
	WriteTimeOn();				//使能，开锁
	if(!I2CStart())return false;
	I2CSendByte(RTC_Address); 
	if(!I2CWaitAck()){I2CStop(); return false;}
	I2CSendByte(0x00);			//设置写起始地址      
	I2CWaitAck();	
	I2CSendByte(psRTC->second);		//second     
	I2CWaitAck();	
	I2CSendByte(psRTC->minute);		//minute      
	I2CWaitAck();	
	I2CSendByte(psRTC->hour|0x80);		//hour ,同时设置小时寄存器最高位（0：为12小时制，1：为24小时制）
	I2CWaitAck();	
	I2CSendByte(psRTC->week);		//week      
	I2CWaitAck();	
	I2CSendByte(psRTC->day);		//day      
	I2CWaitAck();	
	I2CSendByte(psRTC->month);		//month      
	I2CWaitAck();	
	I2CSendByte(psRTC->year);		//year      
	I2CWaitAck();	
	I2CStop();
	
	WriteTimeOff();				//使能，关锁
	return	true;
}


/******设置SD24XX报警中断演示程序演示******/
void WriteALARM(void)				//设置报警时间：2015年2月14日 8：00
{						//只有设置未来的时间才有效
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x09,0x08);	//8时
	I2CWriteOneByte(RTC_Address,0x0b,0x14);	//14日
	I2CWriteOneByte(RTC_Address,0x0c,0x02);	//02月
	I2CWriteOneByte(RTC_Address,0x0d,0x15);	//15年
	I2CWriteOneByte(RTC_Address,0x0e,0x74);	//设置报警允许（使能年、月、日、小时报警）
	I2CWriteOneByte(RTC_Address,0x10,0x92);	//设置INT中断选通（INTS1，INTS0），及报警中断总允许位（INTAE）
	WriteTimeOff();
}

/******关闭SD24XX报警中断程序******/
void ClrALARM(void)							//关闭报警中断
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0x90);
	WriteTimeOff();
}
/******设置SD24XX倒计时中断演示******/
void SetDjs(void)							//设置倒计时中断
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0x0f);//先清倒计时中断总允许位（INTDE）
	I2CWriteOneByte(RTC_Address,0x10,0xf4);//设置周期性中断（IM=1）INT中断选通（INTS1，INTS0），配置倒计时中断总允许位（INTDE）
	I2CWriteOneByte(RTC_Address,0x11,0x30);//选择定时器频率源（TDS1、TDS0）为1/60HZ
	I2CWriteOneByte(RTC_Address,0x13,0x05);//倒计时初值寄存器，设置8位倒计时计数初值（5min）
	WriteTimeOff();
}

/******关闭SD24XX倒计时中断程序******/
void ClrDjs(void)          
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0xf0);
	WriteTimeOff();
}
/******设置SD24XX频率中断演示******/
void SetFrq(void)					
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0xa1);	//选通频率中断（INTS1，INTS0），设置频率中断总允许位（INTFE）
	I2CWriteOneByte(RTC_Address,0x11,0x09);	//设置2Hz频率中断
	WriteTimeOff();
}
/******关闭SD24XX频率中断******/
void ClrFrq(void)         
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0xa0);
	WriteTimeOff();
}

//特别提醒：当写实时时间数据时 (00H~06H), 不可以单独 对 7 个时间数据中的某一位进行写操作 ,
//否则可能会引起时间数据的错误进位 , 所以要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据 .
/*********************************************END OF FILE**********************/
