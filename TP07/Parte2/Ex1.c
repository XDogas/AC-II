#include <detpic32.h>

void main(void) {
    // Config T3 (100Hz), no interrupts
    T3CONbits.TCKPS = 2;
    PR3 = 49999;
    TMR3 = 0;           // Reset timer T3 count register
    T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timer configuration sequence)

    // PWM configs
    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 1;
    OC1RS = 12500;          // OC1RS = ((49999 + 1) * 25)/100
    OC1CONbits.ON = 1;
}
