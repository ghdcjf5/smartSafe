/*
 * LED.c
 * 
 * Created: 2018-02-23 오전 9:15:56
 * Author : Jeong Kyoung Yoon
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>

void led_off_upto(void);
void led_on_downto(void);
void led_off_downto(void);
void led_on_upto(void);
void led_move_upto(void);
void led_move_downto(void);
void led_init(void);
void led_off_move_upto(void);
void led_off_move_downto(void);
void led_close_open(void);
void led_all_close(void);
void led_all_open(void);
void led_all_open2(void);

void led_all_close(void){
	char i;
	for(i=0;i<5;i++){
		PORTC = (0b11111111 << (8-i)) | (0b11111111 >> (8-i));
		_delay_ms(200);
	}
}
void led_all_open(void){
	char i;
	for(i=0;i<5;i++){
		PORTC = ~((0b11111111 << (8-i)) | (0b11111111 >> (8-i)));
		_delay_ms(200);
	}
}
void led_all_open2(void){
	char i;
	for(i=0;i<5;i++){
		PORTC = (0b11111111 << i) & (0b11111111 >> i);
		_delay_ms(200);
	}
}
void led_close_open(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = (0b00000001 << i) | (0b10000000 >> i);
		_delay_ms(200);
	}
}
void led_off_move_upto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = (0b11111110 << i) + 1;
		_delay_ms(200);
	}
}
void led_off_move_downto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = ~(0b10000000 >> i);
		_delay_ms(200);
	}
}
void led_init(void){
	DDRC = 0b11111111;
	PORTC = 0b00000000;
}
void led_move_upto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = 0b00000001 << i;
		_delay_ms(200);
	}
}
void led_move_downto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = 0b10000000 >> i;
		_delay_ms(200);
	}
}
void led_off_upto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = 0b11111111 << i ;
		_delay_ms(200);
	}
}
void led_on_downto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = 0b11111111 << (8-i);
		_delay_ms(200);
	}
}
void led_off_downto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = 0b11111111 >> i;
		_delay_ms(200);
	}
}
void led_on_upto(void){
	char i;
	for(i=0;i<9;i++){
		PORTC = 0b11111111 >> (8-i);
		_delay_ms(200);
	}
}

void led_on_green(void){
	PORTC = 0b00001000;
	PORTA = 0b00100000;
	_delay_ms(100);
	PORTA = 0b00000000;
	_delay_ms(3000);
	PORTC = 0b00000000;
}

void led_on_yellow(void){
	PORTC = 0b00000100;
	PORTA = 0b11100000;
	_delay_ms(100);
	PORTA = 0b00000000;
	_delay_ms(3000);
	PORTC = 0b00000000;
}

void led_on_red(void){
	PORTC = 0b00000001;
	PORTA = 0b11100000;
	_delay_ms(100);
	PORTA = 0b00000000;
	_delay_ms(3000);
	PORTC = 0b00000000;
}


