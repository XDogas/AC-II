#include <detpic32.h>
#include "delay.c"

int main(void) {
    LATDbits.LATD0 = 0;     // The initial value should be set
                            // before configuring the port as output
    TRISDbits.TRISD0 = 0;   // RD0 configured as output
    while(1) {
        delay(500);         // half period = 0.5s = 500 milliseconds
        LATDbits.LATD0 = !LATDbits.LATD0;
    }
    return 0;
}
