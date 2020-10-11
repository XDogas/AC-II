#include <detpic32>

void configADC();
void configInterruptSystem();
void configTimer();

int med, sum, val, i;

int main(void)
{
    TRISB = TRISB & 0x80FF;

    configADC();
    configInterruptSystem();
    configTimer();

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
    IFS1bits.AD1FS = 0;
    EnableInterrupts();
}

void configTimer() //2Hz
{
    T3CONbits.TCKPS = 8;
    PR3 = 39062;
    TMR3 = 0;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    T3CONbits.TON = 1;
}

void _int_(27) isr_adc(void)
{
    AD1CON1bits.ASAM = 1;

    med = 0;

    int *p = (int *)(&ADC1BUF0);

    for(i=0; i<16; i++)
    {
        sum += p[i*4];
    }

    med = sum/16;

    val = (med*5+511)/1023;

    do {
        send2displays(toBcd(val));
        delay(10);
    } while(i++ < 20);

    IFS1bits.AD1IF = 0;
}

void _int_(12) isr_T3(void)
{
    AD1CONbits.ASAM = 1;
    IFS0bits.T3IF = 0;
}

void sen2displays(unsigned char value)
{
    static conte display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    unsigned char displayFlag;

    unsigned char displayLow = value & 0x0F;
    unsigned char displayHigh = value >> 4;

    unsigned char hexValue;

    if(displayFlag)
    {
        LATD = (LATD & 0x80FF) | 0x0020;
        hexValue = display7Scodes[displayLow];
        LATB = (LATB & 0x00FF) | (hexValue << 8);
    }
    else
    {
        LATD = (LATD & 0x80FF) | 0x0040;
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
