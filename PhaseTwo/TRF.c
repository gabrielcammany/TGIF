/*
 * File:   TRF.c
 * Author: Gabriel Cammany
 *
 */

#include <stdio.h>
#include <string.h>

#include "TRF.h"

#define END_BYTE                0x00
#define START_BYTE              0x40
#define IN                      PORTBbits.RB13
#define MAX_ID_RF               3

static unsigned char estatRF,input, timerRF, timerRF2, sincronized, pos, signal;
static unsigned char inValue,id_trama[MAX_ID_RF];

static int caracter, pulses;

void initCadenaRF(unsigned char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        cadena[i] = ' ';
    }
    
}


void RFInit(){
    
    TRISBbits.TRISB13 = 1;
    AD1PCFGbits.PCFG11 = 1; //No vull entrada anal�gica (AN11)!!
    CNPU1bits.CN13PUE = 1;
    timerRF = TiGetTimer();
    
    estatRF =  sincronized =  pos = caracter = pulses = 0;
    initCadenaRF(id_trama, MAX_ID);
    
}

void addBit(char input, char rotation){
    
    //Post: Afegeix un 1 i rota cap a la dreta, en el MSB, en cas de 
    //que input valgui 1 o rota cap a la dreta altrament
    if (input == 1) {
        
        inValue = (inValue >> rotation) & 0x7F ;
        
    }else{
        
        inValue = (rotation == 1 ? (((inValue | 0x80) >> rotation) & (0x7F)) : (inValue | 0x80)) ;
        
    }
}

inline void exitStateInstructions(){
    //Post: Reseteja les variables inValue i pos. Afegeix el
    //bit i fa el resettics
    
    inValue = pos = 0;
    
    addBit(input,1);
    pos++;  
    
    TiResetTics(timerRF);
    
}

char comprovaID(){
  //Post: Retorna 1 en cas de que el id de la trama sigui el mateix que 
  //el ID que ha introduit l'usuari, 0 altrament.   
                
  return ((getIDPos(2) == id_trama[2]) && getIDPos(1) == id_trama[1] && getIDPos(0) == id_trama[0]);
    
}

void MotorRF () {
    
	switch(estatRF) {
        
        case 0:

            if(IN == 1){

                TiResetTics(timerRF);
                estatRF = 1;

            }

            break;
        case 1:

            if(IN == 0){

                if(TiGetTics(timerRF) > 40){

                    estatRF = 2;
                    TiResetTics(timerRF);

                }else{

                    estatRF = 0;

                }

            }
            break;
        case 2:

            if(IN == 1){

                if((TiGetTics(timerRF)) < 60){
                    
                    pulses++;
                    TiResetTics(timerRF);
                    estatRF = 1;

                }else{

                    if(pulses > 6){
                       
                        if(TiGetTics(timerRF) < 145){

                            pulses = 0;
                            estatRF = 0;

                        }else{

                            sincronized = inValue = pos = pulses = caracter = signal = 0;
                            TiResetTics(timerRF);
                            estatRF = 3;

                        } 
                        
                    }else{
                        
                        pulses = 0;
                        estatRF = 0;
                        
                    }
                }
            }

            break;
        case 3:

            if(TiGetTics(timerRF) > 10 && TiGetTics(timerRF) < 20){

                input = IN;

            }

            if(IN == !input && TiGetTics(timerRF) > 20){
                      
                TiResetTics(timerRF);
                estatRF = 4;

            }

            break;
        case 4:

            if(TiGetTics(timerRF) > 49){ 

                if(pos < 8){

                    estatRF = 3;
                    pos++;  
                    
                    if(pos == 8)addBit(input,0);
                    else addBit(input,1);
                    
                    TiResetTics(timerRF);

                }else{
                    
                    if (sincronized == 1){
 
                        estatRF = 5;

                    }else{
                        
                        if(sincronized == 0)sincronized = 10;
                        estatRF = 6;

                    }

                }
            }
            break;
        case 5:

            //TODO
            if(caracter > 159  && signal < 4){

                sincronized = 0;
                estatRF = 8;

            }else if(caracter > 309){

                sincronized = 0;
                estatRF = 8;
                
            }else{
                
                *((getSignal(0))+ caracter) = inValue;
                caracter++;
                estatRF = 3;

            }

            exitStateInstructions();

            break;
        case 6:
            
            if(sincronized == 10 && inValue == 64){
                
                sincronized = 2;
                estatRF = 3;
                caracter = 0;
                exitStateInstructions();
                TiResetTics(timerRF2);
                
            }else{
                
                if(sincronized == 2){

                    if (caracter < 3 ){

                        id_trama[caracter] = inValue;
                        caracter++;
                        estatRF = 3;
                        exitStateInstructions();

                    }else{

                        if(sincronized != 4){

                            estatRF = 7;

                        }else {

                            estatRF = 0;
                            sincronized = 0;

                        }

                    }

                }else{

                    estatRF = 0;
                    sincronized = 0;

                }
            }

            break;
        case 7:
            
            if(comprovaID()){
                    
                char aux = 0;
                
                sincronized = 1;
                estatRF = 3;
                
                *((getSignal(0))+ caracter) = inValue;
                
                signal = ((aux = (inValue > 8 ? inValue - 12 : inValue - 1)) == 7 ? 0 : aux);
                
                caracter++;

            }else{

                estatRF = 0;
                sincronized = 4;

            }
            
            exitStateInstructions();
            
            break;
        case 8:
            
            updateFunctionValues();
            setFunctionSignal(signal);
            setFunctionLength(caracter - EXTRA_SPACES);
            estatRF = 9;
            
            break;
            
        case 9: 
            
            newSignal(signal);
            updateFrecuency();
            estatRF = 10;
            
            break;
            
        case 10:
            
            sincronized = 0;
            estatRF = 11;
            
            setAudioPeriode(calcula_periode(id_trama[0]));
            changeAudioStatus();
            showFunction();
            
            TiResetTics(timerRF);
            break;
            
        case 11:
                        
            if (TiGetTics(timerRF) > 10000){
                
                sincronized++;
                
                if(sincronized == 3){
                    
                    sincronized = 0;
                    estatRF = 0;
                    changeAudioStatus();
                    
                }else{
                    
                    setAudioPeriode(calcula_periode(id_trama[sincronized]));

                    TiResetTics(timerRF);
                    
                }
                
                
                
            }
            
            break;
    }
}


int getLength(){
    return caracter;
}