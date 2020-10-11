// Repetir a alinea anterior mas com um sinal
// no porto de teste OC1 de 100Hz e duty-cycle de 50%

#include <detpic32.h>

void configADC();
void configInterruptSystem();
void configTimer();
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void setPWM(unsigned int duty_cycle);

int med;

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
    AD1CON2bits.SMPI = 4-1;    //Interrupcao e gerada apos 4 amostras(samples)
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

void configTimer()
{
    // T2
    T2CONbits.TCKPS = 5;
    PR2 = 62500;
    TMR2 = 0;

    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;

    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 0;
    setPWM(50);
    OC1CONbits.ON = 1;

    //T3
    T3CONbits.TCKPS = 5;
    PR3 = 62500;
    TMR3 = 0;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;


    EnableInterrupts();

    T2CONbits.TON = 1;
    T3CONbits.TON = 1;
}

void _int_(27) isr_adc(void)
{
    unsigned int sum=0;

    int *p = (int *)(&ADC1BUF0);

    int i;
    for(i=0; i<4; i++)
    {
        sum += p[i*4];
    }

    med = (((sum/4)*99)+511)/1023;

    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_T2(void)
{
    AD1CON1bits.ASAM = 1;
    IFS0bits.T2IF = 0;
}

void _int_(12) isr_T3(void)
{
    AD1CON1bits.ASAM = 1;
    send2displays(toBcd(med));
    IFS0bits.T3IF = 0;
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    unsiged char displayFlag = 0;

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

void setPWM(unsigned int duty_cycle)
{
    OC1RS = ((PR2 + 1) * duty_cycle) / 100;
}
