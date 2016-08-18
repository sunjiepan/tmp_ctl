#ifndef _RTC_H
#define _RTC_H
 
#ifdef  _RTC_C
#define RTC_C_EXTERN
#else
#define RTC_C_EXTERN extern
#endif
 
#define RTC_ADDRESS_READ  0x65   //rtc器件地址+读操作
#define RTC_ADDRESS_WRITE 0x64   //rtc器件地址+写操作
 
#define UNIX_TIME_SET  0x00  //
#define UNIX_TIME_READ 0x01  //
 
#define RTC_FORM_TO_BEIJING 0x00  //RTC时间转换成北京时间格式
#define BEIJING_FORM_TO_RTC 0x01  //与上面相反
 
#define ONE_SECOND_INT_VALUE 100  //定时器3计数100次为一秒
 
//Unix epoch Unix时间戳,单位秒
#define UNIX_EPOCH_MIN   60  //一分钟
#define UNIX_EPOCH_HOUR  3600   //一小时
#define UNIX_EPOCH_DATE  86400   //一天
#define UNIX_EPOCH_WEEK  604800   //一周
#define UNIX_EPOCH_MONTH 2629743   //一月，30.44天
#define UNIX_EPOCH_YEAR  31556926   //一年，365.24天
 
//unix时基
#define UNIX_BASE_YEAR  1970
#define UNIX_BASE_MONTH 1
#define UNIX_BASE_DATE  1
#define UNIX_BASE_HOUR  0
#define UNIX_BASE_MIN   0
#define UNIX_BASE_SECON 0
 
 
#define UnixTimeConversion(YEAR,MONTH,DATE,HOUR,MIN,SECOND) \
                                  (((YEAR) * UNIX_EPOCH_YEAR) \
                                     + ((MONTH) * UNIX_EPOCH_MONTH) \
                                     + ((DATE) * UNIX_EPOCH_DATE) \
                                     + ((HOUR) * UNIX_EPOCH_HOUR) \
                                     + ((MIN) * UNIX_EPOCH_MIN) \
                                     + (SECOND))
 
RTC_C_EXTERN u8 u8_rtcSetBuffer;
RTC_C_EXTERN u32 OneSecondCount;
RTC_C_EXTERN u32 OneSecondFlag;
 
RTC_C_EXTERN void I2C_RTC_DataSetOrRead(u8* pBuffer, u8 addr, u8 NumByte,u8 operation);
RTC_C_EXTERN void GetOrSetSystemTime(u8 operation);
RTC_C_EXTERN void RtcTimeDatasConversion(u8* pBufIn, u8* pBufferOut, u8 NumByte, u8 operation);
 
 
typedef struct SystemTime
{
 
u8 RealTimeChip_time[7];  //实时时钟芯片时间
 
u8 BeiJing_time[9];  //yyyymmddhhmmsszzzz，精确到毫秒
 
u8 Unix_time[4];  //unix时间
     
 
}SYSTEM_TIME_STRUCT;
RTC_C_EXTERN SYSTEM_TIME_STRUCT SystemTimeStr;
 
 
 
#endif