#include"reg52.h"
#include"onewire.h"
#include"tapecontrol.h"
#include"absacc.h"
sbit L1=P0^0;
sbit S4=P3^3;
sbit S5=P3^2;
sbit S6=P3^1;
sbit S7=P3^0;

const unsigned char num[18] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0xff};
unsigned char t_hour=23,t_min=59,t_sec=50;
unsigned char c_hour=0,c_min=0,c_sec=0;
//----------------------------

unsigned int temp;
void showtemp()
{
	 display(7,num[12]);
	delay(500);
	 display(6,nodot[(temp%100)/10]);
	delay(500);
	 display(5,nodot[temp/100]);
	delay(500);
	
	 display(4,0xff);
	delay(500);
 display(3,0xff);
	delay(500);
 display(2,0xff);
	delay(500);
 display(1,0xff);
	delay(500);
 display(0,0xff);
	delay(500);
	
	displayall(0xff);
	delay(500);
}

void delaytemp(unsigned int t)
{
	while(t--)
	{
		//showtemp();
	}
}
void read_temp()
{
	unsigned char LSB,MSB;
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	delaytemp(100);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);//read command
	
	LSB=Read_DS18B20();
	MSB=Read_DS18B20();
	
	temp=MSB;
	temp<<=8;
	temp=temp|LSB;
	
	if((temp&0xf800)==0x0000)
	{
		temp>>=4;
		temp=temp*10;
		temp=temp+(LSB&0x0f)*0.625;
	}
}
//-----------------------------
void inittime0()
//need two timer 
//one in actual timing,one in flash of h,m,s
{
	TMOD=0x11;
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	
	TH1=(65535-50000)/256;
	TL1=(65535-50000)%256;
	ET1=1;
	TR1=1;
	ET0=1;
	TR0=1;
	EA=1;
}
unsigned char mode_set=0,time_set=0,clock_set=0;
unsigned char led_flash=0,led_end=0;//for led
unsigned char t_count0=0,t_count1=0,c_count0=0,led_count=0;//ordinary
unsigned char t_flag=0;//in time,decide flash
void servicetime0()interrupt 1
{
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	
	t_count0++;
	c_count0++;
	led_count++;
	
	
	if(led_count==4)//0.2s flash idk how end
	{
		led_flash=~led_flash;
		led_count=0;
	}
	
	if(t_count0==20)//1s
	{
		t_count0=0;
		t_sec++;
		if(t_sec==60)
		{
			t_sec=0;
			t_min++;
			if(t_min==60)
			{
				t_min=0;
				t_hour++;
				if(t_hour==24)
					t_hour=0;
			}
		}
	}
	
}

void servicetime1()interrupt 3
{
	TH1=(65535-50000)/256;
	TL1=(65535-50000)%256;
	t_count1++;
	if(t_count1==20)
	{
		t_count1=0;
		t_flag=~t_flag;
	}
	
}




//------------------------------
void showtime(unsigned char hour,unsigned char min,unsigned char sec)
{
	display(0,nodot[hour/10]);
	delay(200);
	display(1,nodot[hour%10]);
	delay(200);
	display(2,num[16]);
	delay(200);
	
	display(3,nodot[min/10]);
	delay(200);
	display(4,nodot[min%10]);
	delay(200);
	display(5,num[16]);
	delay(200);
	
	display(6,nodot[sec/10]);
	delay(200);
	display(7,nodot[sec%10]);
	delay(200);
	
	displayall(0xff);
	delay(500);
}


void hour_flash(unsigned char hour,unsigned char min,unsigned char sec)
{
	
	display(2,num[16]);
	delay(200);
	display(3,nodot[min/10]);
	delay(200);
	display(4,nodot[min%10]);
	delay(200);
	display(5,num[16]);
	delay(200);
	display(6,nodot[sec/10]);
	delay(200);
	display(7,nodot[sec%10]);
	delay(200);
	
	if(t_flag==0)
	{
		display(0,nodot[hour/10]);
		delay(200);
		display(1,nodot[hour%10]);
		delay(200);
	}
	else
	{
		display(0,num[17]);
		delay(200);
		display(1,num[17]);
		delay(200);
	}
	displayall(0xff);
	delay(200);
}

void min_flash(unsigned char hour,unsigned char min,unsigned char sec)
{
	display(0,nodot[hour/10]);
	delay(200);
	display(1,nodot[hour%10]);
	delay(200);
	display(2,num[16]);
	delay(200);
	display(5,num[16]);
	delay(200);
	display(6,nodot[sec/10]);
	delay(200);
	display(7,nodot[sec%10]);
	delay(200);
	
	if(t_flag==0)
	{
		display(3,nodot[min/10]);
		delay(200);
		display(4,nodot[min%10]);
		delay(200);
	}
	else
	{
		display(3,num[17]);
		delay(200);
		display(4,num[17]);
		delay(200);
	}
	displayall(0xff);
	delay(500);
}

void sec_flash(unsigned char hour,unsigned char min,unsigned char sec)
{
	display(0,nodot[hour/10]);
	delay(200);
	display(1,nodot[hour%10]);
	delay(200);
	display(2,num[16]);
	delay(200);
	display(3,nodot[min/10]);
	delay(200);
	display(4,nodot[min%10]);
	delay(200);
	display(5,num[16]);
	delay(200);
	
	if(t_flag==0)
	{
		display(6,nodot[sec/10]);
		delay(200);
		display(7,nodot[sec%10]);
		delay(200);
	}
	else
	{
		display(6,num[17]);
		delay(200);
		display(7,num[17]);
		delay(200);
	}
	
	displayall(0xff);
	delay(200);
}




//-------------------------------
void scankeys()
{
	if(S7==0)
	{
		delay(20);
		if(S7==0)
		{
			mode_set=1;//time
			ET0=0,TR0=0;
			while(S7==0);//when pressed,don't read
			time_set++;
		
			switch(time_set)
			{
				case 1: hour_flash(t_hour,t_min,t_sec);
				        break;
				case 2: min_flash(t_hour,t_min,t_sec);
				        break;
				case 3: sec_flash(t_hour,t_min,t_sec);
				        break;
				default: showtime(t_hour,t_min,t_sec);
				        ET0=1,TR0=1;
					      time_set=0;
					      break;
			}
		}
	}
	
	if(S6==0)
	{
		delay(20);
		if(S6==0)
		{
			mode_set=2;//clock
			
			while(S6==0);//when pressed,don't read
			clock_set++;

			switch(clock_set)
			{
				case 1: hour_flash(c_hour,c_min,c_sec);
				        break;
				case 2: min_flash(c_hour,c_min,c_sec);
				        break;
				case 3: sec_flash(c_hour,c_min,c_sec);
				        break;
				default:showtime(t_hour,t_min,t_sec);
				
					      clock_set=0;
					      break;
			}
		}
	}
	if(S5==0)
	{
		delay(20);
		if(S5==0)
		{
			while(S5==0);
			
				if(time_set==1)
				{
					 t_hour++;
				   if(t_hour>=24)t_hour=0;
				}
				else if(time_set==2)
				{
					 t_min++;
				   if(t_min>=60)t_min=0;
				}
				else if(time_set==3)
				{
					 t_sec++;
				   if(t_sec>=60)t_sec=0;
				}
				
				else if(clock_set==1)
				{
					c_hour++;
				  if(c_hour>=24)c_hour=0;
				}
				else if(clock_set==2)
				{
					 c_min++;
				  if(c_min>=60)c_min=0;
				}
				else if(clock_set==3)
				{
					c_sec++;
				 if(c_sec>=60)c_sec=0;
				}
			}
		}
	
		if(S4==0)
	{
		delay(20);
		if(S4==0)
		{
			while(S4==0);
			
				if(time_set==1)
				{
					 t_hour--;
				   if(t_hour==0)t_hour=24;
				}
				else if(time_set==2)
				{
					 t_min--;
				   if(t_min==0)t_min=60;
				}
				else if(time_set==3)
				{
					 t_sec--;
				   if(t_sec==0)t_sec=60;
				}
				
				else if(clock_set==1)
				{
					c_hour--;
				  if(c_hour==0)c_hour=24;
				}
				else if(clock_set==2)
				{
					 c_min--;
				  if(c_min==60)c_min=60;
				}
				else if(clock_set==3)
				{
					c_sec--;
				 if(c_sec==0)c_sec=60;
				}
			}
		}
}
//---------------------
void main()
{
	XBYTE[0x8000]=0xff;
	inittime0();
	 
	while(1)
	{
		read_temp();
		if(time_set==0&&clock_set==0)
		{
			if(S4==0)
			{
				delay(20);
				if(S4==0)
				{
					while(S4==0)
					{
			     delay(20);
			     showtemp();
					}
				}
			}
			showtime(t_hour,t_min,t_sec);
		}
		
		scankeys();
		if(mode_set==1)
		switch(time_set)
			{
				case 1: hour_flash(t_hour,t_min,t_sec);
				        break;
				case 2: min_flash(t_hour,t_min,t_sec);
				        break;
				case 3: sec_flash(t_hour,t_min,t_sec);
				        break;
			}
		else if(mode_set==2)
		{
			switch(clock_set)
			{
				case 1: hour_flash(c_hour,c_min,c_sec);
				        break;
				case 2: min_flash(c_hour,c_min,c_sec);
				        break;
				case 3: sec_flash(c_hour,c_min,c_sec);
				        break;
			}
		}
		else if(((t_hour==c_hour)&&(t_min==c_min)&&(t_sec==c_sec))||(t_hour==0&&t_min==0&&t_sec==0))
		{
			
				if(led_flash==0)
					XBYTE[0x8000]=0xfe;
				else if(led_flash==1)
					XBYTE[0x8000]=0xff;
				delay(50000);
				delay(50000);
				XBYTE[0x8000]=0xff;
			}
		
	}
}