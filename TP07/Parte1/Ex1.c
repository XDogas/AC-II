#include <detpic32.h>

void configTimerT3();

void main(void) {

    configTimerT3();                // Configure Timer T3 (2Hz with interrupts disabled)

    while(1) {
        while(IFS0bits.T3IF == 0);  // Wait until T3IF = 1
        IFS0bits.T3IF = 0;          // Reset T3IF
        putChar('.');
    }
}

void configTimerT3() {
    // Config T3 (2Hz), no interrupts
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;           // Reset timer T3 count register
    T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timer configuration sequence)
}
