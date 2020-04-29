#include <detpic32.h>

void setPWM(unsigned int dutyCycle) {

    if(dutyCycle >= 0 && dutyCycle <= 100) {        // duty_cycle must be in the range [0, 100]
        OC1RS = ((PRx + 1) * dutyCycle) / 100;      // Evaluate OC1RS as a function of "dutyCycle"; alterar x pelo Timer
    } else printStr("Valor de duty-cycle invalido");
}
