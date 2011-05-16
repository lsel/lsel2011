#ifndef MEMORY_H
#define MEMORY_H

#define DEF_SPEED 15
#define ZONA_TUNEL 0
#define ZONA_BARRERA 1
#define ZONA_AGUJAS 2
#define ZONA_ESTACION 3
#define SEM_ROJO  0
#define SEM_VERDE 1
#define VIA_A  1
#define VIA_B  0
#define CERO 0

typedef struct{
	int posTrain1; //DIESEL 
	int posTrain2; //VAPOR
	int speedTrain1;
	int speedTrain2;
	int ultimoTren; //Ultimo tren que ha cambiado
	char currentTrack;
	char semA;
	char semB;
	unsigned long hora_evento_ms;
	char cambio_sensores;
  char estado_sensores;
	int error;
	unsigned long testimacion;
} Train_env;

void init(void);

#endif
