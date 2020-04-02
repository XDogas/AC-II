#include <detpic32.h>
#include "delay.c"
#include "send2displays.c"
#include "toBcd.c"

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
			delay(10);//100hz
			send2displays(toBcd(value));
		}while(++i <20);//diminuindo aumenta a velocidade do contador
		
		value=value+1;
		value=value%60;//contador em modulo 60(so vai ate 59)	
	}
	return 0;
}
