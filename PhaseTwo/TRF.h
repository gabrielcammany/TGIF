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
#include "AuTAudio.h"
#include "PrTPropaganda.h"

#define MAX_DIGITS_RECIEVED_SIO 2
#define MAX_DIGITS_RECIEVED 2 + MAX_DIGITS_RECIEVED_SIO
#define MAX_SIGNALS             6

#define MAX_TIMES               5
#define MAX_PERIODS             7


typedef struct{
    char times[MAX_TIMES];
    char periods[MAX_PERIODS];
}Senyal;

void RFInit();
//Pre: Requereix del TITITMER.c
//Post: Demana un timer i posa la sortida del LED a 0
void MotorRF();

Senyal* getSignals();

#endif	/* LETLEDS_H */

