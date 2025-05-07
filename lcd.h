#include<lpc21xx.h>

#define LCD_D 0x0f<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19

void LCD_INIT(void);
void LCD_COMMAND(char);
void LCD_DATA(char);
void LCD_STR( char *);
void LCD_STRING(char *);
void LCD_SCROL(char *);

void LCD_SCROL( char *s)
{
	int i,j,len =0;
	while(s[len]!='\0')
	{
		len++;
	}
	for(i=0;i<=len;i++)
	{
		LCD_COMMAND(0X80);
		for(j=0;j<16;j++)
		{
			if((i+j)<len)
			{
				LCD_DATA(s[i+j]);
			}
			else
			{
				LCD_DATA(' ');
			}
		}
		delay_ms(300);
	}
}
void LCD_INIT()
{ 
	IODIR1 = LCD_D|RS|RW|E;
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X02);
	LCD_COMMAND(0X0C);
	LCD_COMMAND(0X28);
	LCD_COMMAND(0X80);
}
void LCD_COMMAND(char CMD)
{
	IOCLR1 = LCD_D;
	IOSET1 = (CMD&0XF0)<<16;
	IOCLR1 = RS;
	IOCLR1 = RW;
	IOSET1 = E;
	delay_ms(2);
	IOCLR1 = E;

	IOCLR1 = LCD_D;
	IOSET1 = (CMD&0X0F)<<20;
	IOCLR1 = RS;
	IOCLR1 = RW;
	IOSET1 = E;
	delay_ms(2);
	IOCLR1 = E;
}
void LCD_STR(char *s)
{
	while(*s)
	{
		LCD_DATA(*s++);
	}
}
void LCD_DATA(char d)
{
	IOCLR1 = LCD_D;
	IOSET1 = (d&0XF0)<<16;
	IOSET1 = RS;
	IOCLR1 = RW;
	IOSET1 = E;
	delay_ms(2);
	IOCLR1 = E;

	IOCLR1 = LCD_D;
	IOSET1 = (d&0X0F)<<20;
	IOSET1 = RS;
	IOCLR1 = RW;
	IOSET1 = E;
	delay_ms(2);
	IOCLR1 = E;
}
void LCD_STRING(char *p)
{
	unsigned int i;
	for(i=0;p[i];i++)
	{
		LCD_DATA(p[i]);
	}
}


