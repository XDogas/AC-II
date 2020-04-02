#include <detpic32.h>
#include "delay.c"
int main(){

	LATB=LATB & 0X00FF;//inicializa 
	TRISB= TRISB & 0x00FF; //configura como saida (8 ultimos sao ativados)
	
	LATDbits.LATD5=0;//inicializa
	LATDbits.LATD6=0;//inicializa
	
	TRISDbits.TRISD5=0; //RD5 configured as output
	TRISDbits.TRISD6=0; //RD6 configured as output
	
	LATDbits.LATD5=1;//atribui valor
	LATDbits.LATD6=0;//atribui valor
	
	
	char c;
	while(1){
		c=getChar();
		
		if(c=='a' || c=='A'){
			LATBbits.LATB8=1;//ativa 
			delay(250);//passado x segundos(aumentar para demorar + tempo)
			LATBbits.LATB8=0;//desativa
		}
		else if(c=='b' || c=='B'){
			LATBbits.LATB9=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB9=0;//desativa
		}
		else if(c=='c' || c=='C'){
			LATBbits.LATB10=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB10=0;//desativa
		}
		else if(c=='d' || c=='D'){
			LATBbits.LATB11=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB11=0;//desativa
		}
		else if(c=='e' || c=='E'){
			LATBbits.LATB12=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB12=0;//desativa
		}
		else if(c=='f' || c=='F'){
			LATBbits.LATB13=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB13=0;//desativa
		}
		else if(c=='g' || c=='G'){
			LATBbits.LATB14=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB14=0;//desativa
		}
		else if(c=='.'){
			LATBbits.LATB15=1;//ativa 
			delay(250);//passado x segundos
			LATBbits.LATB15=0;//desativa
		}
	}
}
