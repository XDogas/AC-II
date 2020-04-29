#include <detpic32.h>

void configureAll();
void configTimerT1();
void configTimerT3();
void setPWM(unsigned int);

volatile int voltage;   // Global variable

void main(void) {

    int dutyCycle;

    TRISD = TRISD & 0xFF9F;
    LATD = LATD & 0xF9FF;

    TRISB = TRISB & 0x80FF;
    LATB = LATB & 0x80FF;

    configureAll();
    EnableInterrupts(); // Global Interrupt Enable

    while(1) {
        char portVal = PORTB & 0x0003;   //Read RB1,RB0 to the variable portVal// Read RB1, RB0 to the variable "portVal"
        switch(portVal) {
            case 0: // Measure input voltage
                T1CONbits.TON = 1;  // Enable T1 interrupts
                setPWM(0);          // LED OFF
                break;
            case 1: // Freeze
                T1CONbits.TON = 0   // Disable T1 interrupts
                setPWM(100);        // LED ON (maximum bright)
                break;
            default: // LED brigthness control
                T1CONbits.TON = 1;  // Enable T1 interrupts
                dutyCycle = voltage * 3;
                setPWM(dutyCycle);
                break;
        }
    }
}

void _int_(4) isr_T1(void) {

    AD1CON1bits.ASAM = 1;   // Start A/D conversion
    IFS0bits.T1IF = 0;      // Reset T1IF flag
}

void _int_(12) isr_T3(void) {

    IFS0bits.T3IF = 0;      // Reset T3IF flag
}

void _int_(27) isr_adc(void) {

    int soma = 0;
    int *p = (int *)(&ADC1BUF0);
    for(; p < (int *)(&ADC1BUF8); p+=4) soma += *p;
    double media = (double) soma / 8.0;
    voltage = (char) ((media*33)/1023);
    IFS1bits.AD1IF = 0;     // Reset AD1IF flag

    //int i;
    //int *p= (int *) (&ADC1BUF0);
    //int soma=0;
    //
    //for (i = 0; i < 8; i++)//numero de amostras=8
    //{
    //    soma+=p[i*4];
    //}
    //int media=soma/8;//media das 8 amostras
    //voltage=(media*33*511)/1023;//calculate voltage amplitude
    //
    //IFS0bits.T3IF=0;//reset T3IF
}

void configAll() {

    TRISBbits.TRISB4 = 1;      //Desliga a componente digital de saida do porto RB4
	AD1PCFGbits.PCFG4 = 0;     //Configura o porto RB4 como entrada analogica(AN4)
	AD1CON1bits.SSRC = 7;      //Acionador da conversao dos bits de selecao
	AD1CON1bits.CLRASAM = 1;   //Termina a conversao quando o 1º interruptor A/D é gerado
	AD1CON3bits.SAMC = 16;     //Amostragem do tempo é 16TAD(TAD=100ns)
	AD1CON2bits.SMPI = 8-1;    //Interrupcao e gerada apos 8 amostras(samples)
	AD1CHSbits.CH0SA = 4;      //Entrada AN4
	AD1CON1bits.ON = 1;        //Inicia o conversor A/D

    configTimerT1();            // Configure Timer T1
    configTimerT3();            // Configure Timer T3
    EnableInterrupts();         // Global Interrupt Enable
    T1CONbits.TON = 1;          // Enable timer T1 (must be the last command of the timer configuration sequence)
    T3CONbits.TON = 1;          // Enable timer T3 (must be the last command of the timer configuration sequence)
}

void configTimerT1() {
    // Config Timer T1 (4Hz)
    T1CONbits.TCKPS = 3;
    PR1 = 19530;
    TMR1 = 0;                   // Reset timer T1 count register

    //Ativa interrupcoes
    IPC1bits.T1IP = 2;          // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;          // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;          // Reset timer T1 interrupt flag
}

void configTimerT3() {
    // Config Timer T3 (100 Hz)
    T3CONbits.TCKPS = 2;
    PR3 = 49999;
    TMR3 = 0;                   // Reset timer T3 count register

    //Ativa interrupcoes
    IPC3bits.T3IP = 2;          //Definir a prioridade da interrupcao
    IEC0bits.T3IE = 1;          //Ativa a interrupcao do Timer T3
    IFS0bits.T3IF = 0;          //Reset da interrupcao

    //PWM
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 3;     //Use Timer T3 as the time base for PWM(Pulse With Modulation) generation
	OC1CONbits.ON = 1;         //Enable OC3 module
}

void setPWM(unsigned int dutyCycle) {

    if(dutyCycle >= 0 && dutyCycle <= 100) {        // duty_cycle must be in the range [0, 100]
        OC1RS = ((PRx + 1) * dutyCycle) / 100;      // Evaluate OC1RS as a function of "dutyCycle"; alterar x pelo Timer
    } else printStr("Valor de duty-cycle invalido");
}
