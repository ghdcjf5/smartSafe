/*
 * ds1302.h
 *
 * Created: 2018-03-28 오전 11:48:39
 *  Author: 6층교수실PC
 */ 

#define F_CPU	16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef DS1302_H_
#define DS1302_H_

#define ds1302_DDR DDRE
#define ds1302_PORT PORTE
#define ds1302_PIN PINE

#define clk 2
#define io 3
#define rst 4  // pin number pe2~pe4

#define rst_1() (ds1302_PORT |= (1<<rst))
#define rst_0() (ds1302_PORT &= ~ (1<<rst))
#define clk_1() (ds1302_PORT |= (1<<clk))
#define clk_0() (ds1302_PORT &= ~ (1<<clk))
#define io_1() (ds1302_PORT |= (1<<io))
#define io_0() (ds1302_PORT &= ~ (1<<io))

#define mon 1
#define tue 2
#define wed 3
#define thu 4
#define fri 5
#define sat 6
#define sun 7

#define sec_w 0x80
#define sec_r 0x81
#define min_w 0x82
#define min_r 0x83
#define hour_w 0x84
#define hour_r 0x85
#define date_w 0x86
#define date_r 0x87
#define month_w 0x88
#define month_r 0x89
#define day_w 0x8a
#define day_r 0x8b
#define year_w 0x8c
#define year_r 0x8d

#define w_protect 0x8e

void ds1302_reset(void);	//ds1302_reset
void ds1302_init(void); //ds1302 init
void write(unsigned char);
unsigned char read(void);
void ds1302_write_byte(unsigned char, unsigned char);
unsigned char ds1302_read_byte(unsigned char);
unsigned char ds1302_getsec(void);
unsigned char ds1302_getmin(void);
unsigned char ds1302_gethour(void);
unsigned char ds1302_getdate(void);
unsigned char ds1302_getmonth(void);
unsigned char ds1302_getday(void);
unsigned char ds1302_getyear(void);
void ds1302_setsec(unsigned char);
void ds1302_setmin(unsigned char);
void ds1302_sethour(unsigned char);
void ds1302_setdate(unsigned char);
void ds1302_setmonth(unsigned char);
void ds1302_setday(unsigned char);
void ds1302_setyear(unsigned char);
uint8_t bcd_to_decimal(uint8_t bcd);
uint8_t decimal_to_bcd(uint8_t decimal);





#endif /* DS1302_H_ */