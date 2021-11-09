#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"
#include "robot.h"

unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;

int main ( void){

// Initialisation de l'oscillateur

InitOscillator();

//PWMSetSpeed(-20);

// Configuration des entres sorties
InitIO () ;
LED_BLANCHE = 0;
LED_BLEUE = 0;
LED_ORANGE = 0;

// Initialisation des timers
InitTimer23();
InitTimer1();

// Initialisation PWM
InitPWM();

// Initialisation ADC
InitADC1();
 
// Boucle Principale
while(1){
    unsigned char end = ADCIsConversionFinished();
    if(end){
        int* result = ADCGetResult(); 
        ADCValue0 = result[0]; // droit en se mettant a la place du robot
        ADCValue1 = result[1]; // millieu
        ADCValue2 = result[2]; // gauche
        ADCClearConversionFinishedFlag();
        
        // telemetre
         if ( ADCIsConversionFinished () == 1)
        {
            ADCClearConversionFinishedFlag();
            unsigned int* result = ADCGetResult();
            float volts = ((float) result[2])*3.3/4096*3.2;
            robotState.distanceTelemetreDroit = 34/volts - 5;
            volts = ((float) result[1])*3.3/4096*3.2;
            robotState.distanceTelemetreCentre = 34/volts - 5;
            volts = ((float) result[0])*3.3/4096*3.2;
            robotState.distanceTelemetreGauche = 34/volts - 5;
        }

        // test 
        /*
        if(ADCValue0 > 358){ // DROITE
            LED_ORANGE = 1;
        }else{
            LED_ORANGE = 0;
        }
        if(ADCValue1 > 358){ // MILLIEU
            LED_BLEUE = 1;
        }else{
            LED_BLEUE = 0;
        }
        if(ADCValue2 > 358){ // GAUCHE
            LED_BLANCHE = 1;
        }else{
            LED_BLANCHE = 0;
        }
         */
    }
} // fin main
}
