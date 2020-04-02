#include <detpic32.h>
#include "delay.c"
int main(void){
	
	
	unsigned char segment;
	
	LATDbits.LATD6=0;//display high active
	LATDbits.LATD5=0;//display low active
	
	TRISB= TRISB & 0x00FF; //configura como saida (RB8-RB14)
	TRISDbits.TRISD5=0; //RD5 configured as output
	TRISDbits.TRISD6=0; //RD6 configured as output
	
	while(1){
		LATDbits.LATD6 =! LATDbits.LATD6;
		LATDbits.LATD5 =! LATDbits.LATD5;// toggle display selection
		segment=1;
		int i;
		for (i = 0; i < 7; i++)
		{
			LATB= (LATB & 0X0000) | segment;//send segment value to display
			LATB=LATB <<8;//
			delay(100);// 10hz-delay(100) 100hz-delay(10) 50hz-delay(50)
			segment=segment <<1;
		}
	}
	return 0;
}
