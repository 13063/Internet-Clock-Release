#include <ds1302.h>


////////////////////////////////////////////////////////
/////��ȡ��д����Ǵӵ����ߣ�������д�룬�½��ض�ȡ/////
////////////////////////////////////////////////////////

sbit CLK=P2^0;
sbit SDA=P2^1;
sbit CE=P2^2;





void DS1302_WriteByte(uchar dat)
{
	uchar i;
	CLK=0;
	for(i=0;i<8;i++)
	{
		SDA=dat&0x01;
		CLK=1;			//����һ�������أ�д��
		CLK=0;
		dat=dat>>1;		//�����ͣ���һ����������׼��
	}
}

uchar DS1302_ReadByte()
{
	uchar dat,i;
	for(i=0;i<8;i++)
	{
		dat=dat>>1;
		if(SDA==1)
			dat=dat|0x80;
		CLK=1;
		CLK=0;		//����һ���½���
		
	}
	return dat;
}


uchar DS1302_Read(uchar cmd)
{
	uchar dat;
	CE=0;						//��ʼ��
	CLK=0;						//��ʼ��
	CE=1;						//������ʼ
	DS1302_WriteByte(cmd);		//д������
	//д�������յ�оƬ�ش����ݣ���ȡ
	dat=DS1302_ReadByte();
	CLK=1;
	CE=0;						//��������
	return dat;
}


void DS1302_Write(uchar cmd,uchar dat)
{
	CE=0;						//��ʼ��
	CLK=0;						//��ʼ��
	CE=1;						//������ʼ
	DS1302_WriteByte(cmd);		//д������
	DS1302_WriteByte(dat);		//д����
	CLK=1;
	CE=0;						//��������
}

struct TIME DS1302_GetTime()
{
	uchar temp;
	struct TIME tim;
	
	temp=DS1302_Read(DS1302_HOUR+1);
	tim.hour=temp/16*10+temp%16;
	temp=DS1302_Read(DS1302_MINUTE+1);
	tim.min=temp/16*10+temp%16;
	temp=DS1302_Read(DS1302_SECOND+1);
	tim.sec=temp/16*10+temp%16;
	temp=DS1302_Read(DS1302_YEAR+1);
	tim.year=temp/16*10+temp%16;
	temp=DS1302_Read(DS1302_DATE+1);
	tim.date=temp/16*10+temp%16;
	temp=DS1302_Read(DS1302_DAY+1);
	tim.day=temp/16*10+temp%16;
	temp=DS1302_Read(DS1302_MONTH+1);
	tim.mon=temp/16*10+temp%16;
	return tim;
}



void DS1302_WriteTimeSim(uchar hour,uchar min,uchar sec)
{
	DS1302_Write(DS1302_WP,0x00);				//���оƬд����
	DS1302_Write(DS1302_HOUR,hour/10*16+hour%10);
	DS1302_Write(DS1302_MINUTE,min/10*16+min%10);
	DS1302_Write(DS1302_SECOND,sec/10*16+sec%10);
	DS1302_Write(DS1302_WP,0x80);
	
}
void DS1302_WriteDate(uchar year,uchar mon,uchar date)
{
	DS1302_Write(DS1302_WP,0x00);				//���оƬд����
	DS1302_Write(DS1302_YEAR,year/10*16+year%10);
	DS1302_Write(DS1302_MONTH,mon/10*16+mon%10);
	DS1302_Write(DS1302_DATE,date/10*16+date%10);
	DS1302_Write(DS1302_WP,0x80);
}
