#ifndef _IIC_H
#define _IIC_H

#include "public.h"
sbit SCL=P2^7;
sbit SDA=P2^6;


void iicStart();
void iicStop();
void iicSendData(uchar dat);
void iicWaitAck();
#endif