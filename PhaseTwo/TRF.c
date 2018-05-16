/*
 * File:   TRF.c
 * Author: Gabriel Cammany
 *
 */

#include "TRF.h"

#define END_BYTE                0x00
#define START_BYTE              0x40
#define IN                      PORTBbits.RB13

static unsigned char estatRF,input, timerRF, sincronized, pos, temp[10],
        missatge[MAX_SIGNAL], inValue,id_trama[MAX_ID], frequency[5], vpp, offset, signal;
static int caracter;
static Senyal senyals[6];

void initCadenaRF(unsigned char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        cadena[i] = ' ';
    }
    
}


void myItoa(int num){
    //Post: escriu el valor ascii de num a tmp;
    frequency[4] = (char)(num/1000);
    num = num - (frequency[4]*1000);
    frequency[3] = (char)(num /100);
    num = num - (frequency[3]*100);
    frequency[2] = (char) (num /10);
    num = num - (frequency[2]*10);
    frequency[1] = num;
    frequency[0] = 0;

}



void RFInit(){
    
    TRISBbits.TRISB13 = 1;
    AD1PCFGbits.PCFG11 = 1; //No vull entrada analògica (AN11)!!
    CNPU1bits.CN13PUE = 1;
    timerRF = TiGetTimer();
    
    estatRF =  sincronized =  pos = signal = 0;
    initCadenaRF(id_trama, MAX_ID);
    initCadenaRF(frequency, 4);
    vpp = offset = ' ';
    
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

void setFrequency(unsigned char recieved){
    
    myItoa(recieved * 10);
    frequency[4] += '0';
    frequency[3] += '0';
    frequency[2] += '0';
    frequency[1] += '0';
    
}


void fiTramaPropia(){
    
    signal = missatge[3];
    setFrequency(missatge[4]);
    vpp = missatge[5] + '0';
    offset = missatge[6] + '0';
    
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

                        estatRF = 6;

                    }

                }
            }

            break;
        case 5:

            if(inValue == END_BYTE){

                sincronized = 0;
                estatRF = 8;

            }else{

                missatge[caracter] = inValue;
                caracter++;
                estatRF = 3;

            }

            exitStateInstructions();

            break;
        case 6:

            if(inValue == START_BYTE){

                sincronized = 2;
                estatRF = 3;
                caracter = 0;
                exitStateInstructions();

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

                sincronized = 1;
                estatRF = 3;
                caracter = 16;
                missatge[caracter] = inValue;
                caracter++;

            }else{

                estatRF = 0;
                sincronized = 4;

            }
            
            exitStateInstructions();
            
            break;
        case 8:
            
            fiTramaPropia();
            estatRF = 0;
            
            break;
    }
}

unsigned char* getSignal(unsigned char offset){
    return (missatge+offset);
}

int getLength(){
    return caracter;
}


char getFrequency(char index){
    
    return frequency[index];
    
}

char getVpp(){
    
    return vpp;
    
}

char getOffset(){
    
    return offset;
    
}
