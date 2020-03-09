#include <detpic32.h>
#include "send2displays.c"
//#include "delay.c"  já esta feito o include na função send2displays()

int main(void) {

    //PORTB = PORTB & 0XFFF0;   // inicialize
    //TRISB = TRISB | 0x000F;   // configure RB0 to RB3 as inputs (bin:0000.0000.0000.1111 -> hex:0x000F)
    // ou em vez das 2 linhas anteriores:
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    LATB = LATB & 0x80FF;   // inicialize
    TRISB = TRISB & 0x80FF; // configure RB8 to RB14 as outputs (bin:1000.0000.1111.1111 -> hex:0x80FF)

    //TRISDbits.TRISD5 = 0;   // configure RD5 to RD6 as outputs
    //TRISDbits.TRISD6 = 0;
    // ou em vez das 2 linhas anteriores:
    TRISD = TRISD & 0xFF9F; //(bin:1111.1111.1001.1111 -> hex:0x80FF)

    LATDbits.LATD6 = 1;     // display high active
    LATDbits.LATD5 = 1;     // display low active

    char value = 0;

    while(1) {
        send2displays(value);
        delay(200); // 5hz -> 0,2s = 200ms
        value++;
    }
    return 0;
}
