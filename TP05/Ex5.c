#include <detpic32.h>
#include "delay.c"
#include "send2displays_v2.c"
#include "toBcd.c"

unsigned char toBcd(unsigned char);

int main(void) {

    //TRISB = (TRISB & 0x00FF) | 0x000F;
    // ou (ver qual está bem)
    TRISB = TRISB & 0x80FF;
    
    TRISD = TRISD & 0xFF9F;

    LATD = (LATD & 0xFF9F) | 0x0060;

    // configurar o porto RB4 como entrada analógica :
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;      // RB4 configured as analog input (AN4)

    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this
                                //  mode an internal counter ends sampling and
                                //  starts conversion

    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter
                                //  interrupt is generated. At the same time,
                                //  hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)

    AD1CON2bits.SMPI = 4-1;     // Interrupt is generated after XX samples  // 4 samples will be converted and stored
                                //  (replace XX by the desired number of    // in buffer locations ADC1BUF0 to ADC1BUF3
                                //  consecutive samples)

    AD1CHSbits.CH0SA = 4;       // replace x by the desired input
                                //  analog channel (0 to 15)

    AD1CON1bits.ON = 1;         // Enable A/D converter
                                //  This must the last command of the A/D
                                //  configuration sequence

    int i = 0;
    int v = 0;
    while(1) {
        if(i++ % 25 == 0) {     // 0, 250ms, 500ms, 750ms, ...
            AD1CON1bits.ASAM = 1;                   // Start conversion
            while(IFS1bits.AD1IF == 0);             // Wait while conversion not done (AD1IF == 0)
            int *p = (int *)(&ADC1BUF0);
            int soma = 0;

            for(; p <= (int *)(&ADC1BUFF); p+=4) soma += *p;

            double media = soma / 4.0;
            v = (media * 33) / 1023;
            IFS1bits.AD1IF = 0;                     // Reset AD1IF
        }
        send2displays_v2(toBcd(v & 0xFF));
	printf("%d\n",v);
        delay(10);                                  // Wait 10ms
    }
    return 0;
}
