#include "TFunction.h"
#include "time.h"
#include <xc.h>



#define WRITE_ENABLE            LATAbits.LATA3=0
#define WRITE_DISABLE         LATAbits.LATA3=1
//
//--------------------------------CONSTANTS---AREA-----------


//
//---------------------------End--CONSTANTS---AREA-----------
//

//
//--------------------------------VARIABLES---AREA-----------
//

static unsigned char frequency[5];

static unsigned int timerFunction, periode, length, lecture;
static unsigned char estatFunction,vpp, offset, signal;
static unsigned char missatge[MAX_SIGNAL];
static char vmin[2], vmax[2], vavg[2];

static Senyal senyals[MAX_SIGNALS];

//
//---------------------------End--VARIABLES---AREA-----------
//
//
//--------------------------------PRIVADES----AREA-----------
//

void initSTime(char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        if(i == size -1){
            cadena[i] = 0;
        }else{
            cadena[i] = '0';
        }
    }
    
}

void initSignals(){
    int i;
    for(i = 0; i < MAX_SIGNALS; i++){
        senyals[i].periods = 0;
        initSTime(senyals[i].times, MAX_TIMES);
        senyals[i].show_times = MAX_TIMES - 2;
        
    }
    
}
void convertFrecuency(int num){
    //Post: escriu el valor ascii de num a tmp;
    frequency[4] = 0;
    frequency[3] = (num > 999 ? (char)(num/1000) : 0);
    
    num = num - (frequency[3] ? (frequency[3]*1000) : 0);
    
    frequency[2] = (num > 99 ? (char)(num/100): 0);
    
    num = num - (frequency[2] ? (frequency[2]*100) : 0);
    frequency[1] = (char) (num /10);
    
    num = num - (frequency[1]*10);
    frequency[0] = num;

}

//
//---------------------------End--PRIVADES----AREA-----------
//

//
//--------------------------------PUBLIQUES---AREA-----------
//



void initTFunction(){
    signal = length = 0;
    vpp = offset =  ' ';
    
    vmin[1] = vmax[1] = vavg[1] = '\0';
    frequency[0] = '1';
    frequency[1] = '0';
    frequency[2] = '0';
    frequency[3] = '0';
    frequency[4] = 0;
    
    /*0 - B14
    1 - RB6
    2 - RB12
    3 - RB11
    4 - RA4
    5 - RB7
    6 - RA2
    7 - RB10
    WR - RA3*/

    
    TRISBbits.TRISB6 = TRISBbits.TRISB11 = TRISBbits.TRISB12 = TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB10 = TRISAbits.TRISA3 = TRISBbits.TRISB7 = 0;
    TRISAbits.TRISA2 = TRISAbits.TRISA4 = 0;
    
    AD1PCFGbits.PCFG0 = AD1PCFGbits.PCFG1 = AD1PCFGbits.PCFG9 = 1; //No vull entrada analògica (AN11)!!
    AD1PCFGbits.PCFG10 = AD1PCFGbits.PCFG11 = AD1PCFGbits.PCFG12 = 1; //No vull entrada analògica (AN12)!!
    
    initSignals();
    
    timerFunction = TiGetTimer();
    
}

void setFunctionPeriod(char period){
    
    periode = period * 10;
    
}

void changeValue(){
    
    
    WRITE_ENABLE;
    LATBbits.LATB14 = ( missatge[lecture] & 0x1  );
    LATBbits.LATB6 = ( missatge[lecture] & 0x2  );
    LATBbits.LATB12 = ( missatge[lecture] & 0x4  );
    LATBbits.LATB11 = ( missatge[lecture] & 0x8  );
    LATAbits.LATA4 = ( missatge[lecture] & 0x01 );
    LATBbits.LATB7 = ( missatge[lecture] & 0x20 );
    LATAbits.LATA2  = ( missatge[lecture] & 0x40 );
    LATBbits.LATB10  = ( missatge[lecture] & 0x80 );
    WRITE_DISABLE;    
    
}

void showFunction(){
    
    estatFunction = 1;
    
}

void stopFunction(){
    
   estatFunction = 0;
    
}

void MotorFunction(){
    switch(estatFunction){
        case 0:   
            break;
            
        case 1:   
            
            lecture = 0;
            TiResetTics(timerFunction);
            estatFunction = 2;
            break;
            
        case 2:
            
            if (TiGetTics(timerFunction)>=periode){
                
                estatFunction = 3;
                lecture++;
                if(lecture == length)lecture = 0;
                
            }
            
            break;
        case 3:
            
            changeValue();
            TiResetTics(timerFunction);
            estatFunction = 2;
            
            break;
    }
}

void updateFunctionValues(){
        
    vpp = *(missatge+5) + '0';
    offset = *(missatge+6) + '0';
    *vmin = *(missatge+7) + '0';
    *vmax = *(missatge+8) + '0';
    *vavg = *(missatge+9) + '0';
        
    periode = (*(missatge+4)) * 10;
    convertFrecuency(periode);
    
    frequency[3] += '0';
    frequency[2] += '0';
    frequency[1] += '0';
    frequency[0] += '0';
    
}

void updateFrecuency(){
    
    periode = (periode * 4 / 2550);
    
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

char* getVMin(){
    return vmin;
}

char* getVMax(){
    return vmax;
}

char* getVAvg(){
    return vavg;
}

void setVpp(char value){
    vpp = value;
}

void setOffset(char value){
    offset = value;
}

void setVMin(char value){
    vmin[0] = value;
}

void setVMax(char value){
    vmax[0] = value;
}

void setVAvg(char value){
    vavg[0] = value;
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

char getFunctionSignal(){
    return signal;
}

Senyal* getSignals(){
    return senyals;
}

void newSignal(char type){
    Senyal *auxiliar = (senyals + type);
    auxiliar->show_times = actualitzaTemps(auxiliar->times, auxiliar->show_times, MAX_TIMES - 1);
}