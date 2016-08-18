#include "TM1637.h"
#include "delay.h"
#include "usart.h"
#include "Buzzer.h" 
/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 */

static unsigned char temp;
static unsigned char x=1;
//unsigned char channel_2;
unsigned char Enable_2;
extern unsigned char channel;
extern unsigned char NumDis[24],TIME[10],G_2,K1;
extern int16_t cache[6],data[6];
void TM1637_Start_2(void)
{
	TM1637_DIO_OUT(); 
  TM1637_CLK (1);
	TM1637_OUT_DIO2 = 1;
	TM1637_Delay_us(2);
	TM1637_OUT_DIO2 = 0;
	TM1637_Delay_us(2);
	TM1637_CLK ( 0);
}
void TM1637_Ack_2(void)
{
	TM1637_DIO_IN(); 
  TM1637_CLK ( 0);
	TM1637_Delay_us(5);
	while(TM1637_READ_DIO2);
	TM1637_CLK (1);
	TM1637_Delay_us(2);
}
void TM1637_Stop_2(void)
{
	TM1637_DIO_OUT(); 
	TM1637_CLK ( 0);
	TM1637_Delay_us(2);
	TM1637_OUT_DIO2 = 0;
	TM1637_Delay_us(2);
	TM1637_CLK (1);
	TM1637_Delay_us(2);
	TM1637_OUT_DIO2 = 1;
	TM1637_Delay_us(2);
	TM1637_CLK( 0);
	TM1637_OUT_DIO2 = 0;
}
void TM1637_WriteByte_2(unsigned char oneByte)
{
 unsigned char i;
	TM1637_DIO_OUT(); 
	for(i=0;i<8;i++)
	{
	 TM1637_CLK(0);
		if(oneByte&0x01)
		{
			TM1637_OUT_DIO2 = 1;
		}
		else
		{
			TM1637_OUT_DIO2 = 0;
		}
		TM1637_Delay_us(3);
		TM1637_CLK(1);
		oneByte=oneByte>>1;
	}
}

void TM1637_DisplayChar_2(unsigned char ch,unsigned char p)
{
	if(ch>23)ch=0;//防止数组越界
	TM1637_Start_2();
	TM1637_WriteByte_2(0x44);//0x44固定模式有利于控制显示位，0x40则采用显示地址自加模式这里不使用
	TM1637_Ack_2();
	TM1637_Stop_2();
	TM1637_Start_2();
	
	TM1637_WriteByte_2(0xC0+p);//0X00地址开始显示
	TM1637_Ack_2();
		
	TM1637_WriteByte_2(NumDis[ch]);//显示
	TM1637_Ack_2();
	
	TM1637_Stop_2();
	TM1637_Start_2();
	TM1637_WriteByte_2(0x8C);
	TM1637_Ack_2();
	TM1637_Stop_2();
		
}
void TM1637_Display_2(void)
{
	unsigned char a=0,b=0,c=0,d=0;
	if(cache[1]<0)
	{
		d=((-cache[1])%60)%10;
		c=((-cache[1])%60)/10;
		b=((-cache[1])/60)%10;
		a=0x40;
	}
	else
	{
		d=(cache[1]%60)%10;
		c=(cache[1]%60)/10;
		b=(cache[1]/60)%10;
		a=(cache[1]/60)/10;
	}
	TM1637_Start_2();
	TM1637_WriteByte_2(0x40);//0x40则采用显示地址自加1模式
	TM1637_Ack_2();
	TM1637_Stop_2();
	TM1637_Start_2();
	TM1637_WriteByte_2(0xC0);//0X00地址开始显示
	TM1637_Ack_2();
		
	TM1637_WriteByte_2(TIME[a]);//显示1
	TM1637_Ack_2();
	if(x==1)
	{
		temp=TIME[b];
		x++;
	} 
  temp^=0x80;  //10000000^11001111=01001111;01001111^10000000=11001111
	TM1637_WriteByte_2(temp);//显示2和跳动的冒号
	TM1637_Ack_2();
	
	TM1637_WriteByte_2(TIME[c]);//显示0
	TM1637_Ack_2();
		
	TM1637_WriteByte_2(TIME[d]);//显示0
	TM1637_Ack_2();
		
	TM1637_WriteByte_2(0xFF);
	TM1637_Ack_2();
		
	TM1637_WriteByte_2(0xFF);
	TM1637_Ack_2();
	
	TM1637_Stop_2();
	TM1637_Start_2();
	TM1637_WriteByte_2(0x8C);
	TM1637_Ack_2();
	TM1637_Stop_2();
}

void time_2(void)
{
	if(x>=2)
			{
				if((Enable_2==1)&&(channel==0))
				 {		
						G_2=1;
					  cache[1]--;
						if(cache[1]<0)
						{
							if((-cache[1])>599)
							{
								Enable_2=0;
							}
							K1=1;
						}	
					}
				x=1;
			}
}
void Digital_off_2(void)
{
	TM1637_Start_2();
	TM1637_WriteByte_2(0x88);
	TM1637_Ack_2();
	TM1637_Stop_2();
	
}
