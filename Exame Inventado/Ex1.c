// configurar ADC; codigo de barras nos leds...

#include <detpic32.h>

void configADC();

int main(void)
{
    TRISE = TRISE & 0xFFF0;

    configADC();

    unsigned int med, sum, val, i;
    while(1)
    {
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);

        med = 0;

        int *p = (int *)(&ADC1BUF0);

        for(i=0; i<4; i++)
        {
            sum += p[i*4];
        }

        med = sum/4;

        val = (med*15+511)/1023;
        
        LATE = val;
    }

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
