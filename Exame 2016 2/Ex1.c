// Ler o valor do switch e pôr nos leds

#include <detpic32.h>

int main(void)
{
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    TRISE = TRISE & 0xFFF0;

    unsigned char value;

    while(1)
    {
        value = PORTB & 0x000F;
        LATE = value;
    }

    return 0;
}
