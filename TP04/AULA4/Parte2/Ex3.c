#include <detpic32.h>
#include "delay.c"
#include "send2displays_2.c"

int main(void){
	
	PORTB=PORTB & 0XFFF0;//inicializa 
	TRISB= TRISB | 0X000F; //configure RB0 to RB3 as inputs
	
	LATB=LATB & 0X00FF;//inicializa 
	TRISB= TRISB & 0X00FF; //configure RB8 to RB14 as outputs
	
	LATDbits.LATD5=0;//inicializa
	LATDbits.LATD6=1;//inicializa
	
	TRISDbits.TRISD5=0; //RD5 configured as output
	TRISDbits.TRISD6=0; //RD6 configured as output
	
	char value=0;
	int i;
	while(1){
		i=0;
		do{
			delay(50);
			send2displays_2(value);
		}while(++i <4);
		
		value=value+1;
		value=value%256;
	}
	return 0;	
}
