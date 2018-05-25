#include "TFunction.h"
#include "time.h"
#include <xc.h>



#define WRITE_ENABLE            LATAbits.LATA4=0
#define WRITE_DISABLE         LATAbits.LATA4=1
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
    vpp = offset =  ' ';
    
    vmin[1] = vmax[1] = vavg[1] = '\0';
    frequency[0] = '1';
    frequency[1] = '0';
    frequency[2] = '0';
    frequency[3] = '0';
    frequency[4] = 0;
    
    TRISBbits.TRISB10 = TRISBbits.TRISB11 = TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = TRISBbits.TRISB14 = TRISBbits.TRISB15 = 0;
    TRISAbits.TRISA2 = TRISAbits.TRISA3 = TRISAbits.TRISA4 = 0;
    
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
    LATBbits.LATB10 = ( missatge[lecture] & 0x1  );
    LATBbits.LATB11 = ( missatge[lecture] & 0x2  );
    LATBbits.LATB12 = ( missatge[lecture] & 0x4  );
    LATBbits.LATB13 = ( missatge[lecture] & 0x8  );
    LATBbits.LATB14 = ( missatge[lecture] & 0x01 );
    LATBbits.LATB15 = ( missatge[lecture] & 0x20 );
    LATAbits.LATA2  = ( missatge[lecture] & 0x40 );
    LATAbits.LATA3  = ( missatge[lecture] & 0x80 );
    WRITE_DISABLE;    
    
}

void MotorFunction(){
    switch(estatFunction){
        case 0:   
            break;
            
        case 1:   
            
            lecture = 0;
            TiResetTics(timerFunction);
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

void set_function_values(char fvpp, char foffset, char fvmin, char fvmax, char fvavg, char ffrequency, char fsignal){
    
    vpp = fvpp;
    offset = foffset;
    vmin[0] = fvmin;
    vmax[0] = fvmax;
    vavg[0] = fvavg;
    signal = fsignal;
    
    periode = ffrequency * 10;
    
    
    convertFrecuency(periode);
    
    periode = ((ffrequency * 10) * 4 / 2550);
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

Senyal* getSignals(){
    return senyals;
}

void newSignal(char type){
    Senyal *auxiliar = (senyals + (type > 4 ? type - 12 : type - 1));
    
    auxiliar->show_times = actualitzaTemps(auxiliar->times, auxiliar->show_times, MAX_TIMES - 1);
}