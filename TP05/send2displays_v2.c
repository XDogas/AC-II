#include <detpic32.h>

void send2displays_v2(unsigned char value) {

    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0; // static variable: doesn't loose its
    // value between calls to function
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;
    char hexValue;

    if(displayFlag) {   // if "displayFlag" is 0 then send digit_low to display_low
        LATDbits.LATD6 = 0;     // display high inactive
        LATDbits.LATD5 = 1;     // display low active
        hexValue = display7Scodes[dl];   // convert to 7 segments code
        LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)

    } else {            // else send digit_high to didplay_high
        LATDbits.LATD6 = 1;     // display high active
        LATDbits.LATD5 = 0;     // display low inactive
        hexValue = display7Scodes[dh];   // convert to 7 segments code
        LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)
    }

    displayFlag = !displayFlag;  // toggle "displayFlag" variable
}
