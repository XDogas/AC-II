#include <detpic32.h>
#include "eeprom.h"
#include "i2c.h"

int getTemperature(int *temperature);
void delay(unsigned int ms);


int main(void)
{
    int temperature, ack;
    i2c1_init(TC74_CLK_FREQ);
    while(1)
    {
        ack = getTemperature(&temperature);
        printStr("Temperature: ");
        printInt10(temperature);                // Print "temperature" variable (syscall printInt10)
        putChar('\n');
        delay(250);                             // Wait 250 ms
    }
    return 0;
}

int getTemperature(int *temperature)
{
    int ack;
    i2c1_start();                           // Send Start event
    ack = i2c1_send(ADDR_WR);               // Send Address + WR (ADDR_WR); copy return value to "ack" variable
    ack += i2c1_send(RTR);                  // Send Command (RTR); add return value to "ack" variable
    i2c1_start();                           // Send Start event (again)
    ack += i2c1_send(ADDR_RD);              // Send Address + RD (ADDR_RD); add return value to "ack" variable
    if(ack != 0)                            // Test "ack" variable; if "ack" != 0 then an error has occurred;
    {
        i2c1_stop();                        // send the Stop event
        printStr("Error: ack != 0");        //print an error message and exit loop
    }
    else *temperature = i2c1_receive(I2C_NACK);  // Receive a value from slave (send NACK as argument); copy received value to "temperature" variable
    return ack;
}

void delay(unsigned int ms)
{
    for(; ms>0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000); // Freq(Hz) = 1/Period(seconds)
    }
}
