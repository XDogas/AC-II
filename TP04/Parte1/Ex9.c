#include <detpic32.h>

int main(void) {

    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

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

    LATDbits.LATD6 = 1;     // Select display high  // display high active
    LATDbits.LATD5 = 0;                             // display low inactive

    unsigned char value;
    char hexValue;

    while(1) {
        value = PORTB & 0x000F;                     // read dip-switch
        hexValue = display7Scodes[value];           // convert to 7 segments code
        LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)
    }
    return 0;
}
