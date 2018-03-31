LIST P=18F4321, F=INHX32
#include <p18f4321.inc>

;******************
;* CONFIGURACIONS *
;******************
CONFIG OSC = HSPLL
CONFIG PBADEN = DIG
CONFIG WDT = OFF
CONFIG LVP = OFF

;*************
;* VARIABLES *
;*************
    
TEMPS_UN EQU 0x00
TEMPS_LED EQU 0x01
QUIN_LED EQU 0x02
HEART_BEAT EQU 0x03
ESTAT EQU 0x04
TEMPS_MAX EQU 0x05
 
SIGNAL_TYPE EQU 0x06
COMPTA_BYTES EQU 0x07
 
 
;*****************
;*   CONSTANTS   *
;*****************
 
TAULA7S EQU 0x0000A0
POSICIO_A_DESAR_RAM EQU 0x80
  
;************
;*   FLAGS   *
;************

 FLAG_DESAR EQU 0x80
 FLAG_DESAR_NCON EQU 0x81
 FLAG_ENVIAR EQU 0x82
 FLAG_CONTINUAR EQU 0x01
 FLAG_HEART EQU 0x83
 FLAG_HALF EQU 0x84
 FLAG_PROGRESS EQU 0x85
 FLAG_ENVIAR_ERROR EQU 0x86

;*********************************
; VECTORS DE RESET I INTERRUPCI� *
;*********************************
    
    ORG 0x000000
RESET_VECTOR
    goto MAIN

    ORG 0x000008
HI_INT_VECTOR
    goto HIGH_INT

    ORG 0x000018
LOW_INT_VECTOR
    retfie FAST

;***********************************
;* RUTINES DE SERVEI D'INTERRUPCI� *
;***********************************

HIGH_INT
    call RESET_TIMER
    incf TEMPS_UN,1,0
    incf TEMPS_LED,1,0
    retfie FAST;2

;****************************
;* MAIN I RESTA DE FUNCIONS *
;****************************
INIT_ADCON
    movlw 0x03
    movwf ADCON0,0
    
    movlw 0x0E
    movwf ADCON1,0
    
    movlw 0x84
    movwf ADCON2,0
    
    return

INIT_VARS
    
    movlw 0x5c
    movwf LATD,0  
    
    clrf LATC,0
    movlw 0x02
    movwf ESTAT,0
    
    clrf SIGNAL_TYPE,0
    clrf TEMPS_UN,0
    clrf TEMPS_LED,0
    
    clrf QUIN_LED,0
    
    clrf HEART_BEAT,0
    
    clrf TEMPS_MAX,0
    
    movlw 0x14
    movwf TEMPS_MAX,0
    
    return;2

INIT_SIO
    ;1001 0000 - 0x90
    
    movlw 0x90
    movwf RCSTA,0
    ;0010 0110 - 0x26
    
    movlw 0x26
    movwf TXSTA,0
    movlw 0x81
    movwf SPBRG,0
    clrf SPBRGH,0
    clrf BAUDCON, 0
    
    return

INIT_PORTS
    ;clrf TRISA, 0
    bcf TRISC, 0, 0	;RF Out
    bcf TRISC, 1, 0	;RF Out
    bcf TRISC, 3, 0	;LED0
    bcf TRISC, 2, 0	;LED1
    
    bsf TRISB, 0, 0	;SIO IN
    bsf TRISB, 1, 0	;Envia Info
    bsf TRISB, 2, 0	;Desa Info

    bcf  TRISD, 0, 0 ; 7SEG1
    bcf  TRISD, 1, 0 ; 7SEG2
    bcf  TRISD, 2, 0 ; 7SEG6
    bcf  TRISD, 3, 0 ; 7SEG3
    bcf  TRISD, 4, 0 ; 7SEG4
    bcf  TRISD, 5, 0 ; 7SEG5
    bcf  TRISD, 7, 0 ; 7SEG7

    bcf SSPCON1,SSPEN,0
    
    bcf INTCON2,RBPU,0
    
    return

INIT_TIMER
    ;10001000
    ;88
    movlw 0x88
    movwf T0CON,0
    bcf RCON, IPEN, 0

    ;10100000
    ;E0
    movlw 0xE0
    movwf INTCON, 0

    call RESET_TIMER
    
    return

RESET_TIMER
    movlw 0xD8
    movwf TMR0H, 0
    
    movlw 0xF0
    movwf TMR0L, 0
    bcf INTCON, TMR0IF, 0
    
    return
    
;7SEGMENTS
; 0x00000001 - PART SUPERIOR
; 0x00000010 - PART LATERAL DP
; 0x00000100 - PART LATERAL !DP
; 0x00001000 - PART INFERIOR DP
; 0x00010000 - PART INFERIOR
; 0x00100000 - PART INFERIOR !DP
; 0x10000000 - PART CENTRAL
    
;0x00001011
    
;************
;*  TAULES   *
;************
ORG TAULA7S
    ;Segments del 0, segments del 1
    DB 0x7F, 0XF5

    ;Segments del 2, segments del 3
    DB 0x4C, 0x64

    ;Segments del 4, segments del 5
    DB 0x71, 0x62

    ;Segments del 6, segments del 7
    DB 0x02, 0xF4

    ;Segments del 8, segments del 9
    DB 0x00, 0x20 

;********
;* MAIN *
;********


MAIN
    call INIT_ADCON
    call INIT_VARS
    call INIT_PORTS
    call INIT_SIO
    call INIT_TIMER

LOOP
    
    btfsc PIR1,RCIF,0
    call REBUT
    
    btfss PORTB,2,0 ;Desar Info
    call POLS_DESAR_INFO
    
    btfss PORTB,1,0 ;Enviar Info
    call POLS_ENVIAR_INFO
    
    btfsc ESTAT, 0,0
    call MIG_INTENSITAT
    
    btfss ESTAT, 0,0
    call BLINKING
    
    goto LOOP
    
REBUT
    btfss RCREG, 7,0 ;Qualsevol byte rebut que tingui aquest bit a 1 es refereix a una resposta del pc
    return ;Si rebem un byte i no esta activat no hauriem dentrar aqui
    
    
    
    movlw FLAG_DESAR ;Valor del RCREG per enviar per RF
    cpfsgt RCREG,0 ;Si es mes gran no desara el missatge
    goto DESA ;Desem els bytes
    
    movlw FLAG_DESAR_NCON ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto DESA_NCON ;Desar dades sense enviar confirmacio al PC ja que ja l'hem rebut\
    
    movlw FLAG_ENVIAR ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto ENVIAR_RF ;Enviar dades per RF
    
    movlw FLAG_ENVIAR_ERROR ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto ENVIAR_RF_ERROR ;Enviar dades per RF
    
    return


;***********************************************************
;*********************** - BLOC RF - ***********************
;***********************************************************
    
ENVIAR_RF
        
    clrf ESTAT,0
    bsf ESTAT,1,0
    return
    
ENVIAR_RF_ERROR
    
    movlw 0x7F
    movwf LATD,0  
    clrf ESTAT,0
    bsf ESTAT,0,0
    call USART_DESAT
    
    return
    
    
;***********************************************************
;********************* - BLOC USART - **********************
;***********************************************************

USART_ENVIAR
    movlw FLAG_ENVIAR
    movwf TXREG,0
    goto USART_ESPERA
    
USART_DESAR
    movlw FLAG_DESAR
    movwf TXREG,0
    goto USART_ESPERA
    
USART_DESAT
    movlw FLAG_DESAR_NCON
    movwf TXREG,0
    goto USART_ESPERA

USART_ESPERA
    btfss TXSTA,TRMT,0
    goto USART_ESPERA
    return

USART_ESPERA_INFO
   btfss PIR1,RCIF,0
    goto USART_ESPERA_INFO
   return  

;***********************************************************
;********************* - BLOC DESAR - **********************
;***********************************************************
    
REBRE_CONFIG
    btfss PIR1,RCIF,0
    goto REBRE_CONFIG
	
    movff RCREG, SIGNAL_TYPE ;Movem el caracter a la posicio de la ram corresponent
    
    call USART_DESAR
    return
    
DESA
    call USART_DESAR

DESA_NCON ;Quan ens apreten el boto no necessitem enviar al pc la confirmacio
    clrf RCREG,0
    call REBRE_CONFIG
    clrf COMPTA_BYTES,0
    clrf ESTAT,0
    clrf FSR0H,0
    
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0L, 0

DESA_BUCLE
    btfss PIR1,RCIF,0
	goto DESA_BUCLE ;Mentres no valgui 1 el bit RCIF que ens indica que hi ha un byte ens esperem
	
    movlw 0x99 ; 150
    cpfslt COMPTA_BYTES, 0 ;Si rebem el byte de final del ordinador sortim, no el desem
    goto COMPROVA_FINAL

DESA_CONTINUA_BUCLE
    movff RCREG, POSTINC0 ;Movem el caracter a la posicio de la ram corresponent
    
    incf COMPTA_BYTES,1,0 ;Incrementem en numero de bytes rebut
    
    call USART_DESAT ;Confirmarem al ordinador que ho hem desat
    
    goto DESA_BUCLE ;Esperem una nova dada
    
COMPROVA_FINAL
    btfsc SIGNAL_TYPE,4,0
    goto DESA_CPLX_FINAL
    
FINAL_DESAR   
    call CANVIA_ESTAT_UP
    bcf SIGNAL_TYPE,7,0
    return
    
DESA_CPLX_FINAL
    btfsc SIGNAL_TYPE,7,0
    goto FINAL_DESAR
    
DESA_CPLX_LAST_BYTES
    bsf SIGNAL_TYPE,7,0
    goto DESA_CONTINUA_BUCLE
    
;***********************************************************
;****************** - BLOC POLSADORS - *********************
;***********************************************************
 
    
POLS_DESAR_INFO     
    clrf TEMPS_UN,0
    call ESPERA_16MS
    call ESPERA_BAIXAR_DESAR
    
    call USART_DESAT
    
    return
    
POLS_ENVIAR_INFO
    clrf TEMPS_UN,0
    call ESPERA_16MS
    call ESPERA_BAIXAR_ENVIAR
    
    movlw 0x00
    cpfsgt COMPTA_BYTES,0
    goto ENVIAR_RF_ERROR
    
    call ENVIAR_RF
    
    return
    

ESPERA_BAIXAR_DESAR
    btfss PORTB,2,0
    goto ESPERA_BAIXAR_DESAR
    return
    
ESPERA_BAIXAR_ENVIAR
    btfss PORTB,1,0
    goto ESPERA_BAIXAR_ENVIAR
    return
    
ESPERA_16MS
    movlw 0x0F
    cpfseq TEMPS_UN,0
    goto ESPERA_16MS
    return
    
;***********************************************************
;********************* - BLOC LEDS - ***********************
;***********************************************************
    
ACTIVA_LEDS
    bsf LATC,2,0
    bsf LATC,3,0
    return
    
DESACTIVA_LEDS
    bcf LATC,2,0
    bcf LATC,3,0
    return
    
MIG_INTENSITAT
    movlw 0x0A
    cpfsgt TEMPS_LED,0    
    goto ACTIVA_LEDS
    
    call DESACTIVA_LEDS
    
    movlw 0x14
    cpfslt TEMPS_LED,0
    clrf TEMPS_LED,0
    
    return

BLINKING
    
    btfsc ESTAT, 1,0
    goto PUJADA_LED
    
    btfsc ESTAT, 2,0
    goto ESPERA_CENT
    
    btfsc ESTAT, 3,0
    goto BAIXADA_LED
    
    return    
    
    
    
CANVIA_ESTAT_DOWN
    clrf TEMPS_LED
    clrf ESTAT,0
    
    bsf ESTAT,3,0
    return
       
CANVIA_ESTAT_ESPERA
    clrf ESTAT,0
    bsf ESTAT,2,0
    incf HEART_BEAT,1,0
    
    call USART_HEART_STATUS
    
    clrf TEMPS_LED
    goto LEDS_ON
    
    return
    
CANVIA_ESTAT_UP
    clrf ESTAT,0
    clrf TEMPS_LED,0
    clrf HEART_BEAT,0
    btg QUIN_LED,0,0
    bsf ESTAT,1,0
    return
   
BAIXADA_LED
    call USART_HEART_STATUS
    
    movf HEART_BEAT, 0,0
    cpfsgt TEMPS_LED,0
    goto LEDS_ON
        
    movf TEMPS_MAX,0,0
    cpfsgt TEMPS_LED,0
    goto LEDS_OFF
    
    clrf TEMPS_LED,0
    decf HEART_BEAT,1,0
    
    btfsc STATUS,N,0
    goto CANVIA_ESTAT_UP
    
    return
    
    
ESPERA_CENT
    movlw 0x74
    cpfslt TEMPS_LED,0
    goto CANVIA_ESTAT_DOWN
    
    return
    
PUJADA_LED
    call USART_HEART_STATUS
    
    movf HEART_BEAT, 0,0
    cpfsgt TEMPS_LED,0
    goto LEDS_ON
        
    movf TEMPS_MAX,0,0
    cpfsgt TEMPS_LED,0
    goto LEDS_OFF
    
    clrf TEMPS_LED,0
    incf HEART_BEAT,1,0
    
    movf TEMPS_MAX,0,0
    cpfslt HEART_BEAT,0
    goto CANVIA_ESTAT_ESPERA
    
    return
    
LEDS_ON
    btfsc QUIN_LED,0,0
    goto ACTIVA_LED0
    
    bsf LATC,2,0
    return

ACTIVA_LED0    
    bsf LATC,3,0
    return
    
DESACTIVA_LED0
    bcf LATC,3,0
    return
    
LEDS_OFF
    btfsc QUIN_LED,0,0
    goto DESACTIVA_LED0
    
    bcf LATC,2,0
    return
    
USART_HALF_STATUS
   movlw FLAG_HALF
   movwf TXREG,0
   call USART_ESPERA
   call USART_ESPERA_INFO
   
   return

USART_HEART_STATUS
   movlw FLAG_HEART
   movwf TXREG,0
   call USART_ESPERA
   
   call USART_ESPERA_INFO
   
   movff QUIN_LED,TXREG
   call USART_ESPERA
   
   call USART_ESPERA_INFO
   
   movff HEART_BEAT,TXREG
   goto USART_ESPERA
   
    END
