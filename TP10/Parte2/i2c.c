#include <detpic32.h>
#include "i2c.h"

void i2c1_init(unsigned int clock_freq)
{
    I2C1BRG = (PBCLK + clock_freq) / (2 * clock_freq) - 1;  // Config baudrate generator (see introduction for details)
    I2C1CONbits.ON = 1;                                     // Enable I2C1 module
}

void i2c1_start(void)
{
    while((I2C1CON & 0x001F) != 0); // Wait until the lower 5 bits of I2CxCON are all 0 (the lower 5 bits of I2CxCON must be 0 before attempting to set the SEN bit)
    I2C1CONbits.SEN = 1;            // Activate Start event (I2C1CON, bit SEN)
    while(I2C1CONbits.SEN == 1);    // Wait for completion of the Start event (I2C1CON, bit SEN)
}

void i2c1_stop(void)
{
    while((I2C1CON & 0x001F) != 0); // Wait until the lower 5 bits of I2CxCON are all 0 (the lower 5 bits of I2CxCON must be 0 before attempting to set the PEN bit)
    I2C1CONbits.PEN = 1;            // Activate Stop event (I2C1CON, bit PEN)
    while(I2C1CONbits.PEN == 1);    // Wait for completion of the Stop event (I2C1CON, bit PEN)
}

int i2c1_send(unsigned char value)
{
    I2C1TRN = value;                // Copy "value" to I2C1TRN register
    while(I2C1STATbits.TRSTAT == 1);// Wait while master transmission is in progress (8 bits + ACK\) (I2C1STAT, bit TRSTAT – transmit status bit)
    return I2C1STATbits.ACKSTAT;    // Return acknowledge status bit (I2C1STAT, bit ACKSTAT)
}

char i2c1_receive(char ack_bit)
{
    while((I2C1CON & 0x001F) != 0); // Wait util the lower 5 bits of I2C1CON are all 0 (the lower 5 bits of I2C1CON must be 0 before attempting to set the RCEN bit)
    I2C1CONbits.RCEN = 1;           // Activate RCEN bit (receive enable bit, I2C1CON register)
    while(I2C1STATbits.RBF != 1);   // Wait while byte not received (I2C1STAT, bit RBF – receive buffer full status bit)
    // Send ACK / NACK bit. For that:
    //  1. Copy "ack_bit" to I2C1CON, bit ACKDT (be sure "ack_bit" value is only 0 or 1)
    if(ack_bit != 0 && ack_bit != 1);
    else I2C1CONbits.ACKDT = ack_bit;
    //  2. Wait until the lower 5 bits of I2C1CON are all 0 (the lower 5 bits of I2C1CON must be 0 before attempting to set the ACKEN bit.
    while((I2C1CON & 0x001F) != 0);
    //  3. Start Acknowledge sequence (I2C1CON register, bit ACKEN=1)
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);  // Wait for completion of Acknowledge sequence (I2C1CON, bit ACKEN)
    return I2C1RCV;                 // Return received value (I2C1RCV)
}
