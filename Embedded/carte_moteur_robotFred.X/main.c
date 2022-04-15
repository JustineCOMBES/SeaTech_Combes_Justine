#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"
#include "robot.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"

int ADCValue0;
int ADCValue1;
int ADCValue2;
int ADCValue3;
int ADCValue4;

float vitesse = 30;
unsigned char stateRobot;



void OperatingSystemLoop(void) {
        switch (stateRobot) {
            case STATE_ATTENTE:
                timestamp = 0;
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_ATTENTE_EN_COURS;

            case STATE_ATTENTE_EN_COURS:
                if (timestamp > 1000)
                    stateRobot = STATE_AVANCE;
                break;

            case STATE_AVANCE:
                PWMSetSpeedConsigne(vitesse, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitesse, MOTEUR_GAUCHE);
                stateRobot = STATE_AVANCE_EN_COURS;
                break;
            case STATE_AVANCE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            //
			case STATE_TOURNE_PEU_GAUCHE:
                PWMSetSpeedConsigne(vitesse, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_PEU_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
				//
            case STATE_TOURNE_GAUCHE:
                PWMSetSpeedConsigne(2*vitesse/3, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-vitesse/3, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
			//
			case STATE_TOURNE_PEU_DROITE:
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitesse, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_PEU_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
			//

            case STATE_TOURNE_DROITE:
                PWMSetSpeedConsigne(-vitesse/3, MOTEUR_DROIT);
                PWMSetSpeedConsigne(2*vitesse/3, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_GAUCHE:
                PWMSetSpeedConsigne(vitesse/2, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-vitesse/2, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_DROITE:
                PWMSetSpeedConsigne(-vitesse/2, MOTEUR_DROIT);
                PWMSetSpeedConsigne(vitesse/2, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            default:
                stateRobot = STATE_ATTENTE;
                break;
        }
    }

    unsigned char nextStateRobot = 0;

    void SetNextRobotStateInAutomaticMode(void) {
        unsigned char positionObstacle = PAS_D_OBSTACLE;
		
		// Distances
        int distance = 35;
        int moyenneDistance = 40;
        int petiteDistance = 15;
		int D = robotState.distanceTelemetreDroit < moyenneDistance;
		int M = robotState.distanceTelemetreCentre < distance;
		int G = robotState.distanceTelemetreGauche < moyenneDistance;
		int FG = robotState.distanceTelemetreFullGauche < petiteDistance;
		int FD = robotState.distanceTelemetreFullDroit < petiteDistance;
		
        //Détermination de la position des obstacles en fonction des télémètres
		
		if(M||(D&&G))
			positionObstacle = OBSTACLE_EN_FACE;
		else if(D)
			positionObstacle = OBSTACLE_A_DROITE;
		else if (G)
			positionObstacle = OBSTACLE_A_GAUCHE;
		else if (FD)
			positionObstacle = OBSTACLE_PEU_A_DROITE;
		else if (FG)
			positionObstacle = OBSTACLE_PEU_A_GAUCHE;
		else 
			positionObstacle = PAS_D_OBSTACLE;
		
        // if (robotState.distanceTelemetreDroit < 30 &&
                // robotState.distanceTelemetreCentre > 20 &&
                // robotState.distanceTelemetreGauche > 30) //Obstacle à droite
            // positionObstacle = OBSTACLE_A_DROITE;
        // else if (robotState.distanceTelemetreDroit > 30 &&
                // robotState.distanceTelemetreCentre > 20 &&
                // robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
            // positionObstacle = OBSTACLE_A_GAUCHE;
        // else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
            // positionObstacle = OBSTACLE_EN_FACE;
        // else if (robotState.distanceTelemetreDroit > 30 &&
                // robotState.distanceTelemetreCentre > 20 &&
                // robotState.distanceTelemetreGauche > 30) //pas d?obstacle
            // positionObstacle = PAS_D_OBSTACLE;

        //Détermination de l?état à venir du robot
        if (positionObstacle == PAS_D_OBSTACLE)
            nextStateRobot = STATE_AVANCE;
		else if (positionObstacle == OBSTACLE_PEU_A_DROITE)
            nextStateRobot = STATE_TOURNE_PEU_GAUCHE;
        else if (positionObstacle == OBSTACLE_A_DROITE)
            nextStateRobot = STATE_TOURNE_GAUCHE;
        else if (positionObstacle == OBSTACLE_A_GAUCHE)
            nextStateRobot = STATE_TOURNE_DROITE;
		else if (positionObstacle == OBSTACLE_PEU_A_GAUCHE)
            nextStateRobot = STATE_TOURNE_PEU_DROITE;
        else if (positionObstacle == OBSTACLE_EN_FACE && stateRobot == STATE_TOURNE_DROITE)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
        else if (positionObstacle == OBSTACLE_EN_FACE && stateRobot == STATE_TOURNE_GAUCHE)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        else if (positionObstacle == OBSTACLE_EN_FACE)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

        //Si l?on n?est pas dans la transition de l?étape en cours
        if (nextStateRobot != stateRobot - 1)
            stateRobot = nextStateRobot;
    }
    
int main(void) {

    // Initialisation de l'oscillateur

    InitOscillator();

    //PWMSetSpeed(-20);

    // Configuration des entres sorties
    InitIO();
    LED_BLANCHE = 0;
    LED_BLEUE = 0;
    LED_ORANGE = 0;

    // Initialisation des timers
    InitTimer23();
    InitTimer1();
    InitTimer4();
    // Initialisation PWM
    InitPWM();

    // Initialisation ADC
    InitADC1();


    // Initialisation UART

    InitUART();



    // Boucle Principale
    while (1) {
        // telemetre
        int i;
for(i=0;i<CB_RX1_GetDataSize();i++)
{
unsigned char c=CB_RX1_Get();
SendMessage(&c,1);
}
__delay32(1000);
    }
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            int* result = ADCGetResult();

            ADCValue0 = result[0];
            ADCValue1 = result[1];
            ADCValue2 = result[2];
            ADCValue3 = result[3];
            ADCValue4 = result[4];

            float volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            if (robotState.distanceTelemetreCentre < 30)
                LED_BLEUE = 1;
            else
                LED_BLEUE = 0;

            volts = ((float) result[4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
          /*  if (robotState.distanceTelemetreGauche < 30)
                LED_BLANCHE = 1; // gauche
            else
                LED_BLANCHE = 0;
*/
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
          /*  if (robotState.distanceTelemetreDroit < 30)
                LED_ORANGE = 1;
            else
                LED_ORANGE = 0;
            */
            volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreFullDroit = 34 / volts - 5;
            if (robotState.distanceTelemetreFullDroit < 30 || robotState.distanceTelemetreDroit < 30)
                 LED_ORANGE = 1;
            else
                LED_ORANGE = 0;

            
            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreFullGauche = 34 / volts - 5;
            if (robotState.distanceTelemetreFullGauche < 30 || robotState.distanceTelemetreGauche < 30)
                LED_BLANCHE = 1;
            else
                LED_BLANCHE = 0;
        }
//SendMessageDirect((unsigned char*) "Bonjour" ,7);
//__delay32(40000000); 
        SendMessage("Bonjour" , 7);
        // test 
        //*
        //        if(ADCValue0 > 358){ // G
        //            LED_ORANGE = 1;
        //        }else{
        //            LED_ORANGE = 0;
        //        }
        //        if(ADCValue1 > 358){ // M
        //            LED_BLEUE = 1;
        //        }else{
        //            LED_BLEUE = 0;
        //        }
        //        if(ADCValue2 > 358){ // D
        //            LED_BLANCHE = 1;
        //        }else{
        //            LED_BLANCHE = 0;
        //        }
        //        if(ADCValue3 > 358){ 
        //            LED_BLANCHE = 1;
        //            LED_BLEUE = 1;
        //        }else{
        //            LED_BLANCHE = 0;
        //            LED_BLEUE = 0;
        //        }
        //        if(ADCValue4 > 358){ 
        //            LED_BLANCHE = 1;
        //            LED_ORANGE = 1;
        //        }else{
        //            LED_BLANCHE = 0;
        //            LED_ORANGE = 0;
        //        }
        //*/
    } // fin main
}
