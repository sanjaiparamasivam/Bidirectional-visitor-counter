

#define si_bit 3
#define sto_bit 4
#define sta_bit 5
#define i2cen_bit 6
#define loadval 0x4b

typedef unsigned char u8;
typedef unsigned int u32;

void delay_ms(unsigned int ms)
{
	T0PR = 15000-1;
	T0TCR = 0X01;
	while(T0TC < ms);
	T0TCR = 0X03;
	T0TCR = 0X00;
}


void i2c_init(void);
void i2c_eeprom_write(u8,u8,u8);
u8 i2c_eeprom_read(u8,u8);
void i2c_start(void);
void i2c_write(u8);
void i2c_stop(void);
u8 i2c_nack(void);
void i2c_restart(void);

void i2c_init(void)
{
	PINSEL0 |= 0X50;
	I2SCLL = loadval;
	I2SCLH = loadval;
	I2CONSET = 1<<i2cen_bit;
}
void i2c_eeprom_write(u8 slaveAddr,u8 wordAddr,u8 data)
{
	i2c_start();
	i2c_write(slaveAddr<<1);
	i2c_write(wordAddr);
	i2c_write(data);
	i2c_stop();
	delay_ms(10);
}
u8 i2c_eeprom_read(u8 slaveAddr,u8 wordAddr)
{
	u8 data;
	i2c_start();
	i2c_write(slaveAddr<<1);
	i2c_write(wordAddr);
	i2c_restart();
	i2c_write(slaveAddr<<1|1);
	data = i2c_nack();
	i2c_stop();

	return data;
}
void i2c_start(void)
{
	I2CONSET = 1<<sta_bit;
	//wait until si bit set
	while(((I2CONSET>>si_bit)&1)==0);
	I2CONCLR = 1<<sta_bit;
}
void i2c_write(u8 data)
{
	
	I2DAT = data;
	I2CONCLR = 1<<si_bit;
	while(((I2CONSET>>si_bit)&1)==0);
}
void i2c_stop(void)
{
	I2CONSET = 1<<sto_bit;
	I2CONCLR = 1<<si_bit;
}

u8 i2c_nack(void)
{
	//I2CONSET = 0X00;
	I2CONCLR = 1<<si_bit;
	while(((I2CONSET>>si_bit)&1)==0);
	return I2DAT;
}
void i2c_restart(void)
{
	I2CONSET = 1<<sta_bit;
	//clr si_bit
	I2CONCLR = 1<<si_bit;
	while(((I2CONSET>>si_bit)&1)==0);
	I2CONCLR = 1<<sta_bit;
}

