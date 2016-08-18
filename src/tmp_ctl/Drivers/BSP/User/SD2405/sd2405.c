#define _RTC_C
 
#include "../main/AllModule.h"
 
/******************************************
* 函数名称：I2C_RTC_DataSetOrRead
* 功能描述：初始I2C
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
* 作者      日期               描述
* Carl.L    2009年07月29日     创建
* xxxxxx    xxxx年xx月xx日     修改xx功能
*******************************************/
void I2C_RTC_DataSetOrRead(u8* pBuffer, u8 addr, u8 NumByte,u8 operation)
{
 
if(pBuffer == NULL)
{
 
return;
 
}
 
if(operation == I2C_READ)
{
 
I2C_EE_BufferRead(pBuffer ,addr ,NumByte ,RTC_ADDRESS_READ);
 
}
else if(operation == I2C_WRITE)
{
 
//先进行写允许
u8_rtcSetBuffer = 0x80;
I2C_EE_BufferWrite(&u8_rtcSetBuffer ,0x10 ,1 ,RTC_ADDRESS_WRITE);  //置WRTC1=1 ,地址10H  
u8_rtcSetBuffer = 0x84;
I2C_EE_BufferWrite(&u8_rtcSetBuffer ,0x0F ,1 ,RTC_ADDRESS_WRITE);  //置WRTC2,WRTC3=1  ,地址0FH  
 
//写数据
I2C_EE_BufferWrite(pBuffer ,addr ,NumByte ,RTC_ADDRESS_WRITE);
 
//写禁止
u8_rtcSetBuffer = 0;
I2C_EE_BufferWrite(&u8_rtcSetBuffer ,0x0F ,1 ,RTC_ADDRESS_WRITE);  //置WRTC2,WRTC3=0   ,地址0FH  
//u8_rtcSetBuffer = 0;
I2C_EE_BufferWrite(&u8_rtcSetBuffer ,0x10 ,1 ,RTC_ADDRESS_WRITE);  //置WRTC1=0  ,地址10H  
 
}
 
 
}
 
/******************************************
* 函数名称：GetOrSetSystemTime
* 功能描述：获取或者设置System时间
* 入口参数：operation--操作类型
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
* 作者      日期               描述
* Carl.L    2009年07月29日     创建
* xxxxxx    xxxx年xx月xx日     修改xx功能
*******************************************/
void GetOrSetSystemTime(u8 operation)
{
 
u8 u8_rtcTimesBuffer[7] = {
0
};  //秒,分,时,星期,日,月,年
u32 u32_temp = 0;
u8 u8_Buf[6] = {
0
};
 
if(((operation != UNIX_TIME_SET) && (operation != UNIX_TIME_READ)))
{
 
return;
 
}
 
if(operation == UNIX_TIME_READ)
{
 
//获取实时时钟芯片时间，0x00为RTC的寄存器地址
I2C_RTC_DataSetOrRead(u8_rtcTimesBuffer,0x00 ,7 ,I2C_READ);
 
//首先把小时字节的最高位屏蔽,该位为24/12小时制选项
u8_rtcTimesBuffer[2] &= 0x7F;
 
//start of 转换成Beijing时间，秒，分，时
RtcTimeDatasConversion(u8_rtcTimesBuffer,&SystemTimeStr.BeiJing_time[2],3,RTC_FORM_TO_BEIJING);
//u8_rtcTimesBuffer[3]为星期，不需要该字节
//转换成Beijing时间，秒，分，时
RtcTimeDatasConversion(&u8_rtcTimesBuffer[4],&SystemTimeStr.BeiJing_time[5],3,RTC_FORM_TO_BEIJING);
 
SystemTimeStr.BeiJing_time[0] = 0x00;  //毫秒低8位
SystemTimeStr.BeiJing_time[1] = 0x00;  //毫秒高8位
//把年转换成16进制然后给高字节与低字节
u32_temp = SystemTimeStr.BeiJing_time[7] + 2000; //20xx年
SystemTimeStr.BeiJing_time[7] = u32_temp; //低8位
SystemTimeStr.BeiJing_time[8] = u32_temp >> 8;  //高8位
//end of 转换成Beijing时间
 
//start of 转换成unix时间
u8_Buf[0] = u32_temp - UNIX_BASE_YEAR;  //年
u8_Buf[1] = SystemTimeStr.BeiJing_time[6] - UNIX_BASE_MONTH;  //月
u8_Buf[2] = SystemTimeStr.BeiJing_time[5] - UNIX_BASE_DATE;  //日
u8_Buf[3] = SystemTimeStr.BeiJing_time[4] - UNIX_BASE_HOUR;  //时
u8_Buf[4] = SystemTimeStr.BeiJing_time[3] - UNIX_BASE_MIN;  //分
u8_Buf[5] = SystemTimeStr.BeiJing_time[2];  //秒
 
u32_temp = UnixTimeConversion(u8_Buf[0],u8_Buf[1],u8_Buf[2],u8_Buf[3],u8_Buf[4],u8_Buf[5]); 
SystemTimeStr.Unix_time[0] = u32_temp;  //0-8bit
SystemTimeStr.Unix_time[1] = u32_temp >> 8;  //8-16bit
SystemTimeStr.Unix_time[2] = u32_temp >> 16;  //16-24bit
SystemTimeStr.Unix_time[3] = u32_temp >> 24;  //24-32bit
//end of 转换成unix时间
 
}
else   //设置时进来的一定要是北京时间的格式!!!
{
 
//转换成TRC芯片所需要的格式
//转换年格式
RtcTimeDatasConversion(&SystemTimeStr.BeiJing_time[2],u8_rtcTimesBuffer,3,BEIJING_FORM_TO_RTC);
u8_rtcTimesBuffer[2] |= 0x80;  //使能24小时进制
 
//获取星期数
I2C_RTC_DataSetOrRead(&u8_rtcTimesBuffer[3],0x03 ,1 ,I2C_READ);
RtcTimeDatasConversion(&SystemTimeStr.BeiJing_time[4],&u8_rtcTimesBuffer[4],2,BEIJING_FORM_TO_RTC);
u32_temp = (SystemTimeStr.BeiJing_time[8] << 8) + SystemTimeStr.BeiJing_time[7];
if(u32_temp > 2000)
{
 
u32_temp -= 2000;
RtcTimeDatasConversion((u8 *)u32_temp,&u8_rtcTimesBuffer[6],1,BEIJING_FORM_TO_RTC);
 
}
 
I2C_RTC_DataSetOrRead(u8_rtcTimesBuffer,0x00 ,7 ,I2C_WRITE);
 
 
}
 
 
}
 
/******************************************
* 函数名称：RtcTimeDatasConversion
* 功能描述：Rtc时钟芯片数据格式转换
* 入口参数：pBuffer--数据首地址
*           NumByte--转换数据的长度
*           operation--操作类型
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
* 作者      日期               描述
* Carl.L    2009年07月29日     创建
* xxxxxx    xxxx年xx月xx日     修改xx功能
*******************************************/
void RtcTimeDatasConversion(u8* pBufIn, u8* pBufferOut, u8 NumByte, u8 operation)
{
 
u8 i = 0;
u8 high_4bit = 0;
u8 Low_4bit = 0;
 
if((pBufIn == NULL) || (pBufferOut == NULL))
{
 
return;
 
}
 
if((operation != RTC_FORM_TO_BEIJING) && (operation != BEIJING_FORM_TO_RTC))
{
 
return;
 
}
 
if(operation == RTC_FORM_TO_BEIJING)
{
 
for(i = 0;i < NumByte;i++)
{
 
high_4bit = (*pBufIn & 0xF0) >> 4;
Low_4bit = *pBufIn & 0x0F;
 
*pBufferOut = (high_4bit * 10) + Low_4bit;
 
pBufIn++;
pBufferOut++;
 
}
 
}
else
{
 
for(i = 0;i < NumByte;i++)
{
 
high_4bit = (*pBufIn / 10) << 4;
Low_4bit = *pBufIn & 0x0F;
 
*pBufferOut = high_4bit + Low_4bit;
 
pBufIn++;
pBufferOut++;
 
}
 
 
}
 
}