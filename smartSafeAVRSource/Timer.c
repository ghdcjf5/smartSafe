/*! Timer.c  
*****************************************************************************
//
// File Name	: 'Timer.c'
// Title		: Timer driver
// Author		: KYOUNG YOON JEONG
// Created		: 7/25/2017
// Version		: 1.1
// Target MCU	: Atmel AVR ATMega128
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
// *************************************************/ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

//#define FND_DATA_PORT PORTA		//FND 데이터버스는 PORTA에 연결
//#define FND_DATA_DDR DDRA
#define FND_CONTROL_PORT PORTG	//FND 콘트롤버스는 PORTG에 연결
#define FND_CONTROL_DDR DDRG
#define COUNTDOWN 3
volatile char sec, min, hour, date, day, month, year;
volatile char csec, sw_sec, sw_min, sw_hour;
volatile signed char timer_sec, timer_min, timer_hour;
volatile int dmsec;
volatile char harf_sec, time_flag;
//extern char FND[4];

/*SIGNAL(SIG_OUTPUT_COMPARE1A)   //winavr 사용시 인터럽트 서비스 루틴
{

}
*/

ISR(TIMER0_COMP_vect){			//AtmelStudio 사용시 인터럽트 서비스 루틴
	static char i=0;
	FND_CONTROL_PORT &= 0b11110000;			
	FND_CONTROL_PORT |= 0b00000001<<i;
//	FND_DATA_PORT = FND[i];
	i++;
	if(i==4)i=0;
}

ISR(TIMER1_COMPA_vect){			//AtmelStudio 사용시 인터럽트 서비스 루틴
	dmsec++;
	if(dmsec>=500){
		dmsec=0;
		time_flag = 1;
		sec = ds1302_getsec();
		if(sec == 0){
			min = ds1302_getmin();
			if(min==0){
				hour = ds1302_gethour();
				if(hour==0){
					ds1302_init_gettime();					
				}
			}
		}
		//FND_update(min*100+sec);
	}
	/*
	if(!(dmsec%250)){
		harf_sec ^= 1;
		FND_CONTROL_PORT ^= 0b00010000;
	}
	*/
}
void Timer0_Init(void){
	TCCR0 = 0b00001110;
	OCR0 =124;
	TIMSK |= 1<<OCIE0;
	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}
void Timer1A_Init(int ms){	//매개변수로 넘겨받는 시간 주기로 인터럽트 발생(단위 ms)
	int divider;

	TCCR1A |= 0;              //don't output OC1A
	TCCR1B |= (1<<WGM12) | (1<<CS12);  //Timer1A CTC_mod
	TCCR1C |= 0;       

	switch(TCCR1B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default : break;
	}

 	OCR1AH = (((F_CPU/divider)*ms/1000-1) >> 8);  
	OCR1AL = (((F_CPU/divider)*ms/1000-1) & 0XFF); 
	
	TCNT1H = 0x00;                               // clear Timer/Counter1
	TCNT1L = 0x00;

	TIMSK |= 1<<OCIE1A;                          // enable OC1A interrupt

	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}	
void Timer3A_Init(int ms){	
	int divider;
	
	TCCR3A |= (1<<COM3A0);
	TCCR3B |= (1<<WGM32) | (1<<CS30);
 	      
	switch(TCCR3B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default : break;
	}

 	OCR3AH = ((F_CPU/divider/1000*ms-1) >> 8);  
	OCR3AL = (F_CPU/divider/1000*ms-1) & 0XFF; 
	
	TCNT3H = 0X00;
 	TCNT3L = 0X00;
  
	//ETIMSK |= 1<<OCIE3A;
	TIFR = 0X00;
	ETIFR = 0X00;
}
void PWM_OC3A_init(void){
	TCCR3A |= (1<<COM3A1) | (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33) | (1<<CS31);
	
	OCR3A = 0;
	ICR3 = 19999;
}
	

