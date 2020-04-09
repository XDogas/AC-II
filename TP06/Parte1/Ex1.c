#include <detpic32.h>
#include "delay.c"

void configAll();
void configInterruptSystem();

int main(void) {

    configAll();                // Configure all (digital I/O, analog input, A/D module)
    configInterruptSystem();    // Configure interrupt system

    AD1CON1bits.ASAM = 1;       // Start A/D conversion

    while(1) {}// all activity is done by the ISR
    return 0;
}

// Interrupt Handler
void _int_(27) isr_adc(void) {  // Replace VECTOR by the A/D vector number - see "PIC32 family data sheet" (pages 74-76)

    printStr("ADC1BUF0 value : ");  // Print ADC1BUF0 value
    printInt(ADC1BUF0,16 | 3 << 16);// Hexadecimal (3 digits format)
    printStr("\n");

    AD1CON1bits.ASAM = 1;           // Start A/D conversion
    IFS1bits.AD1IF = 0;             // Reset AD1IF flag

    delay(500);                     // used to make debugging easier
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
