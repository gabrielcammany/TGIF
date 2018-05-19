/*
 * File:   TRF.c
 * Author: Gabriel Cammany
 *
 */

#include "TRF.h"

#define END_BYTE                0x00
#define START_BYTE              0x40
#define IN                      PORTBbits.RB13
#define MAX_ID_RF               3

static unsigned char estatRF,input, timerRF, sincronized, pos, missatge[MAX_SIGNAL];
static unsigned char inValue,id_trama[MAX_ID_RF];

static int caracter;

void initCadenaRF(unsigned char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        cadena[i] = ' ';
    }
    
}


void RFInit(){
    
    TRISBbits.TRISB13 = 1;
    AD1PCFGbits.PCFG11 = 1; //No vull entrada analògica (AN11)!!
    CNPU1bits.CN13PUE = 1;
    timerRF = TiGetTimer();
    
    estatRF =  sincronized =  pos = caracter = 0;
    initCadenaRF(id_trama, MAX_ID);
    
}

void addBit(char input){
    
    //Post: Afegeix un 1 i rota cap a la dreta, en el MSB, en cas de 
    //que input valgui 1 o rota cap a la dreta altrament
    if (input == 1) {
        
        inValue = (inValue >> 1) & 0x7F ;
        
    }else{
        
        inValue = ((inValue | 0x80) >> 1) & 0x7F ;
        
    }
}


void fiTramaPropia(){
        
    set_function_values(missatge[4] + '0',missatge[5] + '0',missatge[6] + '0',
            missatge[7] + '0',missatge[8] + '0', missatge[3],missatge[2]);
    
}

inline void exitStateInstructions(){
    //Post: Reseteja les variables inValue i pos. Afegeix el
    //bit i fa el resettics
    
    inValue = pos = 0;
    addBit(input);
    TiResetTics(timerRF);
    
}

char comprovaID(){
  //Post: Retorna 1 en cas de que el id de la trama sigui el mateix que 
  //el ID que ha introduit l'usuari, 0 altrament.
    
  return (getIDPos(0) == id_trama[0] && getIDPos(1) == id_trama[1] && getIDPos(2) == id_trama[2]);
    
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

                if(TiGetTics(timerRF) > 4){

                    estatRF = 2;
                    TiResetTics(timerRF);

                }else{

                    estatRF = 0;

                }

            }
            break;
        case 2:

            if(IN == 1){

                if(TiGetTics(timerRF) < 5){

                    estatRF = 0;

                }else{

                    if(TiGetTics(timerRF) < 14){

                        estatRF = 0;

                    }else{

                        inValue = 0;
                        pos = 0;
                        TiResetTics(timerRF);
                        estatRF = 3;
                        caracter = 0;

                    }
                }
            }

            break;
        case 3:

            if(TiGetTics(timerRF) == 1){

                input = IN;

            }

            if(IN == !input && TiGetTics(timerRF) > 1){

                TiResetTics(timerRF);
                estatRF = 4;

            }

            break;
        case 4:

            if(TiGetTics(timerRF) > 4){ 

                if(pos<7){

                    estatRF = 3;
                    pos++;
                    addBit(input);
                    TiResetTics(timerRF);

                }else{

                    if (sincronized == 1){

                        estatRF = 5;

                    }else{

                        sincronized = 2;
                        estatRF = 6;
                        caracter = 0;

                    }

                }
            }

            break;
        case 5:

            if(inValue == END_BYTE){

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
                
            break;
        case 7:
            
            if(comprovaID()){

                sincronized = 1;
                estatRF = 3;
                *((getSignal(0))+ caracter) = inValue;
                caracter++;

            }else{

                estatRF = 0;
                sincronized = 4;

            }
            
            exitStateInstructions();
            
            break;
        case 8:
            
            fiTramaPropia();
            sincronized = 0;
            estatRF = 9;
            
            setFunctionLength(caracter - EXTRA_SPACES);
            setAudioPeriode(calcula_periode(id_trama[sincronized]));
            changeAudioStatus();
            
            TiResetTics(timerRF);
            
            break;
            
        case 9:
                        
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