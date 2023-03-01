#ifndef _DS1302_H
#define _DS1302_h

#include "public.h"

struct TIME
{
	uchar sec;
	uchar min;
	uchar hour;
	uchar date;
	uchar mon;
	uchar day;
	uchar year;
};

#define DS1302_SECOND  0x80
#define DS1302_MINUTE  0x82
#define DS1302_HOUR    0x84
#define DS1302_DATE    0x86
#define DS1302_MONTH   0x88
#define DS1302_DAY     0x8A
#define DS1302_YEAR    0x8C
#define DS1302_WP      0x8E
void DS1302_Write(uchar cmd,uchar dat);
uchar DS1302_Read(uchar cmd);
struct TIME DS1302_GetTime();
void DS1302_WriteTimeSim(uchar hour,uchar min,uchar sec);
void DS1302_WriteDate(uchar year,uchar mon,uchar date);


#endif