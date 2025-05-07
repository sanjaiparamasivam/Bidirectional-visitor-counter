#include<lpc21xx.h>
#include"i2c.h"
#include"lcd.h"

#define IN 8
#define OUT 9
#define LED 1<<17

void count(void);

int main()
{
	
	unsigned int in = 00,out = 00,total = 00;
	count();
	i2c_init();

	/*i2c_eeprom_write(0x50,0x00,in);
	i2c_eeprom_write(0x50,0x01,out);
	i2c_eeprom_write(0x50,0x02,total);
	  */
	in = i2c_eeprom_read(0x50,0x00);
	delay_ms(100);

	out = i2c_eeprom_read(0x50,0x01);
	delay_ms(100);

	total = i2c_eeprom_read(0x50,0x02);
	delay_ms(100);
	  

	IODIR0 |= LED;
	
	 while(1)
	 {
		if(((IOPIN0>>IN)&1)==0)
		{
			delay_ms(500);
			in++;
			
			i2c_eeprom_write(0x50,0x00,in);
			LCD_COMMAND(0X83);
		
			LCD_DATA((in/10)+48);
			LCD_DATA((in%10)+48);
			
		}
		if(IN>OUT)
		{
			if(((IOPIN0>>OUT)&1)==0)
			{
				delay_ms(500);
				out++;
				i2c_eeprom_write(0x50,0x01,out);
			
				LCD_COMMAND(0X8d);
			
				LCD_DATA((out/10)+48);
				LCD_DATA((out%10)+48);
			
			}
		}
		total = in-out;
		i2c_eeprom_write(0x50,0x02,total);
		
		LCD_COMMAND(0XCb);
		LCD_DATA((total/10)+48);
		LCD_DATA((total%10)+48);
		
				if(total == 0)
				{
					IOSET0 = LED;
				}
				else
				{
					IOCLR0 = LED;
				}	
	}
}
void count()
{
	LCD_INIT();
	LCD_COMMAND(0X80);
	LCD_SCROL("BIDIRECTIONAL VISITOR COUNTER");

	LCD_COMMAND(0XC4);
	LCD_STR("V24CE2S4");

	delay_ms(1000);
	LCD_COMMAND(0X01);

	LCD_COMMAND(0X80);
	LCD_STR("IN:");

	LCD_COMMAND(0X89);
	LCD_STR("OUT:");

	LCD_COMMAND(0XC5);
	LCD_STR("TOTAL:");
	
}




