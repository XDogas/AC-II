#include <detpic32.h>
#include "delay.c"

int main(void) {

    unsigned char segment;

    LATDbits.LATD6 = 1;     // display high active
    LATDbits.LATD5 = 0;     // display low inactive

    TRISB= TRISB & 0x80FF;  // configure RB8-RB14 as outputs (bin:1000.0000.1111.1111 -> hex:0x80FF)

    TRISDbits.TRISD5 = 0;   // configure RD5 as output (output -> 0 ; input -> 1)
    TRISDbits.TRISD6 = 0;   // configure RD6 as output

    while(1) {

        LATDbits.LATD6 = !LATDbits.LATD6; //
        LATDbits.LATD5 = !LATDbits.LATD5; // toggle display selection

        segment = 1;

        int i;
        for(i=0; i<7; i++) {
            LATB = (LATB & 0X0000) | segment << 8;  // send "segment" value to display
            delay(500);                             // wait 0.5 second = 500 milliseconds
            segment <<= 1;
        }
    }

    return 0;
}
