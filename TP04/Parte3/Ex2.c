#include <detpic32.h>
#include "send2displays_v2.c"
#include "delay.c"
#include "toBcd.c"

int main(void)
{
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    TRISB = TRISB & 0x90FF;

    TRISD = TRISD & 0xFF9F;

    LATD = (LATD & 0xFF9F) | 0x0060;    // 1111.1111.1001.1111 | 0000.0000.0110.0000

    char value = 0;
    int i;

    /*while(1)
    {
        i = 0;
        do
        {
            send2displays_v2(toBcd(value));
            delay(10);      // 100Hz
        } while(i++ < 20);  //diminuindo aumenta a velocidade do contador

        value++;
        value %= 60;
    }*/

    i = 0;
    i=0;
    while(1)
    {
        i++;
        send2displays_v2(toBcd(value));
        delay(10);

        if(i % 20 == 0) value++;    //diminuindo aumenta a velocidade do contador
    }

    return 0;
}
