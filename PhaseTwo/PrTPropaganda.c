#include "PrTPropaganda.h"


static long timestamp;
static unsigned char timerPropaganda, estatPropaganda, reset_time = 4;
static unsigned char rtime[5],
        id[MAX_ID],
        btime[MAX_BIT_TIME], opcio, identificador, bit_time;


void initCadena(unsigned char *cadena, unsigned char size){
    int i;
    for(i = 0; i < size; i++){
        cadena[i] = ' ';
    }
    
}

void actualitzaTemps(){
    //Post: Incrementa el ID que esta per pantalla si no hi ha cap afegit, en cas
    //contrari, incrementa fins arribar al numero
    
    switch(reset_time){
        
        case 4:
            
            reset_time = (rtime[4] == '9' ? reset_time-1 : reset_time); 
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            
            break;
        case 3:
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            reset_time = (rtime[3] == '9' ? reset_time-1 : reset_time); 
            rtime[3] = (rtime[3]<'9' ? (rtime[3] + 1) : '0');
            break;
        case 2:
            rtime[4] = (rtime[4]<'9' ? (rtime[4] + 1) : '0');
            rtime[3] = (rtime[3]<'9' ? (rtime[3] + 1) : '0');
            reset_time = (rtime[2] == '9' ? reset_time-1 : reset_time); 
            break;
        case 1:
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

inline void Statistics(void){
    
    SiPutsCooperatiu("\r\n\0\r************\r\n\0");
    SiPutsCooperatiu("\r Statistics: \r\n\0");
    SiPutsCooperatiu("\r************\r\n\0");
    SiPutsCooperatiu("\r\nGenerated Functions:\r\n\0");
    SiPutsCooperatiu("\r\t1-Configuration of the reception parameters\r\n\0");
    SiPutsCooperatiu("\r\t2-Show statistics\r\n\0");
    SiPutsCooperatiu("\r\t3-Override function\r\n\0");
    SiPutsCooperatiu("\r\t4-Current function values\r\n\0");
    SiPutsCooperatiu("\r\n");
    
}

inline void Menu(void){
    SiPutsCooperatiu("\r\n\0\r************\r\n\0");
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
    estatPropaganda = 0;
    identificador = 2;
    initCadena(id,MAX_ID);
    initCadena(btime,MAX_BIT_TIME);
    initCadena(rtime,5);
}

void MotorPropaganda(void){

    switch(estatPropaganda){
        case 0:
            
            if (SiCharAvail() != 0){
                opcio= SiGetChar();
                
                if ((opcio <= '4') && (opcio >= '1')){
                    
                    estatPropaganda = opcio-'0';
                    
                    SiSendChar('\r');
                    SiSendChar(opcio);
                    SiSendChar('\n');
                    
                }else{
                    
                    SiSendChar('\r');
                    SiSendChar(opcio);
                    
                }
                
            }
            if (TiGetTics(timerPropaganda) > 10000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 300000) timestamp=0;
                actualitzaTemps();
            }
            
            break;
        case 1:
            
            if (SiCharAvail() != 0){
                
                if ((opcio = SiGetChar()) == 27){
                    
                    Menu();
                    estatPropaganda=0;
                    identificador = 2;
                    bit_time = 2;
                    
                } else if (opcio == '\n'){
                    
                    
                    estatPropaganda=11;
                    bit_time = 2;
                    SiPutsCooperatiu(INSERT_ID);
                    
                }else{
                    
                    id[identificador] = opcio;
                    identificador--;
                    if(identificador < 0){
                        identificador = 2;
                        SiSendChar('\r');
                        SiSendChar(opcio);
                    }else{
                        SiSendChar(opcio);
                    }
                    
                }
                
            }
            if (TiGetTics(timerPropaganda) > 10000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 300000) timestamp=0;
                actualitzaTemps();
            } 
            break;

        case 2:
            
            Statistics();
            estatPropaganda=21;
            
            break;
            
        case 3:
            
            if (SiCharAvail() != 0){
                
                if (SiGetChar() == 27){
                    
                    Menu();
                    estatPropaganda=0;
                    
                }
                
            }
            
            break;
        case 4:
            
            if (SiCharAvail() != 0){
                
                if (SiGetChar() == 27){
                    
                    Menu();
                    estatPropaganda=0;
                    
                }
                
            }
            
            break;
            
        case 11:
            
            if (SiCharAvail() != 0){
                
                if ((opcio = SiGetChar()) == 27){
                    
                    Menu();
                    estatPropaganda=0;
                    identificador = 2;
                    bit_time = 2;
                    
                } else if (opcio == '\n'){
                    
                    Menu();
                    estatPropaganda=0;
                    identificador = 2;
                    bit_time = 2;
                    
                }else{
                    
                    btime[bit_time] = opcio;
                    bit_time--;
                    if(bit_time < 0)bit_time = 2;
                    
                }
                
            }
            
            break;
        case 21:
            
            if (SiCharAvail() != 0){
                
                if ((opcio = SiGetChar()) == 27){
                    
                    Menu();
                    estatPropaganda=0;
                    
                }
                
            }
            
            if (TiGetTics(timerPropaganda) > 10000){
                
                TiResetTics(timerPropaganda);
                
                if (++timestamp == 300000) timestamp=0;
                
                actualitzaTemps();
                
                SiPutsCooperatiu(TIMESTAMP);
                SiPutsCooperatiu(rtime+reset_time);
                SiPutsCooperatiu(TIMESTAMP_SECOND);
                
            }
            break;
       case 31:
            SiPutsCooperatiu("\t\tPB2: \0");
            SiSendChar(getPB2()+'0');
            estatPropaganda= 3;
            break;
        case 51:
            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            if (SiCharAvail() != 0){
                switch (SiGetChar()){
                    case 27:
                        Menu();
                        estatPropaganda=0;
                        break;
                    case '0':
                        SiPutsCooperatiu("\r000\0");
                        setBlDuty(0);
                        break;
                    case '1':
                        SiPutsCooperatiu("\r025\0");
                        setBlDuty(PERIODBL/4);
                        break;
                    case '2':
                        SiPutsCooperatiu("\r050\0");
                        setBlDuty(PERIODBL/2);
                        break;
                    case '3':
                        SiPutsCooperatiu("\r075\0");
                        setBlDuty(PERIODBL*3/4);
                        break;
                    case '4':
                        SiPutsCooperatiu("\r100\0");
                        setBlDuty(PERIODBL);
                        break;
                }
            }
            break;
        case 61:

            if (TiGetTics(timerPropaganda) > 1000){
                TiResetTics(timerPropaganda);
                if (++timestamp == 10000) timestamp=0;
            }
            if (SiCharAvail() != 0){
                switch (SiGetChar()){
                    case 27:
                        Menu();
                        estatPropaganda=0;
                        break;
                    case 'J':
                        if (changeAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
                        break;
                    case 'j':
                        if (changeAudioStatus()) SiPutsCooperatiu("\rAltaveu ences! \0");
                        else SiPutsCooperatiu("\rAltaveu apagat!\0");
                        break;
                }
            }
            break;
    }

}

#define     MAXCOLUMNES 16
#define     MAXCADENES 2
#define     MAXSIGNALS 6

static char estatLCD = 0;
const unsigned char cadena[MAXCADENES][MAXCOLUMNES] = {{"Waiting for ID  "},{"ID set          "}}; //Més val que tingui 16 caràcters...

unsigned char headerString[MAXSIGNALS][MAXCOLUMNES] = {{"TRIANGLE      Hz"},{"PULSE         Hz"},{"SAWTOOTH      Hz"},
{"SINE          Hz"},{"RANDOM        Hz"},{"CUSTOM        Hz"}};
unsigned char subheaderString[MAXCOLUMNES] = {"0 V.        0 V."};

static unsigned char timerLCD, i,j, quina, swap, header;
static unsigned char primeraLinia[MAXCOLUMNES], segonaLinia[MAXCOLUMNES];

void setCadena(unsigned char *cadenaToSet){
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
    quina = swap = i = header = 0;
    
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
            segonaLinia[swap--] = ' ';
            
            if(swap == MAXCOLUMNES)swap = 0;
            
        }
        
    }
    
}

inline void prepareHeader(){
    
    headerString[header][13] = getFrequency(1);
    headerString[header][12] = getFrequency(2);
    headerString[header][11] = getFrequency(3);
    headerString[header][10] = getFrequency(4);
    
}

inline void prepareSubHeader(){
    subheaderString[0] = getVpp();
    subheaderString[12] = getOffset();
}


void MotorLCD(void){
    switch (estatLCD){
        case 0:
                    
            if(getLength() > 0){
                
                prepareHeader();
                
            }
            
            estatLCD = 1;
            
            break;
        case 1:
                   
            if(getLength() > 0){ 
                
                LcPutChar(headerString[header][j]);
                
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
                
                if(id[0] != ' '){
                    
                    if(id[1] != ' ' && id[2] != ' '){
                        
                        quina++;
                        swap = 3;
                        
                    }
                    
                }
                
            }else{
                
                if(getLength() > 0){ 
                    
                    prepareSubHeader();
                    
                }
                
            }
            
            estatLCD = 3;
            
            break;
        case 3: //Aquí faig l'itoa, que deu trigar una bona estona el pobre...
            
            
            if(quina && swap){
                
                setCadena(primeraLinia);
                *(primeraLinia + MAXCOLUMNES - 1) = id[2];
                *(primeraLinia + MAXCOLUMNES - 2) = id[1];
                *(primeraLinia + MAXCOLUMNES - 3) = id[0];
                swap = 0;
                initCadena(segonaLinia,MAXCOLUMNES);
                
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
            
            if(getLength() > 0){ 
                    
                LcPutChar(segonaLinia[i]);
                    
            }else{
                
                LcPutChar(subheaderString[i]);
                
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
                j = 0;
                i = 0;
                
            }
            
            break;
    }
}

/*
 *
 *          Fi  PROPAGANDAAAAAAAAAA!!!!
 *
 */

