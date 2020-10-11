#include <detpic32.h>

int main(void)
{
    TRISB = (TRISB & 0xFFF0) | 0x000F;  // inputs: RB3 a RB0 (1111.1111.1111.0000 | 0000.0000.0000.1111)

    TRISE = TRISE & 0xFFF0;             // outputs: RE3 a RE0 (1111.1111.1111.0000)

    unsigned char value;

    while(1)
    {
        value = PORTB & 0x000F;         // read dip-switch
        LATE = value;                   // a)

        // Falta alinea b)
    }

    return 0;
}
