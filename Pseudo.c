#define POLS_CARREGA 0x02
#define POLS_ENVIA_RF 0x03

//Constants
#define ESTAT_ESPERA_POLSADOR EQU 0x00
#define ESTAT_ESPERA_POLSADOR_DOWN EQU 0x01
#define ESTAT_ESPERA_POLSADOR_DESAR EQU 0x02
#define ESTAT_ESPERA_POLSADOR_DEL EQU 0x02
#define ESTAT_BLINKING 0x08
#define ESTAT_BLINKING_DOWN 0x07
#define ESTAT_MIG_INTENSITAT 0x09

#define FLAG_ENVIAR_SEG_BATCH 0x05
#define FLAG_ENVIAR_RF_MSG 0x04
#define FLAG_DESAR_MSG 0x03

//Variables
char ESTAT = 0, ESTATP = 0x00, tempsUn = 0x00, tempsLED = 0x00;
char tempsMAX = 0x00, quinLED = 0, signal = 0x00;


void main(){

	while(1){

		if(PIR1.RCIF == 1){
			rebut();
		}

		if(ESTATP == ESTAT_ESPERA_POLSADOR){

			if(LATC.POLS_CARREGA){ //Get Info
				espera_pulsador();
				tempsUn = 0;
				ESTATP++;
				FLAG_ENVIAR_LED_ON -> TXREG;
			}

		}else if(ESTATP == ESTAT_ESPERA_POLSADOR_DOWN){

			if(!LATC.POLS_CARREGA){

				FLAG_ENVIAR_LED_OFF -> TXREG;

				if(tempsUn <= 2000){
					ESTATP++;
				}else{
					ESTATP = ESTAT_ESPERA_POLSADOR;
				}
			}

		}else if(ESTATP == ESTAT_ESPERA_POLSADOR_DESAR){

			if(tempsUn <= 2000){
				if(LATC.POLS_CARREGA){ //Get Info

					FLAG_ENVIAR_LED_ON -> TXREG;

					espera_pulsador();
					ESTATP++;
				}
			}else{
				peticio_desar_info();
			}

		}else if(ESTATP == ESTAT_ESPERA_POLSADOR_DEL){

			if(!LATC.POLS_CARREGA){

				FLAG_ENVIAR_LED_OFF -> TXREG;

				if(tempsUn <= 2000){
					peticio_del_info();
				}else{
					ESTATP = ESTAT_ESPERA_POLSADOR;
				}
			}

		}

		if(LATC.POLS_ENVIA_RF == 1){ //Send Info
			pols_enviar_rf();
		}

		if(ESTAT == ESTAT_MIG_INTENSITAT){
			mig_intensitat();
		}else{
			blinking();
		}

		if(LATC.BIT2 == 1){

			if(signal == 0){

				FLAG_ENVIAR_LED_CON -> TXREG;
				signal = 1;

			}

		}else{

			signal = 0;

		}

	}

}

void blinking(){

	if(ESTAT == ESTAT_BLINKING){
		pujada_LED();
	}else if(ESTAT = ESTAT_BLINKING_ESPERA){
		espera_cent();
	}else if(ESTAT = ESTAT_BLINKING_DOWN){
		baixada_LED();
	}

}

void espera_cent(){

	if(tempsLED == 100){

		ESTAT = ESTAT_BLINKING_DOWN;
		tempsLED = 0;

	}

}

void pujada_LED(){
	if(tempsLED < aux){
		if(!quinLED){
			LATC.BIT0 = 1;
		}else{
			LATC.BIT1 = 1;
		}
	}else{

		if(tempsLED < tempsMAX){

			if(!quinLED){
				LATC.BIT0 = 0;
			}else{
				LATC.BIT1 = 0;
			}

		}else{

				tempsLED = 0;
				aux++;

				if(aux == tempsMAX){

					ESTAT = ESTAT_BLINKING_ESPERA;

				}

		}
	}
}

void baixada_LED(){
	
	if(tempsLED < aux){
		if(!quinLED){
			LATC.BIT0 = 1;
		}else{
			LATC.BIT1 = 1;
		}
	}else{
		if(tempsLED < tempsMAX){
			if(!quinLED){
				LATC.BIT0 = 0;
			}else{
				LATC.BIT1 = 0;
			}
		}else{

			tempsLED = 0;
			aux--;

			if(!aux){

				ESTAT = ESTAT_BLINKING;
				toggle(quinLED.BIT0);

			}
		}
	}
}

void mig_intensitat(){

	if(tempsLED < 11){
		LATC.BIT0 = 1;
	}else if(tempsLED < 20){
		LATC.BIT0 = 0;
	}else{
		tempsLED = 0;
	}

}

void enviarConfirmacioDesar(){
	FLAG_DESAR_MSG -> TXREG
}

void enviarConfirmacioRf(){
	FLAG_ENVIAR_RF_MSG -> TXREG
}

void enviarSegBatch(){
	FLAG_ENVIAR_SEG_BATCH -> TXREG
}

void peticio_desar_info() {
	FLAG_DESAR_SENSE_CONFIRMACIO_MSG -> TXREG;
}

void peticio_del_info() {
	Offset = 0x00;
	Vpp = 0x00;
	TypeS = 0x00;
	//Esborra tot el bank0 desde la posicio D2 - FF BANK1
	//Tot i que en el fons, eliminant el offset, vpp i typeS no faria falta fer tot el bucle, ja que tractariem com si no s'hagues desat res
	//tot i tenir les dades desades, les sobreescriuriem i ale
}

void rebut(){
	if(RCREG == FLAG_DESAR_MSG){
		desar();
	}
	if(RCREG == FLAG_ENVIAR_RF_MSG){
		enviaRF();
	}
}

void desar(){
	enviarConfirmacioDesar();
	comptaBytes = 0;
	FSR0 = POSICIO_A_DESAR_RAM;
	goto bucle_desar();
}

void bucle_desar(){
	if(PIR1.RCIF == 0){goto bucle_desar();} //Mentres no hem rebut res esperar

	if(RCREG == END_BYTE_NDEF){ //Quan el ordinador ens envii el end byte pararem
		ESTAT = ESTAT_BLINKING; //1hz
		7SEG = PART_INFERIOR;
		tempsMAX = 20; //0-20 la señal
		return //Retornara al bucle principal ja que hem vingut a desar amb goto
	}

	if(RCREG == END_BYTE_DEF){ //Quan el ordinador ens envii el end byte pararem
		ESTAT = ESTAT_BLINKING;
		7SEG = PART_SUPERIOR;
		tempsMAX = 0x00; //Ni idea, ja que sera molt brusc
		return //Retornara al bucle principal ja que hem vingut a desar amb goto
	}

	POSTINC0 = RCREG;
	comptaBytes++;
	goto bucle_desar();
}

void enviaRF(){

	if(comptaBytes != 0x00){

		reset_7Seg();

		enviarConfirmacioRf();

		if(typeS > 0x04){

			restant = 15d;

		}else{

			restant = 30d;

		}

		char byte = 0x00;

		enviarSincronitzacio();

		for(cops = 0; cops < 10; cops++{

			byte = 0;
			enviat = 0;

			while(byte < restant){ // Result es el numero de bytes total entre 10

				POSTINC0 -> AUX; //Afegim el valor de la ram al auxiliar
				tempsUn = 0x00; // Per tal de fer el temps a 0 i 1 amb la codificacio manchester, fiquem
												//tempsUn a 0 per quan salti la interrupcio
				while(enviat < 0x08){ //Enviat son els 8 bits que hem denviar

					if(tempsUn == 0x00){
						enviar_bit_pos(); //Primers 5 mseg
					}

					if(tempsUn == 0x01){
						enviar_bit_neg(); //Segon 5 mseg
						AUX >> 1; //Posem la seguent dada
						tempsUn = 0x00; //Reiniciem el comptador
					}

				}

				enviat = 0;
				byte++;

			}

			incrementa7Seg();
			enviarSegBatch();

		}

		ESTAT = ESTAT_BAK;
		7SEG = 7SEG_BAK;

	}else{

		ESTAT = ESTAT_MIG_INTENSITAT;
		7SEG = PART_CENTRAL;

	}
}

//Fet
void ENVIAR_BIT_PRIMERA_MEITAT(){
	if(AUX.Bit0){ //Si el bit a enviar es 1
		clrBit(LATC5.Bit5); //La primera part a 0
	}else{
		setBit(LATC5.Bit5); //La primera part a 1
	}
}

//Fet
void ENVIAR_BIT_SEGONA_MEITAT(){
	if(AUX.Bit0){ //Si el bit a enviar es 1
		setBit(LATC.Bit5); //La segona part a 1
	}else{
		clrBit(LATC.Bit5); //La segona part a 0
	}
	enviat++;//Hem de tenir un recompte dels bits enviats per anar a
			 //la seguent dada en la ram un cop enviats 8
}
