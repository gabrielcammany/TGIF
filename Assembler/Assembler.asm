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
CONNECTION EQU 0x08
 
TEMPS_CONN EQU 0x09
 
RESTANT EQU 0x0A
COPS EQU 0x0B
AUXILIAR EQU 0x0C
BYTE EQU 0x0D
DIV_DEU EQU 0x0E

TEMPS_UN_RF EQU 0x0F
TEMPS_ZERO_RF EQU 0x10
 
TEMPS_UN_POLS EQU 0x11
ESTAT_P EQU 0x12
 
;*****************
;*   CONSTANTS   *
;*****************
 
TAULA7S EQU 0x0000C0
POSICIO_A_DESAR_RAM EQU 0x80
  
;************
;*   FLAGS   *
;************

FLAG_DESAR EQU 0x80
FLAG_DESAR_NCON EQU 0x81
FLAG_ENVIAR EQU 0x82
FLAG_CONTINUAR EQU 0x01
FLAG_ENVIAR_ERROR EQU 0x83
FLAG_HEART EQU 0x84
FLAG_HALF EQU 0x85
FLAG_PROGRESS EQU 0x86
FLAG_HEART_DATA EQU 0x87
FLAG_CONNECTION EQU 0x88
UNBLOCK EQU 0x89
FLAG_DELETE_INFO EQU 0x8A
FLAG_DATA EQU 0x8B

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
    incf TEMPS_CONN,1,0
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
    clrf ESTAT,0
    clrf SIGNAL_TYPE,0
    clrf TEMPS_UN,0
    clrf TEMPS_LED,0
    clrf QUIN_LED,0
    clrf HEART_BEAT,0
    clrf TEMPS_MAX,0
    clrf TEMPS_CONN,0
    clrf CONNECTION,0
    clrf COPS,0
    clrf RESTANT,0
    clrf ESTAT_P,0
    
    movlw 0x05
    movwf TEMPS_UN_RF,0
    
    movlw 0x0A
    movwf TEMPS_ZERO_RF,0
    
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
    bcf TRISB, 3, 0	;LED SIO

    bcf  TRISD, 0, 0 ; 7SEG1
    bcf  TRISD, 1, 0 ; 7SEG2
    bcf  TRISD, 2, 0 ; 7SEG6
    bcf  TRISD, 3, 0 ; 7SEG3
    bcf  TRISD, 4, 0 ; 7SEG4
    bcf  TRISD, 5, 0 ; 7SEG5
    bcf  TRISD, 7, 0 ; 7SEG7

    bcf SSPCON1,SSPEN,0
    
    bcf INTCON2,RBPU,0
    
    movlw 0x5c
    movwf LATD,0  
    
    clrf LATC,0
    
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
    
    call POLSADOR
    
    ;btfss PORTB,1,0 ;Enviar Info
    ;call POLS_ENVIAR_INFO
    
    btfsc ESTAT, 0,0
    call MIG_INTENSITAT
    
    btfss ESTAT, 0,0
    call BLINKING
    
    btfss CONNECTION,0,0
    call USART_CONNECTAT
    
    btfsc PORTB,0,0
    call USART_DESCONNECTAT
    
    goto LOOP
    
REBUT
    btfss RCREG, 7,0 ;Qualsevol byte rebut que tingui aquest bit a 1 es refereix a una resposta del pc
    return ;Si rebem un byte i no esta activat no hauriem dentrar aqui
    
    movlw FLAG_DESAR 
    cpfsgt RCREG,0 
    goto DESA 
        
    movlw FLAG_DESAR_NCON 
    cpfsgt RCREG,0
    goto DESA_NCON 
    
    movlw FLAG_ENVIAR 
    cpfsgt RCREG,0
    goto ENVIAR_RF 
    
    movlw FLAG_ENVIAR_ERROR
    cpfsgt RCREG,0
    goto ENVIAR_RF_ERROR
    
    movlw FLAG_HEART 
    cpfsgt RCREG,0
    goto ENCENDRE_HEART
    
    movlw FLAG_HALF 
    cpfsgt RCREG,0
    goto ENCENDRE_HALF 
    
    movlw FLAG_HEART_DATA 
    cpfsgt RCREG,0
    goto USART_HEART_STATUS 
    
    movlw UNBLOCK 
    cpfsgt RCREG,0
    goto UNBLOCK_THREAD 
    
    movlw FLAG_DATA 
    cpfsgt RCREG,0
    goto USART_DATA 
    
    return


UNBLOCK_THREAD
    call USART_DESAT
    bsf LATC,2,0
    return
    
    
;***********************************************************
;*********************** - BLOC RF - ***********************
;***********************************************************
    
ENVIAR_RF
        
    movlw 0x00
    cpfsgt COMPTA_BYTES,0
    goto ENVIAR_RF_ERROR
    
    clrf FSR0H, 0
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0L, 0
    clrf COPS,0
    clrf BYTE,0
    clrf RESTANT,0
    
    movff POSTINC0, AUXILIAR
    
    call COMPUTE_DIV_10
    
ENVIAR_FOR    
    movf TEMPS_UN_RF,0
    cpfsgt TEMPS_UN,0
    call ENVIAR_BIT_PRIMERA_MEITAT
    
    movf TEMPS_ZERO_RF
    cpfsgt TEMPS_UN,0
    call ENVIAR_BIT_SEGONA_MEITAT
    
    call USART_BIT_ENVIAT
    incf RESTANT,1,0
    incf BYTE,1,0
    
    movlw 0x08
    cpfsgt BYTE,0
    goto CONTINUA_FOR
    
    clrf BYTE,0
    movff POSTINC0, AUXILIAR
    
    movf DIV_DEU,0,0
    cpfslt RESTANT,0
    goto INCREMENTA_FOR
    
    goto ENVIAR_FOR

ENVIA_PARAMETRES
    movff DIV_DEU,TXREG
    call USART_ESPERA
    
    movff RESTANT,TXREG
    call USART_ESPERA
    
    movff COPS,TXREG
    call USART_ESPERA
    return
        
CONTINUA_FOR
    clrf TEMPS_UN,0
    rrncf AUXILIAR,1,0
    goto ENVIAR_FOR 
    
INCREMENTA_FOR
    incf COPS,1,0
    clrf RESTANT,0
    
    movlw 0x0A
    cpfslt COPS,0
    goto FINAL_ENVIAR_RF
    
    ;call INCREMENTA_7_SEG
    
    goto ENVIAR_FOR 
    
FINAL_ENVIAR_RF
    clrf ESTAT,0
    bsf ESTAT,1,0
    return
    
ENVIAR_BIT_PRIMERA_MEITAT
    btfsc AUXILIAR,0,0 ;Mirem si el primer bit es 0, si ho es fiquem la primera part a 0
    bcf LATC, 5, 0
    btfss AUXILIAR,0,0 ;Si el primer es 0 fiquem la primera part a 1
    bsf LATC, 5, 0
    return
    
ENVIAR_BIT_SEGONA_MEITAT
    btfsc AUXILIAR,0,0 ;Mirem si el primer bit es 0, si ho es fiquem la segona part a 1
    bsf LATC, 5, 0
    btfss AUXILIAR,0,0 ;Si el primer es 1 fiquem la segona part a 0
    bcf LATC, 5, 0
    return
    
ENVIAR_RF_ERROR
    
    movlw 0x7F
    movwf LATD,0  
    clrf ESTAT,0
    bsf ESTAT,0,0
    call USART_DESAT
    
    return
    
COMPUTE_DIV_10
    btfsc SIGNAL_TYPE,4,0
    movlw 0xF0 ;240 -> (300*8/10)
    
    btfss SIGNAL_TYPE,4,0
    movlw 0x78 ;120 -> (150*8/10)
    
    movwf DIV_DEU,0
    return
    
;***********************************************************
;********************* - BLOC USART - **********************
;***********************************************************

USART_DATA
    movff SIGNAL_TYPE,TXREG
    goto USART_ESPERA
    
USART_ENVIAR
    movlw FLAG_ENVIAR
    movwf TXREG,0
    goto USART_ESPERA
    
USART_DESAR_NCON
    movlw FLAG_DESAR_NCON
    movwf TXREG,0
    goto USART_ESPERA
    
USART_DESAR
    movlw FLAG_DESAR
    movwf TXREG,0
    goto USART_ESPERA
    
USART_DESAT
    movlw FLAG_CONTINUAR
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
   
USART_CONNECTAT
   btfsc PORTB,0,0
   return
   
   movlw 0x34
   cpfsgt TEMPS_CONN,0
   return
   
   clrf TEMPS_CONN,0
   movlw FLAG_CONNECTION
   movwf TXREG,0
   goto USART_ESPERA
   
   return
   
USART_DESCONNECTAT
   btfsc CONNECTION,1,0
   return
   
   clrf TEMPS_CONN,0
   clrf CONNECTION,0
   bsf CONNECTION,1,0
   
   bcf LATB,3,0
   ;bsf LATC,3,0
   
   return
   
USART_BIT_ENVIAT
   movlw FLAG_PROGRESS
   movwf TXREG,0
   goto USART_ESPERA
   
USART_BORRAR_INFO
   movlw FLAG_DELETE_INFO
   movwf TXREG,0
   goto USART_ESPERA
   

;***********************************************************
;********************* - BLOC DESAR - **********************
;***********************************************************
    
REBRE_CONFIG   
    btfss PIR1,RCIF,0
    goto REBRE_CONFIG
    
    movff RCREG, SIGNAL_TYPE ;Movem el caracter a la posicio de la ram corresponent
    
    movff SIGNAL_TYPE,TXREG
    
    call USART_ESPERA
    
    return
    
DESA
    clrf TEMPS_UN,0
    call USART_DESAR

DESA_NCON ;Quan ens apreten el boto no necessitem enviar al pc la confirmacio
    clrf RCREG,0
    clrf SIGNAL_TYPE,0
    
    call REBRE_CONFIG
    
    clrf COMPTA_BYTES,0
    clrf ESTAT,0
    clrf FSR0H,0
    
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0L, 0

DESA_BUCLE
    btfss PIR1,RCIF,0
	goto DESA_BUCLE ;Mentres no valgui 1 el bit RCIF que ens indica que hi ha un byte ens esperem
	
    movlw 0x97 ; 150
    cpfslt COMPTA_BYTES, 0 ;Si rebem el byte de final del ordinador sortim, no el desem
	goto COMPROVA_FINAL

DESA_CONTINUA_BUCLE
    movff RCREG, POSTINC0 ;Movem el caracter a la posicio de la ram corresponent
    
    incf COMPTA_BYTES,1,0 ;Incrementem en numero de bytes rebut
    
    movff COMPTA_BYTES, TXREG ;Movem el caracter a la posicio de la ram corresponent
	call USART_ESPERA
    
    goto DESA_BUCLE ;Esperem una nova dada
    
COMPROVA_FINAL
    btfsc SIGNAL_TYPE,4,0
	goto DESA_CPLX_FINAL
    
FINAL_DESAR   
    bcf SIGNAL_TYPE,7,0
    goto CANVIA_ESTAT_UP
    
DESA_CPLX_FINAL
    btfsc SIGNAL_TYPE,7,0
    goto FINAL_DESAR
    
DESA_CPLX_LAST_BYTES
    bsf SIGNAL_TYPE,7,0
    clrf COMPTA_BYTES,0
    incf COMPTA_BYTES,1,0
    
    goto DESA_CONTINUA_BUCLE
    
;***********************************************************
;****************** - BLOC POLSADORS - *********************
;***********************************************************
 
POLSADOR
    
    movlw 0x00
    cpfsgt ESTAT_P,0
    goto ESPERA_POLSADOR
    
    btfsc ESTAT_P,0,0
    goto ESPERA_POLSADOR_DOWN	
    
    btfsc ESTAT_P,1,0
    goto ESPERA_POLSADOR_DESAR
    
    btfsc ESTAT_P,2,0
    goto ESPERA_POLSADOR_DEL
    
    return
    
ESPERA_POLSADOR
    btfss PORTB,2,0 
    goto POLS_DESAR_INFO
    return
    
ESPERA_POLSADOR_DOWN   
    movlw 0x64
    cpfslt TEMPS_UN,0
    call INCREMENTA_ESPERA
    
    btfss PORTB,2,0 
    return
    
    movlw 0x14
    cpfslt TEMPS_UN_POLS,0
    goto RESET_POLSADOR
    
    clrf ESTAT_P,0
    bsf ESTAT_P,1,0 
        
    return

ESPERA_POLSADOR_DESAR
    movlw 0x64
    cpfslt TEMPS_UN,0
    call INCREMENTA_ESPERA
    
    movlw 0x14
    cpfslt TEMPS_UN_POLS,0
    goto PETICIO_DESAR_INFO
        
    btfsc PORTB,2,0 
    return
    
    clrf TEMPS_UN,0
    call ESPERA_16MS
    
    clrf ESTAT_P,0
    bsf ESTAT_P,2,0 
    return
    
ESPERA_POLSADOR_DEL
    movlw 0x64
    cpfslt TEMPS_UN,0
    call INCREMENTA_ESPERA
    
    btfss PORTB,2,0 
    return
    
    movlw 0x14
    cpfsgt TEMPS_UN_POLS,0
    goto PETICIO_DEL_INFO
        
    clrf ESTAT_P,0
    return
    
    
PETICIO_DEL_INFO
    clrf ESTAT_P,0
    bsf LATC,3,0
    bsf LATC,2,0
    
    clrf SIGNAL_TYPE,0
    clrf COMPTA_BYTES,0
    call USART_BORRAR_INFO
    
    return
    
PETICIO_DESAR_INFO
    clrf ESTAT_P,0
    bsf LATC,3,0
    bcf LATC,2,0
    
    btfsc PORTB,0,0
    return
    
    call USART_DESAR_NCON
    return
    
    
INCREMENTA_ESPERA
    incf TEMPS_UN_POLS,1,0
    clrf TEMPS_UN,0
    return

RESET_POLSADOR
    clrf ESTAT_P,0
    return
    
    
POLS_DESAR_INFO     
    clrf TEMPS_UN,0
    call ESPERA_16MS
   
    bsf ESTAT_P,0,0 
    clrf TEMPS_UN_POLS,0
    
    
    
    return
    
POLS_ENVIAR_INFO
    clrf TEMPS_UN,0
    call ESPERA_16MS
    call ESPERA_BAIXAR_ENVIAR
    
    movlw 0x00
    cpfsgt COMPTA_BYTES,0
    goto ENVIAR_RF_ERROR
    
    ;call ENVIAR_RF
    
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
    movlw 0x20
    cpfseq TEMPS_UN,0
    goto ESPERA_16MS
    return
    
;***********************************************************
;********************* - BLOC LEDS - ***********************
;***********************************************************
    
ENCENDRE_HEART
    ;call USART_DESAT
    call CANVIA_ESTAT_UP
    ;bsf LATC,3,0
    return
    
ENCENDRE_HALF
    clrf RCREG,0
    call USART_DESAT
    clrf ESTAT,0
    bsf ESTAT, 0,0
    clrf TEMPS_LED,0
    return
    
    
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
    
    ;call USART_HEART_STATUS
    
    clrf ESTAT,0
    bsf ESTAT,2,0
    incf HEART_BEAT,1,0
    
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
    ;call USART_HEART_STATUS
    
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
    ;call USART_HEART_STATUS
    
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
   movlw FLAG_HEART_DATA
   movwf TXREG,0
   call USART_ESPERA
      
   movf HEART_BEAT,0,0
   movwf TXREG,0
   call USART_ESPERA
   
   return
   
    END
