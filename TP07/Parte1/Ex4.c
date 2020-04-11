#include <detpic32.h>

void configTimerT1();
void configTimerT3();

volatile int syscallFlag = 1;

void main(void) {
    configTimerT1();    // Configure Timer T1 with interrupts enabled
    configTimerT3();    // Configure Timer T3 with interrupts enabled
    while(1);
}

void _int_(4) isr_T1(void) {

    putChar('1');       // print character '1'
    IFS0bits.T1IF = 0;  // Reset T1IF flag
}

void _int_(12) isr_T3(void) {

    putChar('3');       // print character '3'
    IFS0bits.T3IF = 0;  // Reset T3IF flag
}

void configTimerT1() {  // Config Timer T1 (2 Hz)

    T1CONbits.TCKPS = 3;
    PR1 = 39062;
    TMR1 = 0;           // Reset timer T1 count register

    //Ativa interrupcoes
    IPC1bits.T1IP = 2;  // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;  // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;  // Reset timer T1 interrupt flag

    EnableInterrupts(); // Global Interrupt Enable

    T1CONbits.TON = 1;      // Enable timer T1 (must be the last command of the timer configuration sequence)

}

void configTimerT3() {  // Config Timer T3 (1O Hz)

    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;           // Reset timer T3 count register

    //Ativa interrupcoes
	IPC3bits.T3IP=2;   //Definir a prioridade da interrupcao
	IEC0bits.T3IE=1;   //Ativa a interrupcao do Timer T3
	IFS0bits.T3IF=0;   //Reset da interrupcao

    EnableInterrupts(); // Global Interrupt Enable

    T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timer configuration sequence)
}
