#include"absacc.h"
void delay(unsigned int t)
{
	while(t--);
}

void display(unsigned char pos,unsigned char dat)
{
	XBYTE[0xe000]=0xff;
	XBYTE[0xc000]=0x01<<pos;
	XBYTE[0xe000]=dat;
}

void displayall(unsigned char dat)
{
	XBYTE[0xc000]=0xff;
	XBYTE[0xe000]=dat;
}