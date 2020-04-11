#include <detpic32.h>
#include "send2displays_v2.c"
#include "toBcd.c"

void configAll();

volatile int voltage = 0;   // Global variable

void main(void) {



    configAll();                // Configure all (digital I/O, analog input, A/D module, timers T1 and T3, interrupts)

    TRISD = TRISD & 0xFF9F;
    LATD = LATD & 0xF9FF;

    TRISB = TRISB & 0x80FF;
    LATB = LATB & 0x80FF;

    while(1);
}

void _int_(4) isr_T1(void) {

    AD1CON1bits.ASAM = 1;   // Start A/D conversion
    IFS0bits.T1IF = 0;      // Reset T1IF flag
}

void _int_(12) isr_T3(void) {

    send2displays_v2(toBcd(voltage & 0xFF));   // Send "voltage" global variable to displays
    IFS0bits.T3IF = 0;                      // Reset T3IF flag
}

void _int_(27) isr_adc(void) {

    int soma = 0;
    int *p = (int *)(&ADC1BUF0);
    for(; p < (int *)(&ADC1BUF8); p+=4) soma += *p;
    double media = (double) soma / 8.0;
    voltage = (char) ((media*33)/1023);
    IFS1bits.AD1IF = 0;     // Reset AD1IF flag
}

void configAll() {

    TRISBbits.TRISB4 = 1;      //Desliga a componente digital de saida do porto RB4
	AD1PCFGbits.PCFG4 = 0;     //Configura o porto RB4 como entrada analogica(AN4)
	AD1CON1bits.SSRC = 7;      //Acionador da conversao dos bits de selecao
	AD1CON1bits.CLRASAM = 1;   //Termina a conversao quando o 1º interruptor A/D é gerado
	AD1CON3bits.SAMC = 16;     //Amostragem do tempo é 16TAD(TAD=100ns)
	AD1CON2bits.SMPI = 8-1;    //Interrupcao e gerada apos 8 amostras(samples)
	AD1CHSbits.CH0SA = 4;      //Entrada AN4
	AD1CON1bits.ON = 1;        //Inicia o conversor A/D

    // Config Timer T1 (4Hz)
    T1CONbits.TCKPS = 3;
    PR1 = 19530;
    TMR1 = 0;                   // Reset timer T1 count register
    //Ativa interrupcoes
    IPC1bits.T1IP = 2;          // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;          // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;          // Reset timer T1 interrupt flag

    // Config Timer T1 (100 Hz)
    T3CONbits.TCKPS = 5;
    PR3 = 49999;
    TMR3 = 0;                   // Reset timer T3 count register
    //Ativa interrupcoes
    IPC3bits.T3IP=2;            //Definir a prioridade da interrupcao
    IEC0bits.T3IE=1;            //Ativa a interrupcao do Timer T3
    IFS0bits.T3IF=0;            //Reset da interrupcao

    EnableInterrupts();         // Global Interrupt Enable
    T1CONbits.TON = 1;          // Enable timer T1 (must be the last command of the timer configuration sequence)
    T3CONbits.TON = 1;          // Enable timer T3 (must be the last command of the timer configuration sequence)
}
