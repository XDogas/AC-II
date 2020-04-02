#include <detpic32.h>

int main(void) {

    // Configure Timer T3 (2Hz with interrupts disabled)
    T3CONbits.TCKPS = 7;
    PR3 = 39062;
    TMR3 = 0;
    T3CONbits.TON = 1;

    while(1) {
        while(IFS0bits.T3IF == 0);  // Wait until T3IF = 1
        IFS0bits.T3IF = 0;          // Reset T3IF
        putChar('.');
    }

    return 0;
}
