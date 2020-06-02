#include <detpic32.h>
#include "delay.c"

int main(void) {

    LATB = LATB & 0x00FF;   //inicializa
    TRISB = TRISB & 0x00FF; //configura como saida RB8 A RB15 (bin:0000.0000.1111.1111 -> hex:0x00FF)

    LATDbits.LATD5 = 0;     //inicializa
    LATDbits.LATD6 = 0;     //inicializa

    TRISDbits.TRISD5 = 0;   //RD5 configurado como output (output -> 0 ; input -> 1)
    TRISDbits.TRISD6 = 0;   //RD6 configurado como output

    LATDbits.LATD5 = 1;     //atribui valor
    LATDbits.LATD6 = 0;     //atribui valor

    char c;

    while(1) {

        c = getChar();

        if(c == 'a' || c == 'A') {
            LATBbits.LATB8 = 1;
            delay(250); // 250 milliseconds = 0.25 seconds
            LATBbits.LATB8 = 0;
        } else if(c == 'b' || c == 'B') {
            LATBbits.LATB9 = 1;
            delay(250);
            LATBbits.LATB9 = 0;
        } else if(c == 'c' || c == 'C') {
            LATBbits.LATB10 = 1;
            delay(250);
            LATBbits.LATB10 = 0;
        } else if(c == 'd' || c == 'D') {
            LATBbits.LATB11 = 1;
            delay(250);
            LATBbits.LATB11 = 0;
        } else if(c == 'e' || c == 'E') {
            LATBbits.LATB12 = 1;
            delay(250);
            LATBbits.LATB12 = 0;
        } else if(c == 'f' || c == 'F') {
            LATBbits.LATB13 = 1;
            delay(250);
            LATBbits.LATB13 = 0;
        } else if(c == 'g' || c == 'G') {
            LATBbits.LATB14 = 1;
            delay(250);
            LATBbits.LATB14 = 0;
        } else if(c == '.') {
            LATBbits.LATB15 = 1;
            delay(250);
            LATBbits.LATB15 = 0;
        }
        else if(c == 't' || c == 'T')   // acrescentado ao exercicio: tecla t = troca do display,
        {                               // iluminando o display a ser utilizado
            LATDbits.LATD5 = !LATDbits.LATD5;
            LATDbits.LATD6 = !LATDbits.LATD6;

            LATBbits.LATB8 = 1;
            LATBbits.LATB9 = 1;
            LATBbits.LATB10 = 1;
            LATBbits.LATB11 = 1;
            LATBbits.LATB12 = 1;
            LATBbits.LATB13 = 1;
            LATBbits.LATB14 = 1;
            LATBbits.LATB15 = 1;
            delay(250); // 0.250ms
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB10 = 0;
            LATBbits.LATB11 = 0;
            LATBbits.LATB12 = 0;
            LATBbits.LATB13 = 0;
            LATBbits.LATB14 = 0;
            LATBbits.LATB15 = 0;
        }
    }

    return 0;
}
