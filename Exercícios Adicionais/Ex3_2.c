#include <detpic32.h>

void delay(unsigned int ms);

int main(void)
{
    TRISE = TRISE & 0xFFF0; // outputs: RE3 a RE0 (1111.1111.1111.0000)

    //LATE = LATE & 0x0000;   // ou so LATE = 0x000 (bão sei qual é a forma mais correta) ou entao nao se mete

    char key;

    while(1)
    {
        key = getChar();

        if(key == '0')
        {
            LATE = 0x0001;  // 0001 (just RE0)
        }
        else if(key == '1')
        {
            LATE = 0x0002;  // 0010 (just RE1)
        }
        else if(key == '2')
        {
            LATE = 0x0004;  // 0100 (just RE2)
        }
        else if(key == '3')
        {
            LATE = 0x0008;  // 1000 (just RE3)
        }
        else
        {
            LATE = 0x000F;  // 1111 (all leds)
            delay(1000);    // wait 1s = 1000ms
            LATE = 0x0000;  // 0000 (turn off all leds)
        }
    }

    return 0;
}

void delay(unsigned int ms)
{
    for(; ms>0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}
