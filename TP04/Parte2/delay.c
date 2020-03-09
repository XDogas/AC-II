#include<detpic32.h>

void delay(unsigned int ms) {

    for(; ms>0; ms--) {
        resetCoreTimer();
        while(readCoreTimer() < 20000); // Freq(Hz) = 1/Period(seconds)
    }
}
