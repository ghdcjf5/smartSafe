/*
 * Clock.c
 *
 * Created: 2018-03-12 오전 9:19:50
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>

#define FND_DATA_PORT PORTA		//FND 데이터버스는 PORTA에 연결
#define FND_DATA_DDR DDRA
#define FND_CONTROL_PORT PORTG	//FND 콘트롤버스는 PORTG에 연결
#define FND_CONTROL_DDR DDRG
#define LED_PORT PORTC

#define CLOCK		0
#define CORRECT		1
#define STOPWATCH	2
#define COUNTDOWN	3
#define START		0b10000000
#define LAB			0b01000000
#define CLEAR		0b00100000
#define UP			0b01000000
#define CURSOR		0b00100000
char FND[4];
char FND_FONT[10] = {	0b11000000, 0b11111001, 0b10100100,
						0b10110000, 0b10011001, 0b10010010,
						0b10000010, 0b11111000, 0b10000000,
						0b10010000};
char volatile change_time_flag;
extern char sec, min, hour;
extern volatile char harf_sec;
extern volatile char csec, sw_sec, sw_min, sw_hour;
extern volatile signed char timer_sec, timer_min, timer_hour;
extern int He_Pirate[];
extern int He_Pirate1_Beat[];
extern int School_Bell_Tune[];
extern const int School_Bell_Beats[];
extern int Elise_Tune[];
extern const int Elise_Beats[];
extern int GrandFa_Tune[];
extern const int GrandFa_Beats[];

char mod = COUNTDOWN;
volatile char start_flag, play_flag, countdown_flag;
char lab_flag = 1;
char cursor;

int main(void)
{
	char key_value = 0, old_key=0;
	char long_key = 0;
	
	FND_init();
	led_init();
	Timer1A_Init(2);
	Timer0_Init();
	Timer3A_Init(2);
	Speaker_Init();
	FND_update(0);
	LCD_init();
	LCD_string(0x80,"   KCCI Clock   ");
	LCD_string(0xC0,"    00:00:00    ");
	DDRD &= 0b00001111;
	//Music_Player(GrandFa_Tune,GrandFa_Beats); 
	
	sei();
	while (1)
	{
		if(change_time_flag){
			change_time_flag=0;
			Print_Time();
		}
		if(play_flag){
			play_flag = 0;
			Music_Player(School_Bell_Tune,School_Bell_Beats);
		}
		if(PIND & 0b00010000){
			long_key++;
			_delay_ms(10);
			if(long_key >= 200){
				long_key = 0;
				Beep(1);
				mod++;
				if(mod>=4)mod=CLOCK;
				if(mod==CLOCK)LCD_string(0xC0,"    00:00:00    ");
				else if(mod==STOPWATCH)LCD_string(0xC0,"  00:00:00:00  ");
				else if(mod==COUNTDOWN)LCD_string(0xC0,"    00:00:00    ");
			}
		}
		if(old_key != PIND){
			_delay_us(100);
			old_key = PIND;
			
			if(mod == STOPWATCH){
				if(PIND & START){
					start_flag ^= 1;
					lab_flag = 1;
					Beep(1);
				}
				if(PIND & LAB){
					lab_flag ^= 1;
					Beep(1);
				}
				if(PIND & CLEAR){
					csec=0;
					sw_sec=0;
					sw_min=0;
					sw_hour=0;
					Beep(1);
				}
				
			}
			if(mod == CORRECT){
				if(PIND & 0b10000000){
					hour++;
					if(hour>=24)hour=0;
					Beep(1);
				}
				if(PIND & 0b01000000){
					min++;
					if(min>=60)min=0;
					Beep(1);
				}
				if(PIND & 0b00100000){
					sec++;
					if(sec>=60)sec=0;
					Beep(1);
				}
				
			}
			if(mod == COUNTDOWN){
				
				if(PIND & START){
					countdown_flag = 1;
					Beep(1);
				}
				if(PIND & UP){
					switch(cursor){
						case 0: timer_sec++;
							if(timer_sec>= 60)timer_sec=0;
							break;
						case 1: timer_min++;
							if(timer_min>= 60)timer_min=0;
							break;
						case 2: timer_hour++;
							if(timer_hour>= 100)timer_hour=0;
							break;
					}
					Beep(1);
				}
				if(PIND & CURSOR){
					cursor++;
					if(cursor>=3)cursor=0;
					Beep(1);
				}
			}
			
		}
	}
}
void Print_Time(void){
	if(mod == STOPWATCH){
		if(lab_flag){
			LCD_command(0xC2);
			LCD_data('0'+(sw_hour/10));
			LCD_data('0'+(sw_hour%10));
			LCD_command(0xC5);
			LCD_data('0'+(sw_min/10));
			LCD_data('0'+(sw_min%10));
			LCD_command(0xC8);
			LCD_data('0'+(sw_sec/10));
			LCD_data('0'+(sw_sec%10));
			LCD_command(0xCB);
			LCD_data('0'+(csec/10));
			LCD_data('0'+(csec%10));
		}
		
	}
	else if(mod==CLOCK || mod==CORRECT){
		if(harf_sec || mod == CLOCK){
			LCD_command(0xC4);
			LCD_data('0'+(hour/10));
			LCD_data('0'+(hour%10));
			LCD_command(0xC7);
			LCD_data('0'+(min/10));
			LCD_data('0'+(min%10));
			LCD_command(0xCA);
			LCD_data('0'+(sec/10));
			LCD_data('0'+(sec%10));
		}
		else {
			LCD_string(0xC4,"  :  :  ");
		}
	}
	else {
		LCD_command(0xC4);
		LCD_data('0'+(timer_hour/10));
		LCD_data('0'+(timer_hour%10));
		LCD_command(0xC7);
		LCD_data('0'+(timer_min/10));
		LCD_data('0'+(timer_min%10));
		LCD_command(0xCA);
		LCD_data('0'+(timer_sec/10));
		LCD_data('0'+(timer_sec%10));
		if(harf_sec && countdown_flag == 0){
			if(cursor==0){
				LCD_command(0xCA);
				LCD_data(' ');
				LCD_data(' ');
			}else if(cursor==1){
				LCD_command(0xC7);
				LCD_data(' ');
				LCD_data(' ');
			}else{
				LCD_command(0xC4);
				LCD_data(' ');
				LCD_data(' ');
			}
		}
	}
}
void FND_init(void){
	FND_DATA_DDR = 0xff;
	FND_DATA_PORT = 0;
	FND_CONTROL_DDR = 0x1f;
	FND_CONTROL_PORT = 0;
}
void FND_update(unsigned int data){
	FND[0] = FND_FONT[data/1000%10];
	FND[1] = FND_FONT[data/100%10];
	FND[2] = FND_FONT[data/10%10];
	FND[3] = FND_FONT[data%10];
}
void FND_output(void){
	char i;
	for(i=0;i<4;i++){
		FND_CONTROL_PORT = 0b00000001<<i;
		FND_DATA_PORT = FND[i];
		_delay_ms(2);
	}
}


