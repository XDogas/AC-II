#include <detpic32.h>
void send2displays_1(unsigned char value){
	
	static const char display7Scodes[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
	//char convert;
	unsigned char digit_high= value >>4;//display da esquerda(f)
	unsigned char digit_low= value & 0x0f;//display da direita(0)
	
	//low 
	LATDbits.LATD5=1;//ativa o display da direita
	LATDbits.LATD6=0;
	
	LATB=(LATB & 0X00ff) | (display7Scodes[digit_low] <<8);// send to display
	//or
	//convert= display7Scodes[digit_low];//convert to 7 segments code
	//LATB=(LATB & 0X00ff) | ((int)convert <<8);// send to display
	delay(10);
	
	//high
	LATDbits.LATD5=0;
	LATDbits.LATD6=1;//ativa o display da esquerda
	LATB=(LATB & 0X00ff) | (display7Scodes[digit_high] <<8);// send to display
	//or
	//convert= display7Scodes[digit_high];//convert to 7 segments code
	//LATB=(LATB & 0X00ff )| ((int)convert <<8);// send to display
	delay(10);
}
