// igual á primeira parte, com alterações nos Exs. 6 e 9, e apenas se utiliza a main do Ex110

#include <detpic32.h>

#define FALSE 0
#define TRUE 1

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

typedef struct {
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
    unsigned int overrun;   //Ex3
} circularBuffer;

volatile circularBuffer txb;    // Transmission buffer
volatile circularBuffer rxb;    // Reception buffer

void comDrv_config(unsigned int baudrate, char parity, unsigned int stopbits) {

	//Valores a considerar por defeito

	if(baudrate <600 || baudrate >115200){
		baudrate=115200;
	}
	if(parity != 'N' && parity != 'E' && parity != 'O'){
		parity='N';
	}
	if(stopbits !=1 && stopbits !=2){
		stopbits=1;
	}

	//1-Configurar o gerador baudrate de acordo com a taxa de transmissão e receção pretendidas

	U1MODEbits.BRGH=0; //0 se for fator 16 e 1 se for fator 4
	U1BRG=((20000000 + 8*baudrate)/(16*baudrate))-1; //U1BRG=((fpclk + 8*baudrate)/(16*baudrate))-1

	//2-Configurar os parametros da trama: dimensão da palavra a transmitir(numero de data bits) e tipo de paridade

	if(parity == 'N'){//Tipo de paridade 'N'
		U1MODEbits.PDSEL=0;
	}
	else if(parity == 'E'){//Tipo de paridade 'E'
		U1MODEbits.PDSEL=1;
	}
	else if(parity == 'O'){//Tipo de paridade 'O'
		U1MODEbits.PDSEL=2;
	}
	U1MODEbits.STSEL=stopbits-1;//Numero de stop bits(se forem x stop bits fica x-1)

	//3-Ativar os modulos de transmissão e receção

	U1STAbits.UTXEN=1;//Ativa o modulo de transmissão
	U1STAbits.URXEN=1;//Ativa o modulo de rececão

	//4-Ativar a UART

	U1MODEbits.ON=1;//Ativa a UART

    IEC0bits.U1RXIE = 1;
    IPC6bits.U1IP = 2;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1EIE = 1;
    IFS0bits.U1EIF = 0;
}

void comDrv_flushRx(void) {
    // Initialize variables of the reception buffer
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;
    rxb.overrun=0;//Ex3
}

void comDrv_flushTx(void) {
    // Initialize variables of the transmission buffer
    txb.head = 0;
    txb.tail = 0;
    txb.count = 0;
    rxb.overrun=0;//Ex3
}

void comDrv_putc(char ch) {
    while(txb.count == BUF_SIZE){}          // Wait while buffer is full
    txb.data[txb.tail] = ch;                // Copy character to the transmission buffer at position "tail"
    txb.tail = (txb.tail + 1) % BUF_SIZE;   // Increment "tail" index (mod. BUF_SIZE)
    DisableUart1TxInterrupt();              // Begin of critical section
    txb.count++;                            // Increment "count" variable
    EnableUart1TxInterrupt();               // End of critical section
}

void comDrv_puts(char *s) {
    while(*s != 0) {
        comDrv_putc(*s);
        s++;
    }
}

void _int_(24) isr_uart1() {

    if(IFS0bits.U1TXIF) {                               // if U1TXIF is set
        while(txb.count > 0 && U1STAbits.UTXBF == 0) {  // while "count" variable (transmission buffer, txb) is greater than 0 (copy more than 1 char)
            U1TXREG = txb.data[txb.head];               // Copy character pointed by "head" to U1TXREG register
            txb.head = (txb.head + 1) % BUF_SIZE;       // Increment "head" variable (mod BUF_SIZE)    (em vez de % BUF_SIZE pode ser & INDEX_MASK)
            txb.count--;                                // Decrement "count" variable
        }
        if(txb.count == 0)                              // if "count" variable is 0 then
            DisableUart1TxInterrupt();
        IFS0bits.U1TXIF = 0;                            // Reset UART1 TX interrupt flag
    }

    if(IFS0bits.U1RXIF) {                                   // if U1RXIF is set
        while(rxb.count < BUF_SIZE && U1STAbits.URXDA !=0) {// Copy more that 1 char
            rxb.data[rxb.tail] = U1RXREG;                   // Read character from UART and write it to the "tail" position of the reception buffer
            rxb.tail = (rxb.tail + 1) % BUF_SIZE;           // Increment "tail" variable (mod BUF_SIZE)
            if(rxb.count < BUF_SIZE) rxb.count++;           // If reception buffer is not full (e.g. count < BUF_SIZE) then increment "count" variable
            else rxb.head = (rxb.head + 1) % BUF_SIZE;      // Else increment "head" variable (discard oldest character)
            IFS0bits.U1RXIF = 0;                                // reset UART1 RX interrupt flag
        }
    }

    //Ex3
	//Verificação da existencia de overrun
	if(IFS0bits.U1EIF){           //If U1EIF set then
        char dummy;
		if(U1STAbits.OERR) {      //if overrun error then
			U1STAbits.OERR = 0;   //clear OERR flag
			rxb.overrun = 1;
		}
        if (U1STAbits.FERR || U1STAbits.PERR) dummy = U1RXREG;

		IFS0bits.U1EIF=0;//clear UART1 error interrupt flag
	}
}

char comDrv_getc(char *pchar) {
    if(rxb.count == 0) return FALSE;            // Test "count" variable (reception buffer) and return FALSE if it is zero
    DisableUart1RxInterrupt();                  // Begin of critical section
    *pchar = rxb.data[rxb.head];                // Copy character pointed by "head" to *pchar
    rxb.count--;                                // Decrement "count" variable
    rxb.head = (rxb.head + 1) % BUF_SIZE;       // Increment "head" variable (mod BUF_SIZE)
    EnableUart1RxInterrupt();                   // End of critical section
    return TRUE;
}

int main()
{
	char buffer;
	comDrv_config(115200, 'N', 1);     // default "pterm" parameters with RX interrupts enabled and TX interrupts disabled
	comDrv_flushTx();
	comDrv_flushRx();
	EnableInterrupts();
	comDrv_puts("PIC32 UART Device-driver\n");
	while(1) {
		if (comDrv_getc(&buffer)) {    // Read character from reception buffer
			if (buffer == 'S') comDrv_puts("Uma string com 30 caracteres!\n");
			comDrv_putc(buffer);       // Send character to the transmission buffer
		}
	}
	return 0;
}
