/*
 * LED.c
 * 
 * Created: 2018-02-23 ¿ÀÀü 9:15:56
 * Author : Jeong Kyoung Yoon
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>

void PIN_init(void);
void Calculator(void);

unsigned long int Money = 0;

void PIN_init(void){
	DDRA = 0b11110000;
	PORTA = 0b00000000;
}

void Calculator(void){
	if(PORTA == 0b00000001)
	{
		Money += 10;
	}
	else if(PORTA == 0b00000010)
	{
		Money += 50;
	}
	else if(PORTA == 0b00000011)
	{
		Money += 100;
	}
	else if(PORTA == 0b00000100)
	{
		Money += 500;
	}
	else if(PORTA == 0b00000101)
	{
		Money += 1000;
	}
	else if(PORTA == 0b00000110)
	{
		Money += 5000;
	}
	else if(PORTA == 0b00000111)
	{
		Money += 10000;
	}
	else if(PORTA == 0b00001000)
	{
		Money += 50000;
	}
}

