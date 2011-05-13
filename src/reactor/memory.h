#ifndef MEMORY_H
#define MEMORY_H

#define DEF_SPEED 15

const char SEM_ROJO = 0;
const char SEM_VERDE = 1;
const char VIA_A = 1;
const char VIA_B = 0;

typedef struct{
	int posTrain1; //DIESEL 
	int posTrain2; //VAPOR
	int speedTrain1;
	int speedTrain2;
	char currentTrack;
	char semA;
	char semB;
	unsigned long hora_evento_ms;
	char cambio_sensores;
  	char estado_sensores;
	int error;
} Train_env;

void init(void);

#endif
