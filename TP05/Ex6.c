#include <detpic32.h>

int main(void) {

    volatile int aux;

    TRISEbits.TRISE0=0;         //Configure A/D module; configure RE0 as a digital output port

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

    while(1) {

        LATEbits.LATE0 = 1;             // Set LATE0
        AD1CON1bits.ASAM = 1;           // Start conversion
        while(IFS1bits.AD1IF == 0);     // Wait while conversion not done (AD1IF == 0)
        LATEbits.LATE0 = 0;             // Reset LATE0
        aux = ADC1BUF0;             // Read conversion result (ADC1BUF0) to "aux" variable
        IFS1bits.AD1IF = 0;             // Reset AD1IF
    }

    return 0;
}
