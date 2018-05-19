#ifndef _TFUNCTION_H_
#define _TFUNCTION_H_

#define SINES                   1
#define PULSES                  2
#define SAWTOOTH                3
#define TRIANGLES               4
#define RANDOM                  16
#define CUSTOM                  17

#define EXTRA_SPACES            9
#define MAX_SIGNAL             (300 + EXTRA_SPACES)

void initTFunction();

char getFrequency(char pos);

char getVpp();

char getOffset();

char getVMin();

char getVMax();

char getVAvg();

void set_function_values(char vpp, char offset, char vmin, char vmax, char vavg, char frequency, char signal);

unsigned char* getSignal(unsigned char offset);
//Pre: 0<= offset <= MAX_SIGNAL
//Post: Retorna el missatge que hi ha actualment amb el offset especificat

char getTypeSignal();

void setFunctionSignal(char);

void setFunctionLength(int);

int getFunctionLength();
#endif
