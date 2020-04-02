#include <detpic32.h>
#include "delay.c"
int main(){
	
	LATE= LATE & 0XFFF0;//inicializa a 0
	TRISE= TRISE & 0XFFF0; //RE3 a RE0 configurados como saida(1ºS 4 A 0)
	
	
	while(1){
		LATE= LATE+0X0001;
		if(LATE>15){
			LATE=0X0000;
		}
		delay(500);  //4 hz //aumentar para aparecer + devagar
	}
	return 0;
}

