#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "reactor.h"
#include "visualizacion.h"
#define PERIODO 3
#define UPERIODO 500000

void run_visual(struct event_handler_t* eh, Train_env* train_env)
{
  struct timeval next_activation = eh->next_activation;

	printf ("\e[2JMitren Sotfware Visualization\n\n");		
	
	printf("Tren Diesel zona: ");
	switch(train_env->posTrain1){
		case 3:
			printf("\t");
		case 2:
			printf("\t");
		case 1:
			printf("\t");						
		case 0:
			printf("%d",train_env->posTrain1);
		default:
			printf("\n");
	}
	
	printf("Tren de Vapor zona: ");
	switch(train_env->posTrain2){
		case 3:
			printf("\t");
		case 2:
			printf("\t");
		case 1:
			printf("\t");						
		case 0:
			printf("%d",train_env->posTrain2);
		default:
			printf("\n");
	}
/*	
	int v1,v2,t_est;
	char next_track;
	char z1[10]="estac";
	char z2[10]= "fjlfd";
	char next_train[8] = "TRUÑO";
	v1 = train_env->speedTrain1;
	v2 = train_env->speedTrain2;
	next_track= (train_env->currentTrack == VIA_A) ? 'A' : 'B';
	t_est = train_env->testimacion;

	switch(train_env->ultimoTren){
		case 0:
			sprintf(next_train, "DIESEL");
			break;
		case 1:
			sprintf(next_train, "VAPOR");
			break;
		default:
			break;
	}

	switch(train_env->posTrain1){
		case 0:
			sprintf(z1, "TUNEL");
			break;
		case 1:
			sprintf(z1, "BARRERA");
			break;
		case 2:
			sprintf(z1, "AGUJAS");
			break;
		case 3:
			sprintf(z1, "ESTACIÓN");
			break;
		default:
			break;
	}
	switch(train_env->posTrain2){
		case 0:
			sprintf(z2, "TUNEL");
			break;
		case 1:
			sprintf(z2, "BARRERA");	
			break;
		case 2:
			sprintf(z2, "AGUJAS");
			break;
		case 3:
			sprintf(z2, "ESTACIÓN");
			break;
		default:
			break;
	}

  printf("Proyecto MiTren - LSEL 2011\n\nPróximo tren: %s\tVía: %c\tTiempo restante en segundos: %d\n",next_train, next_track, t_est);
	printf("Información sobre trenes.\nTren Diesel:\t\t\tTren de Vapor:\n");
	printf("Zona: %s\t\tZona: %s\nVelocidad: %d\t\tVelocidad: %d\n",z1,z2,v1,v2);
*/
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
