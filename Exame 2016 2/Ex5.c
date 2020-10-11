// UART1: se carregasse na tecla 'l' imprimia no ecrã o valor lido do switch...
// a UART1 tinha que ser com interrupções

#include <detpic32.h>

void configUart(unsigned int baudrate, char parity, unsigned int stopbits);
void putInt(unsigned int value);
void putStr(char *str);
void putc(char byte);
unsigned char toBcd(unsigned char value);

int main(void)
{
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    configUart(115200,'N',1);
    EnableInterrupts();

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
        U1MODEbits.PDSEL = 1;
    }
    else if(parity == 'N')
    {
        U1MODEbits.PDSEL = 0;
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

void _int_(24) isr_uart1()
{
    if(IFS0bits.U1RXIF == 0)
    {
        char c = U1RXREG;
        if(c == 'l')
        {
            char switch = PORTB & 0x000F;
            putInt(switch);
            putStr("\n");
        }
        else
        {
            putStr("Nenhum valor atribuido.\n");
        }

        IFS0bits.U1RXIF = 0;
    }
}

void putInt(unsigned int value)
{
    putc((toBcd(value) >> 4)+'0');
    putc((toBcd(value) & 0x0F)+'0');
}

void putStr(char *str)
{
    while(*str != '\0')
    {
        putc(*str);
        str++;
    }
}

void putc(char byte)
{
    while(U1STAbits.UTXBF);
    U1TXREG = byte;
}

unsigned char toBcd(unsigned char value)
{
    return ((value/10) << 4) + (value % 10);
}
