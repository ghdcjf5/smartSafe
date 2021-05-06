/*
 * clock_password.c
 *
 * Created: 2018-06-08 오전 9:53:36
 * Author : Ro
 * Reference : KCCI_K.Y Jung, M.I Son, J.O Beak
 
 1. H/W 연결
 (1)키패드
********************* 
키패드의
123
456
789
*0#
밑의단자 oooo ooo
         row columm
row의 왼쪽부터 순서대로
PD7~4(AVR)
columm의 왼쪽부터 순서대로
PD0~2(AVR)연결 및 gnd연결(pulldown저항(1k) 필요)
6
bread board 근처의 n1~7기준으로
n1234가 pd4567
n567이 pd012
***************
(2) LCD
단자 순서대로 gnd, Vcc, 1k저항 gnd, pe5~pe7, pb0~pb7

(3) ds1302
gnd는 gnd에 
clk은 pe2
dat은 pe3
rst은 pe4
 
 cf)PG단자는 fnd의 control reg로 사용됐으며 fnd기반으로
 ds1302가 작동되므로 연결금지
 PA단자는 사용가능 만일 fnd를 사용하고 싶으면 timer.c와 fnd.c(추가시)의
 define 해제시 이용가능
 */ 


extern volatile char sec, min, hour, date, day, month, year, time_flag;
#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/interrupt.h>

#define Scr_Shift_Left 0x18
#define LCD_Clear 0x01
#define zero 10
#define add 11
#define sub 12
#define mul 13
#define div 14
#define op 15
#define clear 16

 int mode =0, failed_count=0;
 int password[5] ={1, 1, 1, 1, 1};
 char trans_value=100;
 
 int result[5]={999, 999, 999, 999, 999};
 unsigned int cursor = 0, pw_input_flag=0, result_check_flag=0;
 void ds1302_init_settime();
 unsigned long int Money = 0;
 
int main(void)
{
   
  DDRF=0x01;
  PORTF=0x00;
  DDRA = 0b11110000;
  PORTA = 0b00000000;
 
    Timer0_Init();
    Timer1A_Init(2);
	//Timer3A_Init(2);
    LCD_init();
	led_init();
//	Speaker_Init();
	Money_PIN_init();
    LCD_string(0x80,"   00-00-00     ");
    LCD_string(0xC0,"    00:00:00    ");
    
    ds1302_init();
    ds1302_init_settime(); //초기에 한번은 넣어줘야한다. 그후 //로 막기
    ds1302_init_gettime();
	Key_Init();
    sei();

    while (1) 
    {
		if(time_flag){
			time_flag = 0;
			LCD_Time_print();
		}
		//PORTA = 0b00000000;
		
		/*if(PORTA == 0b00000001 || PORTA == 0b00000010 || PORTA == 0b00000011 || PORTA == 0b00000100 || PORTA == 0b00000101 || PORTA == 0b00000110 || PORTA == 0b00000111 || PORTA == 0b00001000)
		{
			LCD_command(LCD_Clear);
			LCD_string(0x80,"Your Balance");
			LCD_command(0xC0);
			LCD_7D(20000000);
			_delay_ms(3000);
		}*/
      Key_process(Key_scan());
    }
}
unsigned char Key_trans(unsigned char key_value){
   switch(key_value){
      case 1 : return 1; break;//return하면 함수 끝나니까 break필요없다. //o
      case 2 : return 2; break; //o sharp
      case 3 : return 3; break;
      //   case 4 : return 3; break;
      case 5 : return 4; break; //o
      case 6 : return 5; break; //o
      case 7 : return 6; break; //o
      //   case 8 : return 3; break;
      case 9 : return 7; break;//o
      case 10 : return 8; break; //o
      case 11 : return 9; break;//o
      //   case 12 : return 3; break;
      case 13 : return 11; break;//star
      case 14 : return zero; break;//변경
      case 15 : return 12; break;  //sharp
      //   case 16 : return 3; break;
      
      default: return 0; break;
   }
}
void Key_process(unsigned char key_value){
   static unsigned char old_key=0, opcode=0;
	int tmp[5]={999, 999, 999, 999, 999};
   _delay_us(10);
   
   if(key_value==Key_scan()){
	  
      if(old_key != key_value){
         old_key = key_value;
         
         if(pw_input_flag == 0 && opcode==0){
            LCD_command(LCD_Clear);
            _delay_ms(5);
            LCD_command(0xCF);
         }
         
         trans_value = Key_trans(old_key);
         
         if(mode==0){
            if(trans_value==11){
               mode=1;
               LCD_string(0x80," input password");
            }
            else if(trans_value==12){
				mode=2;
				LCD_string(0x80," check password");
			}

            else{
				mode=0;
				LCD_Time_print();
			}
 
         }
		 
          if(mode==1||mode==2||mode==3){
            if(cursor <= 4)   
            {
               key_value=Key_scan();
               trans_value = Key_trans(key_value);
               
               if(1 <= trans_value && trans_value <= 10){
                  switch(trans_value){
                     case zero :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('0');
                     result[cursor+1] = 0;
					 cursor++;
                     pw_input_flag=1;
                     break;
                     case 1:
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('1');
                     result[cursor+1] = 1;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 2 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('2');
                     result[cursor+1] = 2;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 3 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('3');
                     result[cursor+1] = 3;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 4 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('4');
                     result[cursor+1] = 4;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 5 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('5');
                     result[cursor+1] = 5;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 6 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('6');
                     result[cursor+1] = 6;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 7 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('7');
                     result[cursor+1] = 7;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 8 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('8');
                     result[cursor+1] = 8;
                     cursor++;
                     pw_input_flag=1;
                     break;
                     case 9 :
                     if(pw_input_flag)LCD_command(Scr_Shift_Left);
                     else {
                        LCD_command(LCD_Clear);
                        _delay_ms(5);
                        LCD_command(0xCF);
                     }
                     LCD_data('9');
                     result[cursor+1] = 9;
                     cursor++;
                     pw_input_flag=1;
                     break;
					 
				  }
				  
				   PORTA = 0b11000000;
				   _delay_ms(100);
				   PORTA = 0b00000000;
			   }
                     else if(trans_value>=11 && trans_value<=16){
                        if(pw_input_flag){
                           if(mode==1){
                              if(trans_value==11){
                                 
                                 opcode = 1;

                                 LCD_command(LCD_Clear);
                                 _delay_ms(5);
                                 LCD_command(0xC0);
                                 
                                 switch(cursor){
                                    case 4 : LCD_data('0' + result[4]);LCD_command(Scr_Shift_Left);
                                    case 3 : LCD_data('0' + result[3]);LCD_command(Scr_Shift_Left);
                                    case 2 : LCD_data('0' + result[2]);LCD_command(Scr_Shift_Left);
                                    case 1 : LCD_data('0' + result[1]);LCD_command(Scr_Shift_Left);break;
                                 //   default: LCD_string(0x80,"Too Big"); break;
                                 }
								 
								 for(int i=1;i<=cursor;i++)
								 {
									 tmp[i] = result[i];
								 }
								 
                                if(cursor == 1)
                                {
	                                result_check_flag = (tmp[1]==password[1]) ? 1 : 0;
                                }
                                else if(cursor == 2)
                                {
	                                result_check_flag = (tmp[1]==password[1] && tmp[2]==password[2]) ? 1 : 0;
                                }
                                else if(cursor == 3)
                                {
	                                result_check_flag = (tmp[1]==password[1] && tmp[2]==password[2] && tmp[3]==password[3]) ? 1 : 0;
                                }
                                else if(cursor == 4)
                                {
	                                result_check_flag = (tmp[1]==password[1] && tmp[2]==password[2] && tmp[3]==password[3] && tmp[4]==password[4]) ? 1 : 0;
                                }

                                 if(result_check_flag){
                                    LCD_string(0x80,"       success!!! ");
									//Beep(1);
									led_on_green();
                                    _delay_ms(200);
                                    LCD_command(LCD_Clear);
                                    _delay_ms(5);
                                    LCD_command(0xc0);
									PORTF=0x01;
                                    opcode = 0;
                                    mode=0;
                                    result_check_flag=0;
                                    pw_input_flag=0;
									cursor = 0;
									for(int i=0;i<6;i++)
									{
										result[i] = 999;
										tmp[i] = 999;
									}
                               //     LCD_string(0xC0,"                ");
                               //     LCD_command(0xCF);
                                 }
                                 else{
									 if(failed_count>=4)
									 {
										 LCD_string(0x80,"       Warning!!! ");
										 led_on_red();
										 //Siren(1);
										 failed_count=0;
									 }
									 else
									 {
										 LCD_string(0x80,"       fail... ");
										 led_on_yellow();
										 //RRR();
										 failed_count++;
									 }
                                   _delay_ms(200);
                                   LCD_command(LCD_Clear);
                                   _delay_ms(5);
                                   LCD_command(0xc0);
                                    opcode = 0;
                                    mode=0;
                                    result_check_flag=0;
                                    pw_input_flag=0;
									cursor = 0;
									for(int i=0;i<6;i++)
									{
										result[i] = 999;
										tmp[i] = 999;
									}
                                //    LCD_command(0xCF);
								
                                 }
								  _delay_ms(4000);
								PORTF=0x00; 
                              }     
                           }
                           else if(mode==2){ 
                              if(trans_value==12){
                                 opcode = 1;
                                 
                                 LCD_command(LCD_Clear);
                                 _delay_ms(5);
                                 LCD_command(0xc0);
                                 
                                 switch(cursor){
                                    case 4 : LCD_data('0' + result[4]);LCD_command(Scr_Shift_Left);
                                    case 3 : LCD_data('0' + result[3]);LCD_command(Scr_Shift_Left);
                                    case 2 : LCD_data('0' + result[2]);LCD_command(Scr_Shift_Left);
                                    case 1 : LCD_data('0' + result[1]);LCD_command(Scr_Shift_Left);break;
                                  //  default: LCD_string(0x80,"Too Big"); break;
                                 }
                                	 for(int i=1;i<=cursor;i++)
                                	 {
	                                	 tmp[i] = result[i];
                                	 }
                                	 
                                	 if(cursor == 1)
                                	 {
	                                	 result_check_flag = (tmp[1]==password[1]) ? 1 : 0;
                                	 }
                                	 else if(cursor == 2)
                                	 {
	                                	 result_check_flag = (tmp[1]==password[1] && tmp[2]==password[2]) ? 1 : 0;
                                	 }
                                	 else if(cursor == 3)
                                	 {
	                                	 result_check_flag = (tmp[1]==password[1] && tmp[2]==password[2] && tmp[3]==password[3]) ? 1 : 0;
                                	 }
                                	 else if(cursor == 4)
                                	 {
	                                	 result_check_flag = (tmp[1]==password[1] && tmp[2]==password[2] && tmp[3]==password[3] && tmp[4]==password[4]) ? 1 : 0;
                                	 }

                                	 if(result_check_flag){
	                                	  LCD_string(0x80,"    check complete");
	                                	  _delay_ms(70);
	                                	  LCD_string(0xc0,"    change password");
	                                	 _delay_ms(70);
	                                	 opcode = 0;
	                                	 mode=3;
										
	                                	 result_check_flag=0;
	                                	 pw_input_flag=0;
	                                	 cursor = 0;
	                                	 for(int i=0;i<6;i++)
	                                	 {
		                                	 result[i] = 999;
		                                	 tmp[i] = 999;
	                                	 }
	                                	 //     LCD_string(0xC0,"                ");
	                                	 //     LCD_command(0xCF);
                                	 }
                                	 else{
	                                	 LCD_string(0x80,"       fail... ");
	                                	 
	                                	 _delay_ms(200);
	                                	 LCD_command(LCD_Clear);
	                                	 _delay_ms(5);
	                                	 LCD_command(0xc0);
	                                	 opcode = 0;
	                                	 mode=2;
	                                	 result_check_flag=0;
	                                	 pw_input_flag=0;
	                                	 cursor = 0;
	                                	 for(int i=0;i<6;i++)
	                                	 {
		                                	 result[i] = 999;
		                                	 tmp[i] = 999;
	                                	 }
	                                	 //    LCD_command(0xCF);
                                	 }
								 
                                
                               
                               
                              }
                           }
						  else if(mode == 3)
						  {
							  if(trans_value==12){
								  opcode = 1;
								  
								  LCD_command(LCD_Clear);
								  _delay_ms(5);
								  LCD_command(0xc0);
								  
								  switch(cursor){
									  case 4 : LCD_data('0' + result[4]);LCD_command(Scr_Shift_Left);
									  case 3 : LCD_data('0' + result[3]);LCD_command(Scr_Shift_Left);
									  case 2 : LCD_data('0' + result[2]);LCD_command(Scr_Shift_Left);
									  case 1 : LCD_data('0' + result[1]);LCD_command(Scr_Shift_Left);break;
									  //  default: LCD_string(0x80,"Too Big"); break;
								  }
								  for(int i=1;i<=cursor;i++)
								  {
									  password[i] = result[i];
								  }
								  
								  LCD_string(0x80,"    change complete");
								   _delay_ms(200);
								   LCD_command(LCD_Clear);
								   _delay_ms(5);
								   LCD_command(0xc0);
								  // LCD_command(0xCF);
								  opcode = 0;
								  mode=0;
								  result_check_flag=0;
								  pw_input_flag=0;
								  cursor=0;
								  for(int i=0;i<6;i++)
								  {
									  result[i] = 999;
									  tmp[i] = 999;
								  }
							  }
						  }
                           
                        }
                        
                     }
                     
                  }
				  else
				  {
					  LCD_command(LCD_Clear);
					  _delay_ms(5);
					  LCD_command(0xC0);
					  
					  switch(cursor){
						  case 4 : LCD_data('0' + result[4]);LCD_command(Scr_Shift_Left);
						  case 3 : LCD_data('0' + result[3]);LCD_command(Scr_Shift_Left);
						  case 2 : LCD_data('0' + result[2]);LCD_command(Scr_Shift_Left);
						  case 1 : LCD_data('0' + result[1]);LCD_command(Scr_Shift_Left);break;
						  //    default: LCD_string(0x80,"Too Big"); break;
					  }
					  if(key_value==Key_scan())
					  {
						  trans_value = Key_trans(key_value);
						  
							  cursor=0;
							  pw_input_flag = 0;
						
					  }
				  }
            }
            
      }
      
	 
			   
   }
   return;
}
///settime을 초기에 설정해줘야한다.
void ds1302_init_settime()
{
	ds1302_setsec(10);
	ds1302_setmin(31);
	ds1302_sethour(10);
	ds1302_setdate(8);
	ds1302_setday(5);
	ds1302_setmonth(6);
	ds1302_setyear(18);
}

void ds1302_init_gettime()
{
	sec = ds1302_getsec();
	min = ds1302_getmin();
	hour = ds1302_gethour();
	date = ds1302_getdate();
	day = ds1302_getday(); //월화수목금토일
	month = ds1302_getmonth();
	year = ds1302_getyear();
}
void LCD_Time_print(void){
	if(mode==0){		
		LCD_string(0x80,"     /  /       ");
		LCD_string(0xC0,"      :  :      ");
		LCD_command(0x83);
		LCD_data('0'+year/10);
		LCD_data('0'+year%10);
		LCD_command(0x86);
		LCD_data('0'+month/10);
		LCD_data('0'+month%10);
		LCD_command(0x89);
		LCD_data('0'+date/10);
		LCD_data('0'+date%10);
		switch(day){
			case 1:LCD_string(0x8C,"Mon"); break;
			case 2:LCD_string(0x8C,"Thu"); break;
			case 3:LCD_string(0x8C,"Wen"); break;
			case 4:LCD_string(0x8C,"Thr"); break;
			case 5:LCD_string(0x8C,"Fri"); break;
			case 6:LCD_string(0x8C,"Sat"); break;
			case 7:LCD_string(0x8C,"Sun"); break;
		}
		LCD_command(0xC4);
		LCD_data('0'+hour/10);
		LCD_data('0'+hour%10);
		LCD_command(0xC7);
		LCD_data('0'+min/10);
		LCD_data('0'+min%10);
		LCD_command(0xCA);
		LCD_data('0'+sec/10);
		LCD_data('0'+sec%10);
	}
}

void Money_PIN_init(void){
	DDRA = 0b11100000;
	PORTA = 0b00000000;
}

void Calculator(void){
	if(PORTA == 0b00000010)
	{
		Money += 10;
	}
	else if(PORTA == 0b00000100)
	{
		Money += 50;
	}
	else if(PORTA == 0b00000110)
	{
		Money += 100;
	}
	else if(PORTA == 0b00001000)
	{
		Money += 500;
	}
	else if(PORTA == 0b00001010)
	{
		Money += 1000;
	}
	else if(PORTA == 0b00001100)
	{
		Money += 5000;
	}
	else if(PORTA == 0b00001110)
	{
		Money += 10000;
	}
	else if(PORTA == 0b00010000)
	{
		Money += 50000;
	}
}



