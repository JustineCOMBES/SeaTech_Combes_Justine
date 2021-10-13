#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
int main ( void){

// I n i t i a l i s a t i o n de l ? o s c i l l a t e u r

InitOscillator( ) ;

// Configuration des éentres s o r t i e s

InitIO ( ) ;
LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;
InitTimer23();
InitTimer1();

// Boucle Principale
while (1){


} // f i n main
}
