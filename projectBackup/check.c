#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <mqueue.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>



const int RFID_Success = 22;

int rfid_flag=0;




void main(){
		
		wiringPiSetup();
	pinMode(RFID_Success,INPUT);
		
		
	while(1){
		
	
	rfid_flag =digitalRead(RFID_Success);
	printf("check : %d\n",rfid_flag);
	 delay(300);
	 rfid_flag =0;
	}
	
	
}