#ifndef TM1638_H
#define TM1638_H
sbit STB=P2^3;
sbit CLK=P2^4;
sbit DIO=P2^5;

typedef unsigned char uchar;

void TM1638_ShowNum(uchar pos,uchar num);				//ָ��λ��(1~8)д����
void TM1638_ShowTime(uchar hour,uchar min,uchar sec);	
void TM1638_Init();
void TM1638_SetLight(uchar light);						//0~7������
void TM1638_ShowNumber(unsigned long number);
#endif