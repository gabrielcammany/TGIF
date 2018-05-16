/*
 * File:   TPWM.h
 *
 * TAD que fa encendre un led com si fos un heartbeat. Em serveix per veure si
 * el micro s'ha quedat penjat
 *
 * Created on 25 de enero de 2014, 23:02
 */

#ifndef TRF_H
#define	TRF_H
#include <xc.h>
#include "time.h"
#include "PrTPropaganda.h"
#include "AuTAudio.h"

#define MAX_DIGITS_RECIEVED_SIO 2
#define MAX_DIGITS_RECIEVED 2 + MAX_DIGITS_RECIEVED_SIO
#define EXTRA_SPACES            4
#define MAX_SIGNAL             (300 + EXTRA_SPACES)

typedef struct{
    char times;
    int periods;
}Senyal;

void RFInit();
//Pre: Requereix del TITITMER.c
//Post: Demana un timer i posa la sortida del LED a 0
void MotorRF();

unsigned char* getSignal(unsigned char offset);
//Pre: 0<= offset <= MAX_SIGNAL
//Post: Retorna el missatge que hi ha actualment amb el offset especificat

int getLength();
//Pre: -
//Post: Retorna la mida del missatge tenim desat

char getFrequency(char index);

char getVpp();

char getOffset();

Senyal* getSignals();

#endif	/* LETLEDS_H */

