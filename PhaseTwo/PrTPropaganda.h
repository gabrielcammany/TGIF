/* 
 * File:   PrTPropaganda.h
 * Author: JNM
 *
 * Placa de test per al PIC18F4321 controla el backlight del LCD, té un menú
 * interactiu pel SIO, i refresca l'estat dels perifèrics (2 pulsadors, 2 switchos,
 * 1 entrada analògica)
 * 
 */





#ifndef PRTPROPAGANDA_H
#define	PRTPROPAGANDA_H
#include <xc.h>
#include "BlTBacklight.h"
#include "SiTSio.h"
#include "PbTPushbutton.h"
#include "AuTAudio.h"
#include "LcTLCD.h"
#include "AdTADC.h"
#include "SwTSwitch.h"

#define INSERT_BIT_TIME "\rPlease insert bit time\r\n\0"
#define INSERT_ID "\rPlease insert ID\r\n\0"
#define TIMESTAMP "\rTimestamp:"
#define TIMESTAMP_SECOND "seconds since the last reset\r\n\0"

#define MAX_ID 3
#define MAX_BIT_TIME 3
#define EXTRA_ID_STRING 3

void myItoa(int num);
//Pre: 0<= num <= 9999
//Post: deixa a temp[3..0] el num en ASCII
void Menu(void);
//Pre: La SIO està inicialitzada
//Post: Pinta el menu pel canal sèrie

void initPropaganda(void);
//Pre: La SIO està inicialitzada
//Post: Inicialitza el timestamp i pinta la propaganda per la SIO

void MotorPropaganda(void);
void initMotorLCD(void);
//Pre: el LCD està inicialitzat
//Post: inicialitza el LCD per posar la marquesina a 0
void MotorLCD(void);


#endif	/* PRTPROPAGANDA_H */

