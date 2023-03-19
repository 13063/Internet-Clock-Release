#include <ds1302.h>


////////////////////////////////////////////////////////
/////读取与写入均是从低往高，上升沿写入，下降沿读取/////
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
		CLK=1;			//产生一个上升沿，写入
		CLK=0;
		dat=dat>>1;		//先拉低，下一次上升沿做准备
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
		CLK=0;		//产生一个下降沿
		
	}
	return dat;
}


uchar DS1302_Read(uchar cmd)
{
	uchar dat;
	CE=0;						//初始化
	CLK=0;						//初始化
	CE=1;						//操作开始
	DS1302_WriteByte(cmd);		//写命令字
	//写命令后会收到芯片回传数据，读取
	dat=DS1302_ReadByte();
	CLK=1;
	CE=0;						//操作结束
	return dat;
}


void DS1302_Write(uchar cmd,uchar dat)
{
	CE=0;						//初始化
	CLK=0;						//初始化
	CE=1;						//操作开始
	DS1302_WriteByte(cmd);		//写命令字
	DS1302_WriteByte(dat);		//写数据
	CLK=1;
	CE=0;						//操作结束
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
	DS1302_Write(DS1302_WP,0x00);				//解除芯片写保护
	DS1302_Write(DS1302_HOUR,hour/10*16+hour%10);
	DS1302_Write(DS1302_MINUTE,min/10*16+min%10);
	DS1302_Write(DS1302_SECOND,sec/10*16+sec%10);
	DS1302_Write(DS1302_WP,0x80);
	
}
void DS1302_WriteDate(uchar year,uchar mon,uchar date)
{
	DS1302_Write(DS1302_WP,0x00);				//解除芯片写保护
	DS1302_Write(DS1302_YEAR,year/10*16+year%10);
	DS1302_Write(DS1302_MONTH,mon/10*16+mon%10);
	DS1302_Write(DS1302_DATE,date/10*16+date%10);
	DS1302_Write(DS1302_WP,0x80);
}
