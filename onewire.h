#ifndef __ONEWIRE_H
#define __ONEWIRE_H


unsigned int rd_temperature(void);
bit init_ds18b20(void);
unsigned char Read_DS18B20(void);
void Write_DS18B20(unsigned char dat);
void Delay_OneWire(unsigned int t);
#endif
