#include <detpic32.h>

void configTimerT3();

void main(void) {

    configTimerT3();    // Configure Timer T3 with interrupts enabled
    while(1);
}

void _int_(12) isr_T3(void) {   // Replace VECTOR by the timer T3
    putChar('.');
    IFS0bits.T3IF = 0;// Reset T3 interrupt flag
}

void configTimerT3() {
     // Config T3 (2Hz)
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;           // Reset timer T3 count register

    //Ativa interrupcoes
	IPC3bits.T3IP=2;   //Definir a prioridade da interrupcao
	IEC0bits.T3IE=1;   //Ativa a interrupcao do Timer T3
	IFS0bits.T3IF=0;   //Reset da interrupcao

    EnableInterrupts();

    T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timer configuration sequence)
}
