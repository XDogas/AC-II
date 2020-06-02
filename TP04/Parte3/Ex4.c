#include <detpic32.h>
#include "send2displays_v2.c"
#include "delay.c"
#include "toBcd.c"

int main(void)
{
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    TRISB = TRISB & 0x00FF;

    TRISD = TRISD & 0xFF9F;

    LATD = (LATD & 0xFF9F) | 0x0060;    // 1111.1111.1001.1111 | 0000.0000.0110.0000

    char value = 0;
    int i;

    while(1)
    {
        i = 0;
        do
        {
            send2displays_v2(toBcd(value));
            delay(10);      // 100Hz
        } while(i++ < 20);  //diminuindo aumenta a velocidade do contador

        value++;
        value %= 60;

        LATBbits.LATB15 = 1;

        if(value % 2 == 0)
        {
            //LATDbits.LATD5=1;
			//LATDbits.LATD6=0;
            // ou
            LATD = (LATD & 0xFF9F) | 0x0020;    // 1111.1111.1001.1111 | 0000.0000.0010.0000
        }
        else
        {
            //LATDbits.LATD5=0;
			//LATDbits.LATD6=1;
            // ou
            LATD = (LATD & 0xFF9F) | 0x0040;    // 1111.1111.1001.1111 | 0000.0000.0100.0000
        }
        delay(20);  // sem este delay o ponto não é visivel

        if(value == 0)
        {
            int j;
            for(j=0; j<5; j++)  // cada ciclo dura 1s
            {
                LATD = (LATD & 0xFF9F) | 0x0060;
                LATBbits.LATB8 = 1; // a
                LATBbits.LATB9 = 1; // b
                LATBbits.LATB10 = 1;// c
                LATBbits.LATB11 = 1;// d
                LATBbits.LATB12 = 1;// e
                LATBbits.LATB13 = 1;// f
                LATBbits.LATB14 = 0;// g
                LATBbits.LATB15 = 0;// h (ponto)
                delay(500);      // 0.5s = 500ms
                LATD = (LATD & 0xFF9F) | 0x0000;
                delay(500);
            }
        }
    }

    return 0;
}
