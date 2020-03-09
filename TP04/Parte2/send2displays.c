#include <detpic32.h>
#include "delay.c"

void send2displays(unsigned char value) {

    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;
    char hexValue;

    // send digit_low (dl) to display_low: dl = value & 0x0F
    LATDbits.LATD6 = 0;     // display high inactive
    LATDbits.LATD5 = 1;     // display low active
    hexValue = display7Scodes[dl];   // convert to 7 segments code
    LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)
    delay(10);

    // send digit_high (dh) to display_high: dh = value >> 4
    LATDbits.LATD6 = 1;     // display high active
    LATDbits.LATD5 = 0;     // display low inactive
    hexValue = display7Scodes[dh];   // convert to 7 segments code
    LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)
    delay(10);
}
