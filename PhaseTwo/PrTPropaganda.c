
#include "TRF.h"

#include "PrTPropaganda.h"


static long timestamp, signal_avg;
static unsigned int timerPropaganda,timerAudioPropaganda, index_function;
static unsigned char audioStatus, estatPropaganda, reset_time = 4, 
        signal_index, signal_list, opcio, identificador, bit_time,
        signal_max, signal_min;
static char rtime[6],
        id[MAX_ID],
        btime[MAX_BIT_TIME];


void initCadena(char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        if(i == size -1){
            cadena[i] = 0;
        }else{
            cadena[i] = ' ';
        }
    }
    
}

void clearCadena(char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        cadena[i] = 0;
    }
    
}

void initCTime(char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        if(i == size -1){
            cadena[i] = 0;
        }else{
            cadena[i] = '0';
        }
    }
    
}


void actualitzaTemps(){
    //Post: Incrementa el ID que esta per pantalla si no hi ha cap afegit, en cas
    //contrari, incrementa fins arribar al numero
    
    switch(reset_time){
        
        case 4:
            
            reset_time = (rtime[4] == '9' ? reset_time-1 : reset_time); 
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            if(reset_time != 4){
                rtime[3] = '1';
            }
            
            
            break;
        case 3:
            
            rtime[3] = (rtime[4] == '9' ? (rtime[3] + 1) : rtime[3]);
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            
            reset_time = (rtime[3] == '9' ? reset_time-1 : reset_time); 
            
            if(reset_time != 3){
                rtime[3] = '0';
                rtime[2] = '1';
            }
            break;
        case 2:
            rtime[3] = (rtime[4] == '9' ? (rtime[3] + 1) : rtime[3]);
            rtime[2] = (rtime[3] > '9' ? (rtime[2] + 1) : rtime[2]);
            
            rtime[3] = (rtime[3] > '9' ? '0' : rtime[3]);
            rtime[4] = (rtime[4] <'9' ? (rtime[4] + 1) : '0');
            
            reset_time = (rtime[2] == '9' ? reset_time-1 : reset_time); 
            
            if(reset_time != 3){
                rtime[3] = '0';
                rtime[2] = '0';
                rtime[1] = '1';
            }
            break;
        case 1:
            
            rtime[1] = (rtime[2] == '9' ? (rtime[1] + 1) : rtime[1]);
            rtime[2] = (rtime[2] == '9' ? '0' : rtime[2]);
            rtime[2] = (rtime[3] == '9' ? (rtime[2] + 1) : rtime[2]);
            rtime[3] = (rtime[4] == '9' ? (rtime[3] + 1) : rtime[3]);
            rtime[3] = (rtime[3] == '9' ? '0' : rtime[3]);
            rtime[4] = (rtime[4] <'9' ? (rtime[4] + 1) : '0');
            
            reset_time = (rtime[2] == '9' ? reset_time-1 : reset_time); 
            
            if(reset_time != 3){
                rtime[2] = '1';
            }
            
            
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            rtime[3] = (rtime[3]<'9' ? (rtime[3] + 1) : '0');
            rtime[2] = (rtime[2]<'9' ? (rtime[2] + 1) : '0');
            reset_time = (rtime[2] == '9' ? reset_time-1 : reset_time); 
            rtime[1] = (rtime[1]<'9' ? (rtime[1] + 1) : '0');
            break;
        case 0:
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            rtime[3] = (rtime[3]<'9' ? (rtime[3] + 1) : '0');
            rtime[2] = (rtime[2]<'9' ? (rtime[2] + 1) : '0');
            rtime[1] = (rtime[1]<'9' ? (rtime[1] + 1) : '0');
            rtime[0] = (rtime[0]<'9' ? (rtime[0] + 1) : '0');
            break;
        
    }
    
}

char getIDPos(unsigned char pos){
    return id[pos];
}

void setIDPos(int pos, unsigned char value){
    id[pos] = value;
}

char* detect_signal(char signal){
    
    switch(signal){
        
        case 0:
            return "-Sines\r\n\0";
        case 1:
            return "-Pulses\r\n\0";
        case 2:
            return "-Sawtooth\r\n\0";
        case 3:
            return "-Triangles\r\n\0";
        case 4:
            return "-Random\r\n\0";
        case 5:
            return "-Custom\r\n\0";
        
    }
    
    return "-Unknown\r\n\0";
    
}

char* signal_config(char signal){
    
    return (signal > 3 ? "\r\t\t* 24.700 miliseconds configuration frame\r\n\0" : 
        "\r\t\t* 12.720 miliseconds configuration frame\r\n\0");
    
}

inline void Statistics(void){
    
    SiPutsCooperatiu("\r\n\r************\r\n\0");
    SiPutsCooperatiu("\r Statistics: \r\n\0");
    SiPutsCooperatiu("\r************\r\n\0");
    SiPutsCooperatiu("\r\nGenerated Functions:\r\n\0");
    
}

inline void function_values(void){
    
    SiPutsCooperatiu("\r\n\r************************\r\n\0");
    SiPutsCooperatiu("\r Current Function Values: \r\n\0");
    SiPutsCooperatiu("\r************************\r\n\0");
    SiPutsCooperatiu("\rMinimum value ");
    if(getVMin() != ' '){
        SiSendChar(getVMin());
        SiPutsCooperatiu("V\r\n\0");
    }else{
        SiPutsCooperatiu("-\r\n\n\0");
    }
    SiPutsCooperatiu("\rMaximum value ");
    if(getVMin() != ' '){
        SiSendChar((getVMax()));
        SiPutsCooperatiu("V\r\n\0");
    }else{
        SiPutsCooperatiu("-\r\n\n\0");
    }
    SiPutsCooperatiu("\rAverage value ");
    if(getVMin() != ' '){
        SiSendChar(getVAvg());
        SiPutsCooperatiu("V\r\n\n\0");
    }else{
        SiPutsCooperatiu("-\r\n\n\0");
    }
    
}

inline void Menu(void){
    SiPutsCooperatiu("\r\n\r************\r\n\0");
    SiPutsCooperatiu("\r Main Menu: \r\n\0");
    SiPutsCooperatiu("\r************\r\n\0");
    SiPutsCooperatiu("\r\nChoose an option:\r\n\0");
    SiPutsCooperatiu("\r\t1-Configuration of the reception parameters\r\n\0");
    SiPutsCooperatiu("\r\t2-Show statistics\r\n\0");
    SiPutsCooperatiu("\r\t3-Override function\r\n\0");
    SiPutsCooperatiu("\r\t4-Current function values\r\n\0");
    SiPutsCooperatiu("\r\n");
}

void initPropaganda(void){
    //Pre: La sio està inicialitzada
    Menu();
    timestamp = 0;
    timerPropaganda= TiGetTimer();
    timerAudioPropaganda= TiGetTimer();
    estatPropaganda = 0;
    audioStatus = 0;
    identificador  = bit_time = 0;
    
    initCadena(id,MAX_ID);
    clearCadena(btime,MAX_BIT_TIME);
    
    muteAudio();
    
    initCTime(rtime,6);
    
}

inline void playOptionMusic(){
    
    if(audioStatus < 3){
        if (TiGetTics(timerAudioPropaganda) > UN_SEGON/2){
            TiResetTics(timerAudioPropaganda);
            setAudioPeriode(getAudioPeriode() + 300);
            audioStatus++;
            if(audioStatus == 3){
                muteAudio();
            }
        } 
    }
    
}

void MotorPropaganda(void){

    switch(estatPropaganda){
        case 0:            
            if (SiCharAvail() != 0){
                opcio= SiGetChar();
                
                if ((opcio <= '4') && (opcio >= '1')){
                    
                    estatPropaganda = opcio-'0';
                    
                    setAudioPeriode(10 * estatPropaganda);
                    unMuteAudio();
                    SiSendChar('\r');
                    SiSendChar(opcio);
                    SiSendChar('\n');
                    
                    if(estatPropaganda == 1){
                        SiPutsCooperatiu(INSERT_BIT_TIME);
                    }
                    
                    TiResetTics(timerAudioPropaganda);
                    
                }else{
                    
                    SiSendChar('\r');
                    SiSendChar(opcio);
                    
                }
                
            }
            
            if (TiGetTics(timerPropaganda) > UN_SEGON){
                TiResetTics(timerPropaganda);
                if (++timestamp == UN_SEGON*3) timestamp=0;
                actualitzaTemps();
            }
            
            break;
        case 1:
                        
            if (SiCharAvail() != 0){
                
                if ((opcio = SiGetChar()) == 27){
                    
                    Menu();
                    estatPropaganda = audioStatus = 0;
                    identificador = 0;
                    bit_time = 0;
                    
                } else if (opcio == '\r'){
                    
                    estatPropaganda=11;
                    bit_time = 0;
                    SiPutsCooperatiu(INSERT_ID);
                    
                }else{
                    
                    if(bit_time > 2){
                        bit_time = 0;
                        SiSendChar('\r');
                        SiSendChar(opcio);
                    }else{
                        SiSendChar(opcio);
                    }
                    btime[bit_time] = opcio;
                    bit_time++;
                    
                }
                
            }
            
            playOptionMusic();
            
            
            if (TiGetTics(timerPropaganda) > UN_SEGON){
                TiResetTics(timerPropaganda);
                if (++timestamp == UN_SEGON*3) timestamp=0;
                actualitzaTemps();
            } 
            break;

        case 2:
            
            Statistics();
            
            signal_index = 0;
            signal_list = 0;
            estatPropaganda=21;
            
            playOptionMusic();
            break;
            
        case 3:
            
            TiResetTics(timerPropaganda);
            
            SiPutsCooperatiu("\n\n\r*******************\n\r\0");
            SiPutsCooperatiu("\rGetting function values...\n\r\0");
            SiPutsCooperatiu("\r*******************\n\r\0");
            estatPropaganda=31;
            index_function = EXTRA_SPACES - 1;
            signal_max = signal_min = signal_avg = 0;
            
            playOptionMusic();
            break;
        case 4:
            
            function_values();
            
            Menu();
            
            estatPropaganda = 41;
            
            break;
            
        case 11:
            
            if (SiCharAvail() != 0){
                
                if ((opcio = SiGetChar()) == 27){
                    
                    Menu();
                    estatPropaganda = audioStatus = 0;
                    identificador = 0;
                    bit_time = 0;
                    
                } else if (opcio == '\r'){
                    
                    Menu();
                    estatPropaganda = audioStatus = 0;
                    identificador = 0;
                    bit_time = 0;
                    
                }else{
                    
                    if(identificador > 2){
                        identificador = 0;
                        SiSendChar('\r');
                        SiSendChar(opcio);
                    }else{
                        SiSendChar(opcio);
                    }
                    id[identificador] = opcio;
                    identificador++;
                    
                }
                
            }
            
            
            playOptionMusic();
            
            
            break;
        case 21:
            
            if(signal_index == MAX_SIGNALS){
                
                estatPropaganda = 22;
                
            }else{
                
                if( (*(getSignals()+signal_index)).times[0] != 0){
                    
                    signal_list++;
                    
                    SiPutsCooperatiu("\r\t");
                    SiSendChar(signal_list + '0');
                    SiPutsCooperatiu(detect_signal(signal_index));
                    SiPutsCooperatiu("\r\t\t* Received ");
                    SiPutsCooperatiu((*(getSignals()+signal_index)).times);
                    SiPutsCooperatiu(" times\r\n\0");
                    SiPutsCooperatiu("\r\t\t* Generated ");
                    SiPutsCooperatiu((*(getSignals()+signal_index)).periods);
                    SiPutsCooperatiu(" periods\r\n\0");
                    SiPutsCooperatiu(signal_config(signal_index));
                    
            
                }
                
                signal_index++;
                
            }
            
            playOptionMusic();
            
            
            break;
            
        case 22:
            
           
            if(signal_list == 0){
                
                SiPutsCooperatiu("\n\r\t\tNo signals generated! \n\n\r\0");
                
            }
            
            SiPutsCooperatiu("\rID: ");
            if(id[0] != ' '){
                SiPutsCooperatiu(id);
                SiPutsCooperatiu("\n\r\0");
            }else{
                SiPutsCooperatiu("No ID set\n\r\0");
            }
            SiPutsCooperatiu("\rBit time: ");
            if(btime[0] != 0){
                SiPutsCooperatiu(btime);
                SiPutsCooperatiu(" milliseconds\n\r\0");
            }else{
                SiPutsCooperatiu("No bit time set\n\r\0");
            }
            SiPutsCooperatiu(TIMESTAMP);
            SiPutsCooperatiu(rtime+reset_time);
            SiPutsCooperatiu(TIMESTAMP_SECOND);
            estatPropaganda = 23;
            
            playOptionMusic();
            
            break;
        case 23:
            
            if (SiCharAvail() != 0){
                
                if ((opcio = SiGetChar()) == 27){
                    
                    Menu();
                    estatPropaganda = audioStatus = 0;
                    
                }
                
            }
            
            playOptionMusic();
            
            if (TiGetTics(timerPropaganda) > UN_SEGON){
                
                TiResetTics(timerPropaganda);
                
                if (++timestamp == UN_SEGON*3) timestamp=0;
                
                actualitzaTemps();
                
                SiPutsCooperatiu(TIMESTAMP);
                SiPutsCooperatiu(rtime+reset_time);
                SiPutsCooperatiu(TIMESTAMP_SECOND);
                
            }
            
            
            break;
            
        case 31:
            
            playOptionMusic();
            
            if (TiGetTics(timerPropaganda) > (UN_SEGON/100)){
                
                if(index_function < MAX_SIGNAL){
                    
                    *(getSignal(0) + (index_function)) = (char) AdGetMostra();
                    if( *(getSignal(0) + (index_function)) > signal_max){
                        signal_max = *(getSignal(0) + (index_function));
                    }
                    if( *(getSignal(0) + (index_function)) < signal_min){
                        signal_min = *(getSignal(0) + (index_function));
                    }
                    signal_avg = signal_avg + *(getSignal(0) + (index_function));
                    
                    SiSendChar(*(getSignal(0) + (index_function)));
                    index_function++;
                    
                }else {
                    
                    
                    setFunctionSignal(CUSTOM);
                    setFunctionLength(MAX_SIGNAL - EXTRA_SPACES);
                    muteAudio();
                    estatPropaganda = audioStatus = 0;
                    Menu();
                    
                }
                
                TiResetTics(timerPropaganda);
                
            }
            
            
            
            break;
            
        case 41:
            
            if(audioStatus < 3){
                
                if (TiGetTics(timerAudioPropaganda) > (UN_SEGON/2)){
                    TiResetTics(timerAudioPropaganda);
                    setAudioPeriode(getAudioPeriode() + 300);
                    audioStatus++;
                    if(audioStatus == 3){
                        muteAudio();
                        audioStatus = estatPropaganda=0;
                    }
                } 
                
            }
            break;
    }

}

#define     MAXCOLUMNES 16
#define     MAXCADENES 2
#define     MAXSIGNALS 6

static char estatLCD = 0;
const unsigned char cadena[MAXCADENES][MAXCOLUMNES] = {{" Waiting for ID "},{"ID set          "}}; //Més val que tingui 16 caràcters...

unsigned char headerString[MAXSIGNALS][MAXCOLUMNES] = {{"SINE          Hz"},{"PULSE         Hz"},{"SAWTOOTH      Hz"},
{"TRIANGLE      Hz"},{"RANDOM        Hz"},{"CUSTOM        Hz"}};
unsigned char subheaderString[MAXCOLUMNES] = {"0 V.        0 V."};

static unsigned char timerLCD, i,j, quina, swap, header;
static char primeraLinia[MAXCOLUMNES], segonaLinia[MAXCOLUMNES];

void setCadena(char *cadenaToSet){
    int k;
    for(k = 0; k < MAXCOLUMNES; k++){
        *(cadenaToSet + k) = cadena [quina][k];
    }
}

void initMotorLCD(void){
    
    //Pre: El LCD està inicialitzat
    timerLCD = TiGetTimer();
    LcClear();
    //Hi ha caselles de la segona línia que sempre valdran el mateix, les preparo!
    //initCadena(primeraLinia);
    quina = swap = i = 0;
    
    initCadena(segonaLinia,MAXCOLUMNES);
    setCadena(primeraLinia);
    
}


void swapLinia(){
    
    if(swap < MAXCOLUMNES){
        
        segonaLinia[swap] = primeraLinia[swap];
        primeraLinia[swap++] = ' ';
        
        if(swap == MAXCOLUMNES)swap = MAXCOLUMNES * 2;
        
    }else{
        
        if(swap > MAXCOLUMNES){
            
            primeraLinia[swap - (MAXCOLUMNES + 1)] = segonaLinia[swap - (MAXCOLUMNES + 1)];
            segonaLinia[(swap--) - (MAXCOLUMNES + 1)] = ' ';
            
            if(swap == MAXCOLUMNES)swap = 0;
            
        }
        
    }
    
}

inline void prepareHeader(){
    
    headerString[header][13] = getFrequency(3);
    headerString[header][12] = getFrequency(2);
    headerString[header][11] = getFrequency(1);
    headerString[header][10] = getFrequency(0);
    
}

inline void prepareSubHeader(){
    subheaderString[0] = getVpp();
    subheaderString[12] = getOffset();
}


void MotorLCD(void){
    switch (estatLCD){
        case 0:
                    
            if(getFunctionLength() > 0){
                
                header = (getTypeSignal() > 4 ? getTypeSignal() - 12 : getTypeSignal() - 1);
                prepareHeader();
                
            }
            
            estatLCD = 1;
            
            break;
        case 1:
                   
            if(getFunctionLength() > 0){ 
                
                LcPutChar(headerString[header][j++]);
                
            }else{
                
                LcPutChar(primeraLinia[j++]);
                
            }
            
            if (j==16) j= 0;
            
            if (i++ > MAXCOLUMNES) {
                
                estatLCD = 2;
                TiResetTics(timerLCD);
                LcGotoXY(0,1);
                
            }
            
            break;

        case 2: //Preparo el string
            
            if(!quina){
                
                swapLinia();
                
                if(id[0] != ' ' && !identificador){
                    
                    if(id[1] != ' ' && id[2] != ' '){
                        
                        quina++;
                        swap = 3;
                        estatLCD = 3;
                        
                    }
                    
                }
                
                estatLCD = 4;
                
            }else{
                
                if(getFunctionLength() > 0){ 
                    
                    prepareSubHeader();
                    
                }
                
                estatLCD = 4;
            }
            
            break;
        case 3: 
            
            
            if(quina && swap == 3){
                
                setCadena(primeraLinia);
                *(primeraLinia + MAXCOLUMNES - 1) = id[2];
                *(primeraLinia + MAXCOLUMNES - 2) = id[1];
                *(primeraLinia + MAXCOLUMNES - 3) = id[0];
                swap = 2;
                initCadena(segonaLinia,MAXCOLUMNES);
                
            }else if(quina && swap == 2 && !identificador){
                
                *(primeraLinia + MAXCOLUMNES - 1) = id[2];
                *(primeraLinia + MAXCOLUMNES - 2) = id[1];
                *(primeraLinia + MAXCOLUMNES - 3) = id[0];
                
            }
            
            estatLCD = 4;
            
            break;
        case 4:
            
            if (TiGetTics(timerLCD)> 500){
                
                //Observo que si estresso molt al LCD arriba un punt que alguna
                //vegada pinta malament un caràcter. Deu tenir una cua interna?
                //si la té, aposto a que és de 24 posicions (mal número)...
                //Cap problema, donem 50 ms. de calma entre ràfega i ràfega i gas
                TiResetTics(timerLCD);
                i=0;
                estatLCD = 5;
                
            }
            
            break;
        case 5:
            
            if(getFunctionLength() > 0){ 
                
                LcPutChar(subheaderString[i]);
                    
            }else{
                
                LcPutChar(segonaLinia[i]);
                
            }
            
            if (i++ > (MAXCOLUMNES - 1)) {
                
                estatLCD = 6;
                TiResetTics(timerLCD);
                
            }
            
            break;
                
        case 6:
            
            if (TiGetTics(timerLCD) >= 2000){
                
                estatLCD = 0;
                LcGotoXY(0,0);
                j = i = 0;
                
            }
            
            break;
    }
}

/*
 *
 *          Fi  PROPAGANDAAAAAAAAAA!!!!
 *
 */

