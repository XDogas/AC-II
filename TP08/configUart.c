#include <detpic32.h>

void configUart(unsigned int baudrate, char parity, unsigned int stopbits){
	
	//Valores a considerar por defeito
	
	if(baudrate <600 || baudrate >115200){
		baudrate=115200;
	}
	if(parity != 'N' && parity != 'E' && parity != 'O'){
		parity='N';
	}
	if(stopbits !=1 && stopbits !=2){
		stopbits=1;
	}
	
	//1-Configurar o gerador baudrate de acordo com a taxa de transmissão e receção pretendidas

	U1MODEbits.BRGH=0; //0 se for fator 16 e 1 se for fator 4
	U1BRG=((20000000 + 8*baudrate)/(16*baudrate))-1; //U1BRG=((fpclk + 8*baudrate)/(16*baudrate))-1
	
	//2-Configurar os parametros da trama: dimensão da palavra a transmitir(numero de data bits) e tipo de paridade
	
	if(parity == 'N'){//Tipo de paridade 'N'
		U1MODEbits.PDSEL=0;
	}
	else if(parity == 'E'){//Tipo de paridade 'E'
		U1MODEbits.PDSEL=1;
	}
	else if(parity == 'O'){//Tipo de paridade 'O'
		U1MODEbits.PDSEL=2;
	}
	U1MODEbits.STSEL=stopbits-1;//Numero de stop bits(se forem x stop bits fica x-1)
	
	//3-Ativar os modulos de transmissão e receção
	
	U1STAbits.UTXEN=1;//Ativa o modulo de transmissão
	U1STAbits.URXEN=1;//Ativa o modulo de rececão
	
	//4-Ativar a UART
	
	U1MODEbits.ON=1;//Ativa a UART
}
