// igual ao anterior com interrupts

#include <detpic32.h>

void configADC();
void configInterruptSystem();
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void delay(unsigned int ms);

unsigned int med, sum, val, i;

int main(void)
{
    TRISB = TRISB & 0x80FF;

    configADC();
    configInterruptSystem();

    AD1CON1bits.ASAM = 1;

    while(1);

    return 0;
}

void configADC()
{
    TRISBbits.TRISB4 = 1;      //Desliga a componente digital de saida do porto RB4
    AD1PCFGbits.PCFG4 = 0;     //Configura o porto RB4 como entrada analogica(AN4)
    AD1CON1bits.SSRC = 7;      //Acionador da conversao dos bits de selecao
    AD1CON1bits.CLRASAM = 1;   //Termina a conversao quando o 1º interruptor A/D é gerado
    AD1CON3bits.SAMC = 16;     //Amostragem do tempo é 16TAD(TAD=100ns)
    AD1CON2bits.SMPI = 16-1;    //Interrupcao e gerada apos 16 amostras(samples)
    AD1CHSbits.CH0SA = 4;      //Entrada AN4
    AD1CON1bits.ON = 1;        //Inicia o conversor A/D
}

void configInterruptSystem()
{
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
    EnableInterrupts();
}

void _int_(27) isr_adc(void)
{
    AD1CON1bits.ASAM = 1;
    while(IFS1bits.AD1IF == 0);

    med = 0;
    sum = 0;

    int *p = (int *)(&ADC1BUF0);

    for(i=0; i<16; i++)
    {
        sum += p[i*4];
    }

    med = sum/16;

    val =( med*(57+9)+511)/1023;
    val -= 9;

    i = 0;
    if(val>0)
    {
        do {
            send2displays(toBcd(val));
            delay(10);
        } while(i++ < 20);
    }
    else
    {
        unsigned char blinkFlag = 0;

        if(blinkFlag)
        {
            do {
                send2displays(toBcd(val));
                delay(10);
            } while(i++ < 20);
        }
        else{
            LATB = LATB & 0xFF00;
            delay(200);
        }

        blinkFlag = !blinkFlag;
    }
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    unsigned char displayFlag = 0;

    unsigned char displayLow = value & 0x0F;
    unsigned char displayHigh = value >> 4;

    char hexValue;

    if(displayFlag)
    {
        LATD = (LATD & 0xFF9F) | 0x0020;
        hexValue = display7Scodes[displayLow];
        LATB = (LATB & 0x00FF) | (hexValue << 8);
    }
    else
    {
        LATD = (LATD & 0xFF9F) | 0x0040;
        hexValue = display7Scodes[displayHigh];
        LATB = (LATB & 0x00FF) | (hexValue << 8);
    }

    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
    return ((value/10) << 4) + (value % 10);
}

void delay(unsigned int ms)
{
    for(; ms>0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}
