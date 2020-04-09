#include <detpic32.h>
#include "delay.c"
#include "send2displays_v2.c"
#include "toBcd.c"

void configAll();
void configInterruptSystem();

volatile unsigned char voltage = 0; // Global variable

int main(void) {

    TRISB = (TRISB & 0x00FF) | 0x000F;
    TISD = TRISD & 0xFF9F;

    configAll();                // Configure all (digital I/O, analog input, A/D module)
    configInterruptSystem();    // Configure interrupt system

    AD1CON1bits.ASAM = 1;       // Start A/D conversion

    unsigned int cnt = 0;
    while(1){                                       // Ciclo infinito
        if(cnt++ % 25 == 0) AD1CON1bits.ASAM = 1;   // Start conversion (250ms(4 samples/second))
        send2displays_v2(toBcd(voltage & 0xFF));    // Send voltage variable to displays
        delay(10);                                  // wait 10ms
    }
        // OU
    //while(1){                                       // Ciclo infinito
    //    if(cnt % 25 == 0) AD1CON1bits.ASAM = 1;     // Start conversion (250ms(4 samples/second))
    //    send2displays_v2(toBcd(voltage));           // Send voltage variable to displays
    //    cnt++;
    //    delay(10);                                  //wait 10ms
    //}

	return 0;
}

void _int_(27) isr_adc(void) {

    int soma = 0;
    int *p = (int *)(&ADC1BUF0);
    for(; p < (int *)(&ADC1BUF8); p+=4) soma += *p;
    double media = (double) soma / 8.0;
    voltage = (char) ((media*33)/1023);
    IFS1bits.AD1IF = 0;         // Reset
}

void configAll() {

    TRISBbits.TRISB4 = 1;      //Desliga a componente digital de saida do porto RB4
	AD1PCFGbits.PCFG4 = 0;     //Configura o porto RB4 como entrada analogica(AN4)
	AD1CON1bits.SSRC = 7;      //Acionador da conversao dos bits de selecao
	AD1CON1bits.CLRASAM = 1;   //Termina a conversao quando o 1º interruptor A/D é gerado
	AD1CON3bits.SAMC = 16;     //Amostragem do tempo é 16TAD(TAD=100ns)
	AD1CON2bits.SMPI = 1-1;    //Interrupcao e gerada apos 1 amostra
	AD1CHSbits.CH0SA = 4;      //Entrada AN4
	AD1CON1bits.ON = 1;        //Inicia o conversor A/D
}

void configInterruptSystem() {

    IPC6bits.AD1IP=2;//	Configura a prioridade da interrupcao (A/D interrupts priority)
	IEC1bits.AD1IE=1;// Ativa a interrupcao (A/D interrupts enabled)
	IFS1bits.AD1IF=0;// Clear interrupt flag (clear A/D flag)

    EnableInterrupts();         // Global Interrupt Enable
}
