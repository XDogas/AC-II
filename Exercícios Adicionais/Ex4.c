#include <detpic32.h>

void delay(unsigned int ms);
void send2displays(unsigned char value);
void send2displays_v1(unsigned char value);

int main(void)
{
    TRISE = TRISE & 0xFFF0;             // outputs: RE3 a RE0 (1111.1111.1111.0000)

    LATE = LATE & 0x0000;               // ou so LATE = 0x000 (bão sei qual é a forma mais correta)

    //TRISB = (TRISB & 0xFFF0) | 0x000F;  // inputs: RB0 to RB3 (bin:0000.0000.0000.1111 -> hex:0x000F)

    TRISB = TRISB & 0x80FF;             // outputs: RB8 to RB14 (bin:1000.0000.1111.1111 -> hex:0x80FF)

    TRISD = TRISD & 0xFF9F;             //outputs: RD5 and RD6 (bin:1111.1111.1001.1111 -> hex:0xFF9F)
    LATD = (LATD & 0xFF9F) | 0x0060;    // displays low and high active (1111.1111.1001.1111 | 0000.0000.0110.0000)

    char key;
    int i;

    while(1)
    {
        key = getChar();

        if(key == '0')
        {
            LATE = 0x0001;  // 0001 (just RE0)
            i = 0;
            do // com o do while(key == '0') a key vai ser sempre 0 ou seja é um ciclo infinito
            {
                send2displays(0);
                delay(10);          // 100Hz (no flick)
            } while(++i < 100); // mostra dueante 1s (10 * 100 = 1000ms)
        }
        else if(key == '1')
        {
            LATE = 0x0002;  // 0010 (just RE1)
            send2displays(1);
        }
        else if(key == '2')
        {
            LATE = 0x0004;  // 0100 (just RE2)
            send2displays(2);
        }
        else if(key == '3')
        {
            LATE = 0x0008;  // 1000 (just RE3)
            send2displays(3);
        }
        else
        {
            LATE = 0x000F;  // 1111 (all leds)
            send2displays(255);
            delay(1000);    // wait 1s = 1000ms
            LATE = 0x0000;  // 0000 (turn off all leds)
            //LATD = (LATD & 0xFF9F) | 0x0000;  // turn off displays (funciona, mas acho que não deveria funcionar desta forma,  porque os displays
                                                // estao a ser desligados e não voltam a ser ligados, mas está a dar igual a desligar os segmentos)
            // ou para desligar todos os segmentos (que é o pedido)
            /*LATBbits.LATB8 = 0; // a
            LATBbits.LATB9 = 0; // b
            LATBbits.LATB10 = 0;// c
            LATBbits.LATB11 = 0;// d
            LATBbits.LATB12 = 0;// e
            LATBbits.LATB13 = 0;// f
            LATBbits.LATB14 = 0;// g*/
            // ou em vez das linhas a cima
            LATB = LATB & 0x80FF;
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

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;

    unsigned char displayLow = value & 0x0F;
    unsigned char displayHigh = value >> 4;

    char hexValue;

    if(displayFlag)
    {
        LATD = (LATD & 0xFF9F) | 0x0020;            // display low active and high inactive (1111.1111.1001.1111 | 0000.0000.0010.0000)
        hexValue = display7Scodes[displayLow];      // convert to 7 segments code
        LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display
    }
    else
    {
        LATD = (LATD & 0xFF9F) | 0x0040;            // display low inactive and high active (1111.1111.1001.1111 | 0000.0000.0100.0000)
        hexValue = display7Scodes[displayHigh];     // convert to 7 segments code
        LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display
    }

    displayFlag = !displayFlag;
}

void send2displays_v1(unsigned char value) {

    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;
    char hexValue;

    // send digit_low (dl) to display_low: dl = value & 0x0F
    LATDbits.LATD6 = 0;     // display high inactive
    LATDbits.LATD5 = 1;     // display low active
    hexValue = display7Scodes[dl];   // convert to 7 segments code
    LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)
    delay(10);

    // send digit_high (dh) to display_high: dh = value >> 4
    LATDbits.LATD6 = 1;     // display high active
    LATDbits.LATD5 = 0;     // display low inactive
    hexValue = display7Scodes[dh];   // convert to 7 segments code
    LATB = (LATB & 0x00FF) | (hexValue << 8);   // send to display (0x00FF or 0x0000)
    delay(10);
}
