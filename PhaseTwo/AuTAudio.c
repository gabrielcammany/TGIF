#include "AuTAudio.h"
#include <xc.h>

static unsigned char estat;
static unsigned int periode, timerAudio;

void AuInit(){
    SET_AUDIO_DIR();
    AUDIO_OFF();
    timerAudio = TiGetTimer();
    estat = 0;
    periode = 1;
}

char getAudioPeriode(void){
    return periode;
}

void setAudioPeriode(char nouPeriode){
    periode= nouPeriode;

}

char getAudioStatus(void){
    return estat != 2;
}


char calcula_periode(char digit){
    
    return ( digit + 1 ) * 5; 
    
}

void MotorAudio(){
    switch(estat){
        case 0:
            if (TiGetTics(timerAudio)>=periode){
                TiResetTics(timerAudio);
                AUDIO_ON();
                estat = 1;
            }
            break;
        case 1:
            if (TiGetTics(timerAudio)>=periode){
                TiResetTics(timerAudio);
                AUDIO_OFF();
                estat = 0;
            }
            break;
        case 2:
            //Callo
            break;
    }
}

void muteAudio(){
    estat = 2;
    AUDIO_OFF();
}

void unMuteAudio(){
    estat = 0;
}

char changeAudioStatus(){
    //Post Canvia l'estat d'audio
    if (estat == 2){
        //estic callat, toca parlar!
        estat = 0;
    }else{
        //Estic parlant, callo!
        estat = 2;
        AUDIO_OFF();
    }
    return estat == 0;
}