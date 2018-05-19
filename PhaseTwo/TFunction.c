#include "TFunction.h"
#include "time.h"
#include <xc.h>


//
//--------------------------------CONSTANTS---AREA-----------


//
//---------------------------End--CONSTANTS---AREA-----------
//

//
//--------------------------------VARIABLES---AREA-----------
//

static unsigned char frequency[5];

static unsigned int timerFunction, periode, length;
static unsigned char estatFunction,vpp, offset, signal, vmin, vmax, vavg;
static unsigned char missatge[MAX_SIGNAL];

//
//---------------------------End--VARIABLES---AREA-----------
//
//
//--------------------------------PRIVADES----AREA-----------
//


void convertFrecuency(int num){
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

//
//---------------------------End--PRIVADES----AREA-----------
//

//
//--------------------------------PUBLIQUES---AREA-----------
//



void initTFunction(){
    signal = length = 0;
    vpp = offset = vmin = vmax = vavg = ' ';
    
    frequency[0] = '1';
    frequency[1] = '0';
    frequency[2] = '0';
    frequency[3] = '0';
    frequency[4] = 0;
    
    timerFunction = TiGetTimer();
    
}

void MotorFunction(){
    switch(estatFunction){
        case 0:
            if (TiGetTics(timerFunction)>=periode){
                TiResetTics(timerFunction);
                estatFunction = 1;
            }
            break;
        case 1:
            if (TiGetTics(timerFunction)>=periode){
                TiResetTics(timerFunction);
                estatFunction = 0;
            }
            break;
        case 2:
            //Callo
            break;
    }
}

void set_function_values(char fvpp, char foffset, char fvmin, char fvmax, char fvavg, char ffrequency, char fsignal){
    
    vpp = fvpp;
    offset = foffset;
    vmin = fvmin;
    vmax = fvmax;
    vavg = fvavg;
    signal = fsignal;
    
    
    convertFrecuency(ffrequency * 10);
    frequency[4] += '0';
    frequency[3] += '0';
    frequency[2] += '0';
    frequency[1] += '0';
    
}
//
//---------------------------End--PUBLIQUES---AREA-----------
//

char getVpp(){
    
    return vpp;
 
}

char getOffset(){
    
    return offset;
    
}

char getFrequency(char pos){
    return *(frequency + pos);
}

char getTypeSignal(){
    
    return signal;
}

char getVMin(){
    return vmin;
}

char getVMax(){
    return vmax;
}

char getVAvg(){
    return vavg;
}

unsigned char* getSignal(unsigned char offset){
    return (missatge+offset);
}

void setFunctionLength(int flength){
    length = flength;
}

int getFunctionLength(){
    return length;
}

void setFunctionSignal(char value){
    signal = value;
}