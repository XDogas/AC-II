#include <detpic32.h>
#include "delay.c"
int main(void){
	
	static const char display7Scodes[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
	
	PORTB=PORTB & 0XFFF0;//inicializa 
	TRISB= TRISB | 0X000F; //configure RB0 to RB3 as inputs
	
	LATB=LATB & 0X00FF;//inicializa 
	TRISB= TRISB & 0X00FF; //configure RB8 to RB14 as outputs
	
	LATDbits.LATD5=0;//inicializa a 0 logo esta inativo
	LATDbits.LATD6=1;//inicializa a 1 logo esta ativo
	
	TRISDbits.TRISD5=0; //RD5 configured as output
	TRISDbits.TRISD6=0; //RD6 configured as output
	
	unsigned char value;
	//char convert;
	while(1){
		
		value= PORTB & 0X000F; //read dip-switch
		LATB=(LATB & 0X0000) | (display7Scodes[value] <<8);// send to display
				// or 
		//value= PORTB & 0X000F; //read dip-switch
		//convert= display7Scodes[value];//convert to 7 segments code
		//LATB=(LATB & 0X0000) | ((int)convert <<8);// send to display
	}
	return 0;
}
