// Ler o valor do switch e pôr no display menos significativo

#include <detpic32.h>

int main(void)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    TRISB = (TRISB & 0xFFF0) | 0x000F;

    TRISB = TRISB & 0x80FF;

    TRISD = TRISD & 0xFF9F;
    LATD = (LATD & 0xFF9F) | 0x0020;

    unsigned char value;
    char hexValue;

    while(1)
    {
        value = PORTB & 0x000F;
        hexValue = display7Scodes[value];
        LATB = (LATB & 0x00FF) | (hexValue << 8);
    }

    return 0;
}
