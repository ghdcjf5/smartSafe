/*
 * ds1302.c
 *
 * Created: 2018-03-28 오전 11:48:20
 *  Author: 6층교수실PC
 */ 
#include "ds1302.h"

uint8_t bcd_to_decimal(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0f);
}

uint8_t decimal_to_bcd(uint8_t decimal)
{
	uint8_t temp=0;
	temp = (decimal / 10) << 4;
	temp = temp + (decimal % 10);
	//return ( ((decimal / 10) << 4) | (decimal % 10) );
	return temp;
}

unsigned char ds1302_getsec(void)	//read the second
{
	unsigned char temp;
	temp = ds1302_read_byte(sec_r);
	return bcd_to_decimal(temp);
}

unsigned char ds1302_getmin(void)	//read the minute
{
	unsigned char temp;
	temp = ds1302_read_byte(min_r);
	return bcd_to_decimal(temp);
}

unsigned char ds1302_gethour(void)	//read the byte with register w_byte
{
	unsigned char temp;
	temp = ds1302_read_byte(hour_r);
	return bcd_to_decimal(temp);
}

unsigned char ds1302_getdate(void)	//read the byte with register w_byte
{
	unsigned char temp;
	temp = ds1302_read_byte(date_r);
	return bcd_to_decimal(temp);
}

unsigned char ds1302_getmonth(void)	//read the byte with register w_byte
{
	unsigned char temp;
	temp = ds1302_read_byte(month_r);
	return bcd_to_decimal(temp);
}

unsigned char ds1302_getday(void)	//read the byte with register w_byte
{
	unsigned char temp;
	temp = ds1302_read_byte(day_r);
	return bcd_to_decimal(temp);
}

unsigned char ds1302_getyear(void)	//read the byte with register w_byte
{
	unsigned char temp;
	temp = ds1302_read_byte(year_r);
	return bcd_to_decimal(temp);
}

void ds1302_setsec(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(sec_w, temp);
}

void ds1302_setmin(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(min_w, temp);
}

void ds1302_sethour(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(hour_w, temp);
}

void ds1302_setdate(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(date_w, temp);
}

void ds1302_setmonth(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(month_w, temp);
}

void ds1302_setday(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(day_w, temp);
}

void ds1302_setyear(unsigned char w_byte)	//write the byte with register w_byte
{
	unsigned char temp;
	temp = decimal_to_bcd(w_byte);
	ds1302_write_byte(year_w, temp);
}

unsigned char ds1302_read_byte(unsigned char w_byte)	//read the byte with register w_byte
{
	unsigned char temp;
	ds1302_reset();    // ready
	write(w_byte);
	temp = read();
	rst_0(); // finish transmittion
	clk_0();
	
	return temp;
}

void ds1302_init(void) // sets all pins as output and low
{
	ds1302_DDR |= (1 << rst) | (1 << clk) | (1 << io);
	clk_0(); // sclk -> 0
	rst_0(); // rst -> 0
	io_0(); // io -> 0
}

void ds1302_reset(void)	//sets the pins to begin the ds1302 communication
{
	clk_0(); // sclk -> 0
	rst_0(); // rst -> 0
	rst_1(); // rst -> 1
}

void ds1302_write_byte(unsigned char w_byte, unsigned char w_2_byte)	//write the byte with register w_byte
{
	ds1302_reset();  // ready
	write(w_byte);   // write address
	write(w_2_byte); // write data
	rst_0(); // finish transmittion
	clk_0();
}

void write(unsigned char W_Byte)	//writes the W_Byte to the DS1302
{
	unsigned char i;

	ds1302_DDR |= (1 << io); // io as output -> 1

	for (i = 0; i < 8; ++i)
	{
		/*
		io_0();

		if (W_Byte & 0x01)
		{
			io_1();
		}
		*/
		(W_Byte & 0x01) ? io_1() : io_0();
		clk_0();
		_delay_us(2);
		clk_1();
		_delay_us(2);
		W_Byte >>= 1;
	}
}

unsigned char read(void)	//reads the ds1302 reply
{
	unsigned char i, R_Byte, TmpByte;

	R_Byte = 0x00;

	ds1302_DDR &= ~(1 << io); // io as input -> 0

	for (i = 0; i < 8; ++i) //get byte
	{
		clk_1();
		_delay_us(2);
		clk_0();
		_delay_us(2);
		/*
		TmpByte = 0;
		if (bit_is_set(PINA, io))
			TmpByte = 1;
		TmpByte <<= 7;
		R_Byte >>= 1;
		R_Byte |= TmpByte;
		*/
		ds1302_PIN & (1 << io) ? (TmpByte = 1) : (TmpByte = 0);
		TmpByte <<= i;
		R_Byte |= TmpByte;
	}
	return R_Byte;
}
