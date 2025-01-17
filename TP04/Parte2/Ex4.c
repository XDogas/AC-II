#include <detpic32.h>
#include "send2displays_v2.c"
#include "delay.c"

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
    // ou em vez das 2 linhas anteriores:
    //LATD = (LATD & 0xFF9F) | 0x0060;    // 1111.1111.1001.1111 | 0000.0000.0110.0000

    char counter = 0;
    int i;

    while(1) {
        i = 0;
        do {
            //delay(20);// 50 Hz (20 ms) low flicker
                        // or                           (comentar 1 dos delays)
            delay(10);  // 100 Hz (10 ms) no flicker
            send2displays_v2(counter);
        } while(++i < 4);

        counter++;
        counter %= 256;
    }
    return 0;
}
