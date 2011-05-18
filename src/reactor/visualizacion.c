#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "reactor.h"
#include "visualizacion.h"
#define PERIODO 0
#define UPERIODO 500000

void run_visual(struct event_handler_t* eh, Train_env* train_env)
{
  struct timeval next_activation = eh->next_activation;
	char tren[8];
	char via[2];
	unsigned long testimacion = 0;

	printf ("\e[2JMitren Sotfware visualization \n\n");		
	printf("        . . . . o o o o o\n");
	printf("               _____      o      ________\n");
	printf("      ____====  ]OO|_n_n__][.    |  Mi  |\n");
	printf("     [________]_|__|________)<   | tren |\n");
	printf("      oo    oo  'oo OOOO-| oo\\_  ~~~||~~~\n");
	printf("  +--+--+--+--+--+--+--+--+-$1-+--+--+--+--+\n");
	
	// Próximo tren (tren) es el que está más cerca de la estación.
	//Es el diesel
	if ((train_env -> posTrain1 == 3) | (train_env->posTrain2 ==3) ){
		if ( train_env -> posTrain2 == 3)
		{
			sprintf(tren, "diesel");
			testimacion = train_env -> testimacionDiesel;
		}
		if ( train_env -> posTrain1 == 3)
		{
			sprintf(tren, "vapor");
			testimacion = train_env -> testimacionVapor;
		}
	}
	else{
		if (train_env->posTrain1 > train_env->posTrain2)
		{
			sprintf(tren, "diesel");
			testimacion = train_env -> testimacionDiesel;
		}
		//Es el de vapor
		if (train_env->posTrain1 < train_env->posTrain2)
		{
			sprintf(tren, "vapor");
			testimacion = train_env -> testimacionVapor;
		}
	//En caso de que estén en la misma zona (no debería pasar), el último en cambiar
	//ha sido el último en entrar, por tanto primero llegará el que ya estaba en esa zona.
		if (train_env->posTrain1 == train_env->posTrain2)
		{
			if (train_env->ultimoTren == DIESEL)
			{
				sprintf(tren, "vapor");
				testimacion = train_env -> testimacionVapor;
			}
			else{
				sprintf(tren, "diesel");
				testimacion = train_env -> testimacionDiesel;
			}
		}
	}
	//Próxima vía: SI hay un tren en la estación, es la del tren, si no lo hay, es la posición de las vías.
	if ((train_env->posTrain1 == ZONA_ESTACION) | (train_env->posTrain2 == ZONA_ESTACION))
	{
		if (train_env->currentTrack == VIA_A){
			sprintf(via, "B");
		} else {
			sprintf(via, "A");
		}
	} 
	else{
		if (train_env->currentTrack == VIA_A){
			sprintf(via, "A");
		} else {
			sprintf(via, "B");
		}
	}
	//info prox tren en la estación	
	printf("\nPróximo tren (%s) efectuará entrada en la estación por vía: %s en %ld\n\n", tren, via, testimacion);
	//info trenes
	printf("InfoTrenes\n");
	printf("Tren Diesel:\n");
	printf("\tEstimación de llegada: %ld segundos\n",  train_env->testimacionDiesel);
	printf("\tVelocidad: %2d\n", train_env->speedTrain1);
	printf("\tZona:");
	switch(train_env->posTrain1){
		case 3:
			printf("\t");
		case 2:
			printf("\t");
		case 1:
			printf("\t");						
		case 0:
			printf("\t%d",train_env->posTrain1);
		default:
			printf("\n");
	}
	//printf("\tEstimación de llegada: %2d segundos\n", 1);
	//printf("\tVelocidad: %2d\n", train_env->speedTrain1);

	printf("Tren de Vapor:\n");
	printf("\tEstimación de llegada: %ld segundos\n", train_env->testimacionVapor);
	printf("\tVelocidad: %2d\n", train_env->speedTrain2);
	printf("\tZona:");
	switch(train_env->posTrain2){
		case 3:
			printf("\t");
		case 2:
			printf("\t");
		case 1:
			printf("\t");						
		case 0:
			printf("\t%d",train_env->posTrain2);
		default:
			printf("\n");
	}
	
	printf("\n\n");		

  //next call
  next_activation.tv_sec += PERIODO;
  next_activation.tv_usec += UPERIODO;
  reactor_delay_until (&next_activation);
}

EventHandler* visualizacion_eh_new (const char* dev, int prio) 
{
	VisualizacionEH* visual_eh = (VisualizacionEH*) malloc(sizeof(VisualizacionEH));
	EventHandler* eh = &visual_eh->eh;
	event_handler_init(eh, prio, run_visual);
	eh->events = EVENTS_TIMEOUT;
	return eh;
}
