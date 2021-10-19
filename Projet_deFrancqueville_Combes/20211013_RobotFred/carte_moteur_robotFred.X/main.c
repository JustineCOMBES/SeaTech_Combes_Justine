#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
int main ( void){

// Initialisation de l'oscillateur

InitOscillator();

//PWMSetSpeed(-20);

// Configuration des entres sorties
InitIO () ;
LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;

// Initialisation des timers
InitTimer23();
InitTimer1();
// Initialisation PWM
InitPWM();

// Boucle Principale
while (1){

} // f i n main
}
