#include <detpic32.h>
#include "delay.c"

int main(void) {

    LATE = LATE & 0xFFF0;
    TRISE = TRISE & 0xFFF0;

    while(1) {
        LATE += 0x0001;
        if(LATE > 15) LATE = 0x0000;
        delay(250);         // 4Hz -> 0.25s = 250 milliseconds
    }
    return 0;
}
