/*
 * FND.c
 *
 * Created: 2018-02-23 오후 5:16:16
 * Author : user
 * FND : CommonA type 
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>

//#define FND_DATA_PORT PORTA		//FND 데이터버스는 PORTA에 연결
//#define FND_DATA_DDR DDRA		
//#define FND_CONTROL_PORT PORTG	//FND 콘트롤버스는 PORTG에 연결
//#define FND_CONTROL_DDR DDRG
char FND[4];
char FND_FONT[10] = {	0b11000000, 0b11111001, 0b10100100,
						0b10110000, 0b10011001, 0b10010010,
						0b10000010, 0b11111000, 0b10000000,
						0b10010000};
/*				
void FND_init(void){
	FND_DATA_DDR = 0xff;
	FND_DATA_PORT = 0;
	FND_CONTROL_DDR = 0x1f;
	FND_CONTROL_PORT = 0;
}
*/
void FND_update(unsigned int data){
	FND[0] = FND_FONT[data/1000%10];
	FND[1] = FND_FONT[data/100%10];
	FND[2] = FND_FONT[data/10%10];
	FND[3] = FND_FONT[data%10];
}
/*
void FND_output(void){
	char i;
	for(i=0;i<4;i++){
		FND_CONTROL_PORT = 0b00000001<<i;
		FND_DATA_PORT = FND[i];
		_delay_ms(2);
	}
}
*/

