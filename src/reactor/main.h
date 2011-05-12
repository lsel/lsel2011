#ifndef MAIN_H
#define MAIN_H

#define VIA_A 1
#define VIA_B 0
#define SEM_VERDE 1
#define SEM_ROJO 0
#define DEF_SPEED 15

typedef struct{
	int posTrain1;
	int posTrain2;
	int speedTrain1;
	int speedTrain2;
	char currentTrack;
	char semA;
	char semB;
} Train_env;

void init(void);

#endif
