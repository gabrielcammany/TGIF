LIST P=18F4321, F=INHX32
#include <p18f4321.inc>

;******************
;* CONFIGURACIONS *
;******************
CONFIG OSC = HSPLL
CONFIG PBADEN = DIG
CONFIG WDT = OFF
CONFIG LVP = OFF

;****************
;* CONSTANTS_OLD *
;****************
BIT_RESPOSTA_PC EQU 0x08

FLAG_BYTE_ENVIAT_MSG EQU 0x86
FLAG_DESAR_SENSE_CONFIRMACIO_MSG EQU 0x84
FLAG_ID_PLACA EQU 0xE8

ESTAT_LEDS_CIRCULAR EQU 0x02
ESTAT_POLSADOR EQU 0x01
ESTAT_BLINKING_10HZ EQU 0x04
ESTAT_BLINKING_5HZ EQU 0x08

POSICIO_A_DESAR_RAM EQU 0x80
END_BYTE EQU 0x88
MAX_LEDS EQU 0x0B

VALOR_A_MULTIPLICAR EQU 0xCD ;205

TEMPS_100_MSEG EQU 0x14 ;20
TEMPS_200_MSEG EQU 0x28 ;40
TEMPS_400_MSEG EQU 0x50 ;80
TEMPS_500_MSEG EQU 0x34 ;100 - 0x64
TEMPS_1000_MSEG EQU 0xC8 ;200

;*************
;* VARIABLES *
;*************

ESTAT        EQU 0x02
ESTATP       EQU 0x03
TEMPS_UN	 EQU 0x04
TEMPS_LED	 EQU 0X05
TEMPS_MAX	 EQU 0x06
QUIN_LED	 EQU 0x07
SIGNAL       EQU 0X08
AUX          EQU 0X09
OFFSET       EQU 0X0A
VPP_REF	     EQU 0X0B
TYPES	     EQU 0X0C
COMPTABYTES  EQU 0X0D
ENVIAT	     EQU 0X0E
PERIODE	    EQU 0X0F
	    
COMPTA_BYTES_L EQU 0x11
COMPTA_BYTES_H EQU 0x12
LEDS EQU 0x05
RESULTAT_DIVISIO EQU 0x06
INDEX EQU 0x07
AUXILIAR EQU 0x10
RESTANT EQU 0x13

;************
;* CONSTANTS *
;************
POLS_CARREGA                EQU 0x01
POLS_ENVIA_RF               EQU 0x02

LED0                        EQU 0X02
LED1                        EQU 0X03

PART_INFERIOR               EQU 0X05
PART_SUPERIOR               EQU 0X06
PART_CENTRAL                EQU 0X07

BIT5                        EQU 0X08

ESTAT_ESPERA_POLSADOR_DOWN  EQU 0x22
ESTAT_ESPERA_POLSADOR_DEL   EQU 0x24
ESTAT_BLINKING              EQU 0x25
ESTAT_BLINKING_ESPERA       EQU 0x26
ESTAT_BLINKING_DOWN         EQU 0x27
ESTAT_MIG_ESTAT             EQU 0X28

;************
;*   FLAGS   *
;************

FLAG_ENVIAR_LED_ON          EQU 0X40
FLAG_ENVIAR_LED_OFF         EQU 0X41
FLAG_DESAT_MSG		    EQU 0x42
FLAG_DESAR_MSG              EQU 0X43
FLAG_ENVIAR_RF_MSG          EQU 0X44
FLAG_ENVIAR_SEG_BATCH       EQU 0X45


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


;************
;*  TAULES   *
;************
;    TAULA7S EQU 0x03A
;    ORG TAULA7S
    ;Segments del 0, segments del 1
;    DB 0xfE, 0X28

    ;Segments del 2, segments del 3
;    DB 0xCD, 0x6D

    ;Segments del 4, segments del 5
;    DB 0x2B, 0x67

    ;Segments del 6, segments del 7
;    DB 0xE7, 0x2C

    ;Segments del 8, segments del 9
;    DB 0xEF, 0x2F

;***********************************
;* RUTINES DE SERVEI D'INTERRUPCI� *
;***********************************

HIGH_INT
    call RESET_TIMER
    incf TEMPS_UN,1,0
    retfie FAST;2

;****************************
;* MAIN I RESTA DE FUNCIONS *
;****************************


INIT_VARS
    
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
    bsf TRISC, 3, 0	;Polsador Carrega
    bsf TRISC, 4, 0	;Polsador RF
    bcf TRISC, 5, 0	;Bit RF
    bsf TRISC, 7, 0	;RX Eusart
    bsf TRISC, 6, 0	;TX Eusart

    setf TRISB, 0    ; 7Segments

    bsf  TRISD, 2, 0 ; LED0
    bsf  TRISD, 3, 0 ; LED1

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
    ;CF
    ;3A
    movlw 0x3C
    movwf TMR0H, 0
    ;30
    ;98
    movlw 0xA6
    movwf TMR0L, 0
    bcf INTCON, TMR0IF, 0
    return

;********
;* MAIN *
;********


MAIN
    call INIT_VARS
    call INIT_PORTS
    call INIT_SIO
    call INIT_TIMER

LOOP
    btfsc PIR1,RCIF,0
    call REBUT
    
    btfsc ESTATP,0,0
    ;call ESTAT_ESPERA_POLSADOR
    
    btfsc ESTATP,1,0
    call ESTAT_ESPERA_POLSADOR_DOWN
    
    btfsc ESTATP,2,0
    call ESTAT_ESPERA_POLSADOR_DESAR
    
    btfsc ESTATP,3,0
    call ESTAT_ESPERA_POLSADOR_DEL

    btfsc PORTC,4,0
    call POLS_ENVIAR_RF

    btfsc PORTC,5,0
    call CONNECTAT_DB9
    
    btfsc ESTAT, 0,0
    goto MIG_INTENSITAT
    
    call BLINKING

    goto LOOP


;***********************************************************
;************* - BLOC COMUNCACIO PC - **********************
;***********************************************************

CONNECTAT_DB9
    return

ENVIAR_CONFIRMACIO_RF
    movlw FLAG_ENVIAR_RF_MSG
    movwf TXREG,0
    goto ESPERA

ENVIAR_CONFIRMACIO_DESAT
    movlw FLAG_DESAT_MSG
    movwf TXREG,0
    goto ESPERA

ENVIAR_CONFIRMACIO_DESAR
    movlw FLAG_DESAR_MSG
    movwf TXREG,0
    goto ESPERA

ENVIAR_END_BYTE
    movlw END_BYTE
    movwf TXREG,0
    goto ESPERA

ESPERA
    btfss TXSTA,TRMT,0
    goto ESPERA
    return

ENVIAR_PETICIO_DESAR
    movlw FLAG_DESAR_SENSE_CONFIRMACIO_MSG
    movwf TXREG,0
    call ESPERA
    clrf TEMPS_UN,0
    movlw ESTAT_POLSADOR
    movwf ESTAT,0
    return

REBUT
    btfss RCREG, 7,0 ;Qualsevol byte rebut que tingui aquest bit a 1 es refereix a una resposta del pc
    return ;Si rebem un byte i no esta activat no hauriem dentrar aqui
    
    movlw FLAG_DESAR_MSG ;Valor del RCREG per enviar per RF
    cpfsgt RCREG,0 ;Si es mes gran no desara el missatge
    goto DESA ;Desem els bytes
    movlw FLAG_ENVIAR_RF_MSG ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto ENVIAR_RF ;Enviar dades per RF
    movlw FLAG_DESAR_SENSE_CONFIRMACIO_MSG ;Valor del RCREG com a confirmacio del PC per enviar dades
    cpfsgt RCREG,0
    goto DESA_SENSE_CONFIRMACIO ;Desar dades sense enviar confirmacio al PC ja que ja l'hem rebut
    return


;***********************************************************
;******************** - BLOC LEDS - ************************
;***********************************************************

BLINKING
    btfsc ESTAT, 1,0
    goto PUJADA_LED
    
    btfsc ESTAT, 2,0
    goto ESPERA_CENT
    
    btfsc ESTAT, 3,0
    goto BAIXADA_LED
    
    return

ESPERA_CENT
    return
    
BAIXADA_LED
    return
    
CANVIA_ESTAT_ESPERA
    bcf ESTAT,1,0
    bsf ESTAT,2,0
    return
    
PUJADA_LED
    movf AUX, 0,0
    cpfsgt TEMPS_LED,0
    goto LEDS_ON
    
    movf TEMPS_MAX
    cpfsgt TEMPS_LED,0
    goto LEDS_OFF
    
    clrf TEMPS_LED,0
    incf AUX,1,0
    
    movlw TEMPS_MAX
    cpfslt AUX,0
    goto CANVIA_ESTAT_ESPERA
    
    return
        
LEDS_ON
    btfsc QUIN_LED,0
    goto ACTIVA_LED0
    
    bsf LATC,1,0
    return
    
ACTIVA_LED0  
    bsf LATC,0,0
    return
    
    
LEDS_OFF
    btfsc QUIN_LED,0
    goto ACTIVA_LED0
    
    bcf LATC,1,0
    return
    
DESACTIVA_LED0  
    bcf LATC,0,0
    return
    
ACTIVA_LEDS
    bsf LATC,0,0
    bsf LATC,1,0
    return
    
DESACTIVA_LEDS
    bcf LATC,0,0
    bcf LATC,1,0
    return
    
MIG_INTENSITAT
    movlw 0x0A
    cpfslt TEMPS_LED,0
    goto ACTIVA_LEDS
    
    bcf LATC,0,0
    bcf LATC,1,0
    
    movlw 0x13
    cpfslt TEMPS_LED,0
    clrf TEMPS_LED,0
    return
     

;***********************************************************
;****************** - BLOC POLSADORS - *********************
;***********************************************************

ESTAT_ESPERA_POLSADOR_DESAR
    return
    
POLS_CARREGA_MISSATGE
    clrf LATB,0
    clrf LATD,0
    clrf TEMPS_UN,0
    call ESPERA_16MS
    call ESPERA_BAIXAR_3
    goto ENVIAR_PETICIO_DESAR
    return

ESPERA_BAIXAR_3
    btfsc PORTC,3,0
    goto $-2
    return

ESPERA_BAIXAR_4
    btfsc PORTC,4,0
    goto $-2
    return

POLS_ENVIAR_RF
    clrf LATB,0
    clrf LATD,0
    call ESPERA_16MS
    call ESPERA_BAIXAR_4
    clrf TEMPS_UN,0
    goto ENVIAR_RF_SENSE_CONFIRMACIO;
    return

ESPERA_16MS
    movlw 0x08
    cpfseq TEMPS_UN,0
    goto $-2
    return

ESPERA_POLSADOR
    clrf LATB,0
    clrf LATD,0
    movlw TEMPS_1000_MSEG
    cpfsgt TEMPS_UN,0
    return
    goto COMPROVACIO_10_SEG
    ;return

COMPROVACIO_10_SEG
    incf AUXILIAR,1,0
    movlw 0x09 ;50 ja que 50*200 son 10k
    cpfsgt AUXILIAR,0 ;Si encara no hem arribat a 50 encara ens hem desperar
    goto RETORNA_POLSADOR
    ;goto ACTIVACIO_LEDS_CIRCULAR
    ;return

RETORNA_POLSADOR
    clrf TEMPS_UN,0
    return

;***********************************************************
;**************** - BLOC RF - ******************************
;***********************************************************

ENVIAR_RF
    movlw 0x00
    cpfsgt COMPTA_BYTES_L, 0
    ;goto ACTIVACIO_LEDS_CIRCULAR ;Activar leds circular
    call ENVIAR_CONFIRMACIO_RF
    clrf ENVIAT,0
    goto ESPERA_CANAL_ZERO
    clrf TEMPS_UN,0

ENVIAR_RF_SENSE_CONFIRMACIO
    movlw 0x00
    cpfsgt COMPTA_BYTES_L, 0
    ;goto ACTIVACIO_LEDS_CIRCULAR ;Activar leds circular
    clrf TEMPS_UN,0
    clrf ENVIAT,0
    goto ESPERA_CANAL_ZERO

NETEJA_RF
    btfsc TEMPS_UN, 0,0
    goto SET_PRIMERA_PART ;Primers 5 mseg
    btfsc TEMPS_UN, 1,0
    goto SET_SEGONA_PART ;Segon 5 mseg
    goto NETEJA_RF

SET_PRIMERA_PART
    bsf LATC, 5, 0
    goto NETEJA_RF

SET_SEGONA_PART
    bcf LATC, 5, 0
    incf ENVIAT,1,0
    clrf TEMPS_UN,0
    goto ESPERA_CANAL_ZERO

ESPERA_CANAL_ZERO
    movlw 0x0A
    cpfsgt ENVIAT,0
    goto NETEJA_RF
    clrf TEMPS_UN,0
    goto TEMPS_10

TEMPS_10
    movlw 0x02
    cpfslt TEMPS_UN,0
    goto ENVIAR
    bcf LATC, 5, 0
    goto TEMPS_10


ENVIAR
    movlw 0x03
    SUBWF COMPTA_BYTES_L,1,0
    ;call DIVIDIR_10
    movlw 0x03
    addwf COMPTA_BYTES_L,1,0
    clrf LATB, 0
    clrf LATD, 0
    clrf LEDS, 0
    clrf ESTAT, 0
    clrf ENVIAT,0
    clrf AUXILIAR,0
    clrf INDEX,0
    clrf FSR0H,0
    clrf RESTANT,0
    incf LEDS, 1, 0
    btfsc COMPTA_BYTES_H,0,0
    bsf ESTAT,0,0
    movff COMPTA_BYTES_L,RESTANT
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0L, 0

BUCLE_RESTANT_ENVIAR_RF
    movlw 0x00
    cpfsgt RESTANT,0
    goto PRE_FINAL_RF
    movff POSTINC0, AUXILIAR
    clrf TEMPS_UN,0

BUCLE_ENVIAR_8_BITS ;Ens quedarem aqui fins que no hem enviat 8 bits
    movlw 0x08
    cpfslt ENVIAT,0
    goto FINAL_BUCLE_ENVIAR ;Incrementarem index
    btfsc TEMPS_UN, 0,0
    goto ENVIAR_BIT_PRIMERA_MEITAT ;Primers 5 mseg
    btfsc TEMPS_UN, 1,0
    goto ENVIAR_BIT_SEGONA_MEITAT ;Segon 5 mseg
    goto BUCLE_ENVIAR_8_BITS ;Tornem al inici del bucle

FINAL_BUCLE_ENVIAR
    incf INDEX, 1,0
    clrf ENVIAT,0
    decf RESTANT,1,0
    movf RESULTAT_DIVISIO,0,0
    cpfslt INDEX,0
    call ACTIVA_LEDS_RF
    goto BUCLE_RESTANT_ENVIAR_RF ;Tornem al seguent 10 percent

ENVIAR_BIT_PRIMERA_MEITAT
    btfsc AUXILIAR,0,0 ;Mirem si el primer bit es 0, si ho es fiquem la primera part a 0
    bcf LATC, 5, 0
    btfss AUXILIAR,0,0 ;Si el primer es 0 fiquem la primera part a 1
    bsf LATC, 5, 0
    goto BUCLE_ENVIAR_8_BITS

ENVIAR_BIT_SEGONA_MEITAT
    btfsc AUXILIAR,0,0 ;Mirem si el primer bit es 0, si ho es fiquem la segona part a 1
    bsf LATC, 5, 0
    btfss AUXILIAR,0,0 ;Si el primer es 1 fiquem la segona part a 0
    bcf LATC, 5, 0
    incf ENVIAT,1,0
    rrncf AUXILIAR,1,0
    clrf TEMPS_UN,0
    goto BUCLE_ENVIAR_8_BITS

ACTIVA_LEDS_RF
    incf LEDS,1,0
    ;call ACTIVAR_LEDS_PROCES
    clrf INDEX, 0
    return

PRE_FINAL_RF
    btfss ESTAT,0,0
    goto FINAL_RF
    setf RESTANT,0
    clrf ESTAT,0
    goto BUCLE_RESTANT_ENVIAR_RF

FINAL_RF
    movlw MAX_LEDS
    cpfseq LEDS,0
    call ACTIVA_LEDS_RF
    movlw ESTAT_BLINKING_10HZ
    movwf ESTAT,0
    btfss TEMPS_UN,0,0
    goto $-2
    bcf LATC,5,0
    clrf TEMPS_UN,0
    return


;***********************************************************
;**************** - BLOC DESAR - ****************************
;***********************************************************

DESA
    call ENVIAR_CONFIRMACIO_DESAR

DESA_SENSE_CONFIRMACIO ;Quan ens apreten el boto no necessitem enviar al pc la confirmacio
    clrf COMPTA_BYTES_L,0
    clrf COMPTA_BYTES_H,0
    clrf ESTAT,0
    clrf FSR0H,0
    movlw POSICIO_A_DESAR_RAM
    movwf FSR0L, 0

BUCLE_DESAR
    btfss PIR1,RCIF,0
	goto BUCLE_DESAR ;Mentres no valgui 1 el bit RCIF que ens indica que hi ha un byte ens esperem
    movlw END_BYTE
    cpfseq RCREG, 0 ;Si rebem el byte de final del ordinador sortim, no el desem
	goto FINAL_BUCLE
    movlw ESTAT_BLINKING_5HZ
    movwf ESTAT,0
    clrf TEMPS_UN,0
    return

FINAL_BUCLE
    movff RCREG, POSTINC0 ;Movem el caracter a la posicio de la ram corresponent
    incf COMPTA_BYTES_L,1,0 ;Incrementem en numero de bytes rebut
    btfsc STATUS,C,0
    call RESTART_COMPTA
    call ENVIAR_CONFIRMACIO_DESAT ;Confirmarem al ordinador que ho hem desat
    goto BUCLE_DESAR ;Esperem una nova dada

RESTART_COMPTA
    incf COMPTA_BYTES_H,1,0
    clrf COMPTA_BYTES_L,0
    return

    
    
;***********************************************************
;*******
;* END *
;*******
    END
