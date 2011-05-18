#ifndef MEMORY_H
#define MEMORY_H

#define CERO 0
#define CUATRO 4
//Velocidad
#define DEF_SPEED 15
//Zonas de la vía
#define ZONA_TUNEL 0
#define ZONA_BARRERA 1
#define ZONA_AGUJAS 2
#define ZONA_ESTACION 3
//Vías y semáforos.
#define SEM_ROJO  0
#define SEM_VERDE 1
#define VIA_A  1
#define VIA_B  0
//Trenes
#define VAPOR 0
#define DIESEL 1
//Sensores
#define S0ABAJO 0x01
#define S0ARRIBA 0x02
#define S1ABAJO 0x04
#define S1ARRIBA 0x08
#define S2ABAJO 0x10
#define S2ARRIBA 0x20
#define S3ABAJO 0x40
#define S3ARRIBA 0x80
//DEBUG REACTOR
#ifdef DEBUG_REACTOR
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

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
	unsigned long testimacionDiesel;
	unsigned long testimacionVapor;
} Train_env;

void init(void);

#endif
