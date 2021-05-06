#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <mqueue.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

const int Photo_PIN = 7;
const int Photo_MPIN = 0;
const int Photo_TPIN = 2;

const int RFID_RST = 6;
const int RFID_SDA = 10;
const int RFID_MOSI = 12;
const int RFID_MISO = 13;
const int RFID_SCK = 14;


const int RFID_Success = 22;

const int MotorM_IN1_PIN = 8;
const int MotorM_IN2_PIN = 9;
const int MotorM_ENA_PIN = 3;

const int Money_PIN1 = 15;
const int Money_PIN2 = 16;
const int Money_PIN3 = 23;
const int Money_PIN4 = 4;

const int Door_Laser_PIN = 26;
const int Motor_Laser_PIN = 27;

const int Motion_PIN = 21;

const int Door_Servo_PIN = 24;
const int Door_State_AVR_PIN = 25;
const int Door_Switch_AVR_PIN = 11;

char motion_flag = 0;
char Door_state = 0;
char Door_Switch_state = 0;

double cap = 0.01;
double adj = 2.11;
double res = 0;
double i = 0;
double t = 0;

int RF_success_flag =0;


mqd_t mfd;

unsigned int Door_PhotoSensor(const int Photo_PIN)
{
   unsigned int Photo_count = 0; 

   pinMode(Photo_PIN, OUTPUT);
   digitalWrite(Photo_PIN, LOW);

   delay(1);

   pinMode(Photo_PIN, INPUT);

   while(digitalRead(Photo_PIN) == LOW) {
      Photo_count += 1; 
   } 
   return Photo_count;
}

double Motor_PhotoSensor(const int Photo_MPIN, const int Photo_TPIN)
{
   pinMode(Photo_MPIN,OUTPUT);
   pinMode(Photo_TPIN,OUTPUT);
   digitalWrite(Photo_MPIN,LOW);
   digitalWrite(Photo_TPIN,LOW);
   delay(1);
   pinMode(Photo_MPIN,INPUT);
   delay(1);
   digitalWrite(Photo_TPIN,HIGH);
   time_t starttime=0, endtime=0;
   
   starttime=clock();
   endtime=clock();
   
   while(digitalRead(Photo_MPIN)==LOW){
      
      endtime=clock();
      
   }
   
   double mesureresistance=endtime-starttime;
   
   res =(mesureresistance/cap)*adj;
   i++;
   t=0;
   t=t+res;
   if(i==10){
      t=t/i;
      i =0;
      t =0;
   }
   return t;
}

void Motor_init()
{
   pinMode(MotorM_IN1_PIN, OUTPUT);
   pinMode(MotorM_IN2_PIN, OUTPUT);
   pinMode(MotorM_ENA_PIN, PWM_OUTPUT);
   //pwmWrite(MotorM_ENA_PIN, 0, 255);
   digitalWrite(MotorM_IN1_PIN, LOW);
   digitalWrite(MotorM_IN2_PIN, LOW);
}

void Motor_on()
{
   pwmWrite(MotorM_ENA_PIN, 200);
   digitalWrite(MotorM_IN1_PIN, HIGH);
   digitalWrite(MotorM_IN2_PIN, LOW);
}

void Motor_off()
{
   digitalWrite(MotorM_IN1_PIN, LOW);
   digitalWrite(MotorM_IN2_PIN, LOW);
   pwmWrite(MotorM_ENA_PIN, 0);
}
/*
void Coin_PIN_setup()
{
   pinMode(Money_PIN1, INPUT);
   pinMode(Money_PIN2, INPUT);
   pinMode(Money_PIN3, INPUT);
   pinMode(Money_PIN4, INPUT);
}
*/
void Motion_Check()
{
   pinMode(Motion_PIN, INPUT);
   if(digitalRead(Motion_PIN))
   {
      motion_flag = 1;
     // printf("Motion Detected!\n");
      delay(1000);
   }
   else
   {
      motion_flag = 0;
      printf("Motion waiting\n");
      delay(1000);
   }
   delay(5);
}

void Door_Check()
{
   pinMode(Door_State_AVR_PIN, INPUT);
   Door_state = digitalRead(Door_State_AVR_PIN);
   printf("state_check : %d\n",Door_state);
}

void Door_Servo_on()
{
   pinMode(Door_Servo_PIN, PWM_OUTPUT);
   
   pwmSetClock(19);
   pwmSetMode(PWM_MODE_MS);
   
   pwmSetRange(20000);
   pwmWrite(Door_Servo_PIN, 1600);
   
   
   
	 
   
   
   
   
   
}

void Door_Servo_off()
{
   pinMode(Door_Servo_PIN, PWM_OUTPUT);
   
   pwmSetClock(19);
   pwmSetMode(PWM_MODE_MS);
   
   pwmSetRange(20000);
   pwmWrite(Door_Servo_PIN, 600);
}

void Door_Switch_Check()
{
   pinMode(Door_Switch_AVR_PIN, INPUT);
   Door_Switch_state = digitalRead(Door_Switch_AVR_PIN);
   printf("closed check : %d\n",Door_Switch_state);
}

/*
PI_THREAD(Coin_wait)
{
   const int Money_PIN1 = 15;
   const int Money_PIN2 = 16;
   const int Money_PIN3 = 23;
   const int Money_PIN4 = 4;
   const int Coin_Servo_PIN = 1;
   
   void Coin_PIN_setup()
   {
      pinMode(Money_PIN1, INPUT);
      pinMode(Money_PIN2, INPUT);
      pinMode(Money_PIN3, INPUT);
      pinMode(Money_PIN4, INPUT);
   }

   void Coin_Servo()
   {
      pinMode(Coin_Servo_PIN, PWM_OUTPUT);
      
      pwmSetClock(19);
      pwmSetMode(PWM_MODE_MS);
      
      pwmSetRange(20000);
	  delay(3000);
      pwmWrite(Coin_Servo_PIN, 1600);
      delay(1500);
      pwmWrite(Coin_Servo_PIN, 800);
   }

   char Result1, Result2, Result3, Result4 = 0;
   
   while(1){
	   Coin_PIN_setup();

      Result1 = digitalRead(Money_PIN1);
      Result2 = digitalRead(Money_PIN2);
      Result3 = digitalRead(Money_PIN3);
      Result4 = digitalRead(Money_PIN4);

	  delay(300);
printf("%d %d %d %d\n",Result1,Result2,Result3,Result4);
      //조건문에서 서보동작만 코딩요구. digitalRead, digitalWrite 필요없음.
      if(Result1 == 1 && Result2 == 0 && Result3 == 0 && Result4 == 0) // 0001
      {
         Coin_Servo();
      }
      else if(Result1 == 0 && Result2 == 1 && Result3 == 0 && Result4 == 0) // 0010
      { 
         Coin_Servo();
      }
      else if(Result1 == 0 && Result2 == 0 && Result3 == 1&& Result4 == 0) // 0011
      { 
         Coin_Servo();
      }
      else if(Result1 == 0 && Result2 == 0 && Result3 == 0 && Result4 == 1) // 0100
      { 
         
		// pinMode(Money_PIN1, OUTPUT);
       //  pinMode(Money_PIN2, OUTPUT);
        // pinMode(Money_PIN3, OUTPUT);
       //  pinMode(Money_PIN4, OUTPUT);
       //  digitalWrite(Money_PIN1, LOW);
      //   digitalWrite(Money_PIN2, HIGH);
      //   digitalWrite(Money_PIN3, LOW);
      //   digitalWrite(Money_PIN4, LOW);
       //  delay(10);
		 
         Coin_Servo();
      }
      Result1 = 0;
      Result2 = 0;
      Result3 = 0;
      Result4 = 0;
   }
}
*/

PI_THREAD(Ultra_wait)
{
   const int Ultra_TRIG = 28;
   const int Ultra_ECHO = 29;
   const int Ultra_AVR = 5;

   int Ultra_Bill_Case_getCM()
   {
      //Send trig pulse
      digitalWrite(Ultra_TRIG, HIGH);
      delayMicroseconds(20);
      digitalWrite(Ultra_TRIG, LOW);

      //Wait for echo start
      while(digitalRead(Ultra_ECHO) == LOW);

      //Wait for echo end
      long startTime = micros();
      while(digitalRead(Ultra_ECHO) == HIGH);
      long travelTime = micros() - startTime;

      //Get distance in cm
      int distance = travelTime / 58;

      return distance;
   }
   
   int Ultra_result = 0;
   
   pinMode(Ultra_TRIG, OUTPUT);
   pinMode(Ultra_ECHO, INPUT);
   while(1)
   {
      digitalWrite(Ultra_TRIG, LOW);
	  delay(1000);
      Ultra_result = Ultra_Bill_Case_getCM();
      //printf("Distance : %dcm\n", Ultra_result);
      if(Ultra_result >= 12)
      {
         digitalWrite(Ultra_AVR, HIGH); // AVR LCD Money Reset
         delay(50);
      }
      digitalWrite(Ultra_AVR, LOW);
   }
}

void main()
{
   int Door_Photo_result = 0;
   double Motor_Photo_result = 0;
   
   double gap;
   time_t startTime=0, endTime=0;
   
   wiringPiSetup();
   
   pwmSetMode(PWM_MODE_MS);
   pwmSetRange(20000);
   
  // piThreadCreate(Coin_wait);
   piThreadCreate(Ultra_wait);
   pinMode(Door_Laser_PIN, OUTPUT);
   digitalWrite(Door_Laser_PIN, LOW);
   //digitalWrite(Door_Laser_PIN, HIGH);
   pinMode(Motor_Laser_PIN,OUTPUT);
    digitalWrite(Motor_Laser_PIN, HIGH);
    Door_Servo_off(); // 폐문
   Motor_off();
   
   while(1)
   {
     /*
	 pinMode(Motor_Laser_PIN,OUTPUT);
      digitalWrite(Motor_Laser_PIN, HIGH);
	  */
      Motion_Check();
      Door_Check();
	  
	 // Door_Switch_Check();///지울것
	 
      if(Door_state == 1) // 인증에 따른 개방신호
      {	
		  printf("check %d\n");
		  delay(200);
		
         while(1)
         {
			pinMode(RFID_Success,INPUT);
			
		 RF_success_flag =	digitalRead(RFID_Success);
			printf("check_RF_SUC %d\n ",RF_success_flag);
			delay(500);
			if(RF_success_flag==0){
				 Door_Servo_on(); // 문 개방
				delay(3000);
				Door_Switch_Check(); 
				
				 if(Door_Switch_state == 1) // 문 닫힘 확인신호
				{
				   delay(200);
				   Door_Servo_off(); // 폐문
				   Door_state = 0;
				   break;
				}
				
				
			}
          
		  RF_success_flag =0; 
		   
         }
      }
      
      if(motion_flag == 1)
      {
         digitalWrite(Door_Laser_PIN, HIGH);
         Door_Photo_result = 0;
         Door_Photo_result = Door_PhotoSensor(Photo_PIN);
         delay(200);
         printf("%d\n", Door_Photo_result);

         if(Door_Photo_result >= 2500) //입구 가려질 때
         {
            Motor_init();
			/*
              pinMode(Motor_Laser_PIN,OUTPUT);
				digitalWrite(Motor_Laser_PIN, HIGH);
           */
            Motor_on();
			Motor_Photo_result=0;
			Motor_Photo_result = Motor_PhotoSensor(Photo_MPIN, Photo_TPIN);
            printf("first : %lf\n", Motor_Photo_result);    ////

            //if(Motor_Photo_result >= 4000 && Door_Photo_result >= 2500)
           

			if(Motor_Photo_result >= 4000)  //처음 가려짐
				   //둘다가리고 무조건 지폐 들어감
			{
				   startTime=clock();
				   
				   
				   while(1)
				   {
					   Motor_Photo_result = Motor_PhotoSensor(Photo_MPIN, Photo_TPIN);
					   printf("second : %lf\n", Motor_Photo_result);
					   if(Motor_Photo_result < 3000)
					   {
						   break;
					   }
				   }

					  endTime=clock();
					  delay(200);
					  Motor_off();
					  digitalWrite(Motor_Laser_PIN, LOW);
					  digitalWrite(Door_Laser_PIN, LOW);
					  
					  gap =(float)(endTime-startTime)/1000;
					   printf("gap: %f\n", gap);
					   
					   if(gap >= 5 && gap <= 15) // 1001
					   {
						  digitalWrite(Money_PIN1, HIGH);
						  digitalWrite(Money_PIN2, LOW);
						  digitalWrite(Money_PIN3, LOW);
						  digitalWrite(Money_PIN4, HIGH);
					   }
					   else if(gap >= 7000 && gap <= 9500) // 0011
					   {
						  digitalWrite(Money_PIN1, LOW);
						  digitalWrite(Money_PIN2, LOW);
						  digitalWrite(Money_PIN3, HIGH);
						  digitalWrite(Money_PIN4, HIGH);
					   }
					   else if(gap >= 9500 && gap <= 11000) // 1010
					   {
						  digitalWrite(Money_PIN1, HIGH);
						  digitalWrite(Money_PIN2, LOW);
						  digitalWrite(Money_PIN3, HIGH);
						  digitalWrite(Money_PIN4, LOW);
					   }
					   else if(gap >= 51000 && gap <= 52000) // 1011
					   {
						  digitalWrite(Money_PIN1, HIGH);
						  digitalWrite(Money_PIN2, LOW);
						  digitalWrite(Money_PIN3, HIGH);
						  digitalWrite(Money_PIN4, HIGH);
					   }
					   delay(30);
					   digitalWrite(Money_PIN1, LOW);
					   digitalWrite(Money_PIN2, LOW);
					   digitalWrite(Money_PIN3, LOW);
					   digitalWrite(Money_PIN4, LOW);
					   digitalWrite(Motor_Laser_PIN, LOW);
					}
			}
         }
         motion_flag = 0;
      }
	
}

//수요일 서보동작 해결
// 동전배선확인 및 동전 분류기기구 확인
//문 스위치
//RFID