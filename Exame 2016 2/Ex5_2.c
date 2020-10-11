// UART1: se carregasse na tecla 'l' imprimia no ecrã o valor lido do switch...
// a UART1 tinha que ser com interrupções

#include <detpic32.h>

int main(void)
{
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    configUart(115200,'N',1);

    while(1);

    return 0;
}

void configUart(unsigned int baudrate, char parity, unsigned int stopbits)
{
    U1MODEbits.BRGH = 0;
    U1BRG = ((20000000 + 8*baudrate) / (16*baudrate)) - 1;

    if(parity == 'O')
    {
        U1MODEbits.PDSEL = 2;
    }
    else if(parity == 'E')
    {
        U1MODEb4its.PDSEL = 1;
    }
    else if(parity == 'N')
    {
        U1MODEb4its.PDSEL = 0;
    }

    U1MODEbits.STSEL = stopbits-1;

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    IPC6bits.U1IP = 2;
    IEC0bits.U1RXIE = 1;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;

    U1MODEbits.ON = 1;
}
