#include <detpic32.h>

void configTimerT3();
void configPWM();
void setPWM(unsigned int);

int main(void) {

    configTimerT3();    // Configure Timer T3 (100Hz with interrupts enabled)
    configPWM();

    while(1);
}

void configTimerT3() {
    // Config T3 (100Hz)
    T3CONbits.TCKPS = 2;
    PR3 = 49999;
    TMR3 = 0;           // Reset timer T3 count register

    //Ativa interrupcoes
	IPC3bits.T3IP=2;   // Definir a prioridade da interrupcao
	IEC0bits.T3IE=1;   // Ativa a interrupcao do Timer T3
	IFS0bits.T3IF=0;   // Reset da interrupcao

    EnableInterrupts();

    T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timer configuration sequence)
}

void configPWM() {
    // PWM configs
    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 3   // Use Timer T3 as the time base for PWM(Pulse With Modulation) generation;
    setPWM(10);             // Testar com diferentes valores de duty-cycle (10%, 65% e 80%)
    OC1CONbits.ON = 1;      // Enable OC3 module
}

void _int_(12) isr_T3(void) {
	IFS0bits.T3IF=0;   // reset T3IF
}

void setPWM(unsigned int dutyCycle) {

    if(dutyCycle >= 0 && dutyCycle <= 100) {        // duty_cycle must be in the range [0, 100]
        OC1RS = ((PRx + 1) * dutyCycle) / 100;      // Evaluate OC1RS as a function of "dutyCycle"; alterar x pelo Timer
    } else printStr("Valor de duty-cycle invalido");
}
