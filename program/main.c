#include <STC89C5xRC.H>
#include "tm1638.h"
#include <ds1302.h>
#include <delay.h>
#include "public.h"
#include <ssd1306.H>
#include <ds18b20.H>
#include <uart.H>

sbit recieveLED=P0^7;
sbit writeLED=P0^6;
sbit key=P3^2;
sbit IT2= XICON^0;
sbit EX2= XICON^2;
sbit EX3 = XICON^6;
sbit IT3 = XICON^4;

float T;
struct  TIME time;				//DS1302ʱ��ṹ��
uchar state;					//�������ݰ�״̬��
uchar len;						//�������ݰ�����
uchar NetTimeBuffer[15];		//�������ݰ�����
uchar valid;					//������Ч��Ч��־
char light;
uchar flag;						//�շ���־
uchar recieveFlag;				//�Ƿ���ʾ�������ѱ�־λ
uchar receiveShowFlag;

struct systime					//��������ʱ��ṹ��
{
	uchar hour;
	uchar min;
	uchar sec;
	uchar year;
	uchar mon;
	uchar date;
} netTime;

void displayRTCDate();			//�������ʾʱ��оƬ����
void displayRTCTime();			//�������ʾʱ��оƬʱ��
void OLEDShowTemp();			//OLED��ʾ�¶�
void OLEDShowNetTime();			//OLED��ʾ����ʱ��
void OLEDShowNetDate();			//OLED��ʾ��������
void OLEDShowRTCTime();			//OLED��ʾRTCʱ��
void OLEDShowRTCDate();			//OLED��ʾRTC����



void main()
{
	uint localTime;
	uint webTime;
	
	recieveLED=1;
	DS18B20_ConvertT();
	UART_Init();
	
	IT0=1;
	EX0=1;

	IT1=1;
	EX1=1;
	
	IT2=1;
	EX2=1;
	
	IT3=1;
	EX3=1;
	
	OLED_Init();
	OLED_Clear();
	TM1638_Init();
	TM1638_SetLight(0);
//	DS1302_WriteTimeSim(19,54,10);
//	DS1302_WriteDate(22,2,24);
	while(1)
	{
		uchar temp=flag;
		displayRTCTime();		//�������ʾʱ��оƬʱ��
		OLEDShowTemp();
		if(receiveShowFlag)
			OLEDShowNetTime();
		else
			OLED_ShowString(1,2,"            ",16);
		
		OLEDShowRTCTime();
		OLEDShowRTCDate();

		if(NetTimeBuffer[1]==0x0d)
		{
			OLED_ShowChar(80,4,'X',16);
			valid=0;
		}
		else if(NetTimeBuffer[1]=='2')
		{
			OLED_ShowChar(80,4,' ',16);
			valid=1;
		}

		
		localTime=60*time.min+time.sec;
		webTime=netTime.min*60+netTime.sec;
		
		//ds1302ʱ��������ʱ�����10����д�룬����Ƶ��д��
		if((valid==1)  && (localTime<=webTime-8 || localTime>=webTime+12) && (temp!=flag))
		{
			writeLED=~writeLED;
			DS1302_WriteTimeSim(netTime.hour,netTime.min,netTime.sec+2);
			writeLED=~writeLED;
		}
		
		if((valid==1)  && (time.year!=netTime.year)&& (temp!=flag) )
		{
			writeLED=~writeLED;
			DS1302_WriteDate(netTime.year,netTime.mon,netTime.date);
			writeLED=~writeLED;
		}
	}
}

void displayRTCTime()
{
	time=DS1302_GetTime();
	TM1638_ShowTime(time.hour,time.min,time.sec);
}
void displayRTCDate()
{
	time=DS1302_GetTime();
	TM1638_ShowTime(time.year,time.mon,time.date);
}
void OLEDShowNetTime()
{
	netTime.hour=(NetTimeBuffer[9]-'0')*10+(NetTimeBuffer[10]-'0');
	netTime.min=(NetTimeBuffer[11]-'0')*10+(NetTimeBuffer[12]-'0');
	netTime.sec=(NetTimeBuffer[13]-'0')*10+(NetTimeBuffer[14]-'0');
	
	netTime.year=(NetTimeBuffer[3]-'0')*10+(NetTimeBuffer[4]-'0');
	netTime.mon=(NetTimeBuffer[5]-'0')*10+(NetTimeBuffer[6]-'0');
	netTime.date=(NetTimeBuffer[7]-'0')*10+(NetTimeBuffer[8]-'0');
	
	OLED_ShowChar(1,2,'0'+netTime.hour/10,16);
	OLED_ShowChar(9,2,'0'+netTime.hour%10,16);
	OLED_ShowChar(17,2,':',16);
	OLED_ShowChar(25,2,'0'+netTime.min/10,16);
	OLED_ShowChar(33,2,'0'+netTime.min%10,16);
	OLED_ShowChar(41,2,':',16);
	OLED_ShowChar(49,2,'0'+netTime.sec/10,16);
	OLED_ShowChar(57,2,'0'+netTime.sec%10,16);
}
void OLEDShowNetDate()
{	
	netTime.year=(NetTimeBuffer[3]-'0')*10+(NetTimeBuffer[4]-'0');
	netTime.mon=(NetTimeBuffer[5]-'0')*10+(NetTimeBuffer[6]-'0');
	netTime.date=(NetTimeBuffer[7]-'0')*10+(NetTimeBuffer[8]-'0');
	
	OLED_ShowChar(1,2,'0'+netTime.year/10,16);
	OLED_ShowChar(9,2,'0'+netTime.year%10,16);
	OLED_ShowChar(17,2,':',16);
	OLED_ShowChar(25,2,'0'+netTime.mon/10,16);
	OLED_ShowChar(33,2,'0'+netTime.mon%10,16);
	OLED_ShowChar(41,2,':',16);
	OLED_ShowChar(49,2,'0'+netTime.date/10,16);
	OLED_ShowChar(57,2,'0'+netTime.date%10,16);
}
void OLEDShowRTCTime()
{
	time=DS1302_GetTime();
	
	OLED_ShowChar(1,0,'0'+time.hour/10,16);
	OLED_ShowChar(9,0,'0'+time.hour%10,16);
	OLED_ShowChar(17,0,':',16);
	OLED_ShowChar(25,0,'0'+time.min/10,16);
	OLED_ShowChar(33,0,'0'+time.min%10,16);
	OLED_ShowChar(41,0,':',16);
	OLED_ShowChar(49,0,'0'+time.sec/10,16);
	OLED_ShowChar(57,0,'0'+time.sec%10,16);		
}
void OLEDShowRTCDate()
{
	time=DS1302_GetTime();
	
	OLED_ShowChar(1,4,'0'+time.year/10,16);
	OLED_ShowChar(9,4,'0'+time.year%10,16);
	OLED_ShowChar(17,4,'/',16);
	OLED_ShowChar(25,4,'0'+time.mon/10,16);
	OLED_ShowChar(33,4,'0'+time.mon%10,16);
	OLED_ShowChar(41,4,'/',16);
	OLED_ShowChar(49,4,'0'+time.date/10,16);
	OLED_ShowChar(57,4,'0'+time.date%10,16);	
}
void OLEDShowTemp()
{
	OLED_ShowNum(9,6,T,3,16);		//��ʾ�¶���������
	OLED_ShowChar(36,6,'.',16);		//��ʾС����
	OLED_ShowNum(42,6,(unsigned long)(T*10000)%10000,4,16);//��ʾ�¶�С������
	OLED_ShowChar(78,6,'\'',16);
	OLED_ShowChar(86,6,'C',16);		
	
	DS18B20_ConvertT();	//ת���¶�
	T=DS18B20_ReadT();	//��ȡ�¶�
	if(T<0)				//����¶�С��0
	{
		OLED_ShowChar(0,6,'-',16);	//��ʾ����
		T=-T;			//���¶ȱ�Ϊ����
	}
	else				//����¶ȴ��ڵ���0
	{
		OLED_ShowChar(0,6,'+',16);	//��ʾ����
	}
}

void UART_IT() interrupt 4
{
	uchar buf;
	if(RI==1)
	{
		buf=SBUF;
		if(buf==0x40)
		{
			state=1;					//���յ���ͷ'@',��״̬1,��¼14��ʱ������0D
			len=15;
			if(recieveFlag)
				recieveLED=0;
		}
		if(state==1&&len>0)
		{
			NetTimeBuffer[15-len]=buf;	//�����ܵ������ݴ���NetTime����
			//��NetTimeBuffer[0]Ӧ����'@'
			len--;
		}
		if(buf==0x0A)
		{
			state=2;					//���ܵ���β��״̬2
			len=0;
			if(recieveFlag)
				recieveLED=1;
		}
	}
	flag++;
	
	RI=0;
}

void lightSet() interrupt 0		//������������ 
{
	//delay(20);
	//if(key==0)
	{
		light++;
		if(light>8)
			light=0;
		TM1638_SetLight(light);
	}
}

void lightSet2() interrupt 2	//������������
{
	//delay(20);
	//if(key==0)
	{
		light--;
		if(light<0)
			light=8;
		TM1638_SetLight(light);
	}
}

void recieveSet() interrupt 6	//�������Ľ�������
{
	recieveFlag=!recieveFlag;
}
void exint3() interrupt 7 		//
{
	receiveShowFlag=!receiveShowFlag;
}