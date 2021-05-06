#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define Key_DDR DDRD
#define Key_PORT PORTD  //출력포트
#define Key_PIN  PIND  //입력포트


/*
#define ADD 11
#define SUB 12
#define MUL 13
#define DIV 14
#define EQUAL 15
#define CLEAR 16

*/

#define ZERO 10
#define star 11
#define sharp 12

void Key_Init(void){
	Key_DDR = 0b11110000;  //입력을 하위 4비트 출력은 상위 4비트 쓸것이다. 초기화 
	Key_PORT = 0x00;  //전원이 안들어가고있는것
}

unsigned char Key_scan_sub(void){
	char i;
	i = Key_PIN & 0x0F;  //하위 4비트만 본다.
	if(i == 0x01)return 1;
	else if(i == 0x02)return 2;
	else if(i == 0x04)return 3;
	else if(i == 0x08)return 4;
	else return 0;  //각 해당핀에서 1,2,3,4를 return
}
unsigned char Key_scan(void){  //0~16까지 17개 리턴
	char i;
	for(i=0;i<4;i++){
		Key_PORT = 0x10 << i;
		_delay_ms(10);  //chattering방지
		if(Key_scan_sub()){
			_delay_ms(2);  //chattering 방지
			return i*4+Key_scan_sub();
		}
	}
	return 0;
}
/*
unsigned char Key_trans(unsigned char key_value){
	switch(key_value){
		case 1 : return 1; break;//return하면 함수 끝나니까 break필요없다. //o
		case 2 : return 2; break; //o sharp
		case 3 : return 3; break;
	//	case 4 : return 3; break;
		case 5 : return 4; break; //o
		case 6 : return 5; break; //o
		case 7 : return 6; break; //o
	//	case 8 : return 3; break;
		case 9 : return 7; break;//o
		case 10 : return 8; break; //o
		case 11 : return 9; break;//o
	//	case 12 : return 3; break;
		case 13 : return star; break;
		case 14 : return ZERO; break;//변경
		case 15 : return sharp; break;  //변경
	//	case 16 : return 3; break;
		
		default: return 0; break;


/// 키패드의
//*********************
//123
//456
//789
//*0#
//밑의단자 oooo ooo
//         row columm
//row의 왼쪽부터 순서대로
//PD7~4(AVR)
//columm의 왼쪽부터 순서대로
//PD0~2(AVR)연결 및 gnd연결(pulldown저항 필요)
//***************
		
	}
	
}
*/

