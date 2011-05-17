#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "reactor.h"
#include "tracking.h"

#define PERIODO 1
#define UPERIODO 500000

#define S0ABAJO 0x01
#define S0ARRIBA 0x02
#define S1ABAJO 0x04
#define S1ARRIBA 0x08
#define S2ABAJO 0x10
#define S2ARRIBA 0x20
#define S3ABAJO 0x40
#define S3ARRIBA 0x80


void run_tracking(struct event_handler_t* eh, Train_env* train_env)
{
  TrackingEH* track_eh = (TrackingEH*) eh;
  struct timeval next_activation = eh->next_activation;
	
  DEBUG(printf("Traza tracking\n"));
  if ((train_env -> cambio_sensores & S0ABAJO) != 0){
    if ((train_env -> estado_sensores & S0ARRIBA) != 0){
      train_env -> posTrain1 = 0;
			train_env -> ultimoTren = 0;
    }else{
      train_env -> posTrain2 = 0;
			train_env -> ultimoTren = 1;
    }
  }

  if ((train_env -> cambio_sensores & S1ABAJO) != 0){;
    if ((train_env -> estado_sensores & S1ARRIBA) != 0){
      train_env -> posTrain1 = 1;
			train_env -> ultimoTren = 0;
    }else {
      train_env -> posTrain2 = 1;
			train_env -> ultimoTren = 1;
    }
  }	

  if ((train_env -> cambio_sensores & S2ABAJO) != 0){
    if ((train_env -> estado_sensores & S2ARRIBA) != 0){
      train_env -> posTrain1 = 2;
			train_env -> ultimoTren = 0;
    }else {
      train_env -> posTrain2 = 2;
			train_env -> ultimoTren = 1;
    }
  }

  if ((train_env -> cambio_sensores & S3ABAJO) != 0){
    if ((train_env -> estado_sensores & S3ARRIBA) != 0){
      train_env -> posTrain1 = 3;
			train_env -> ultimoTren = 0;
    }else {
      train_env -> posTrain2 = 3;
			train_env -> ultimoTren = 1;
    }
  }	
	
  //next call
  next_activation.tv_sec += PERIODO;
  next_activation.tv_usec += UPERIODO;
  reactor_delay_until (&next_activation);
  //Si las posiciones han cambiado, evaluar cambio de velocidad.	
  observable_notify_all (&track_eh->observable, train_env);
}

EventHandler* tracking_eh_new (const char* dev, int prio) 
{
	TrackingEH* track_eh = (TrackingEH*) malloc(sizeof(TrackingEH));
	EventHandler* eh = &track_eh->eh;
	Observable* observable = &track_eh->observable;
	event_handler_init(eh, prio, run_tracking);
	observable_init (observable);
	//eh->fd = -1;
	eh->events = EVENTS_TIMEOUT;
	return eh;
}

void tracking_eh_add_observer (TrackingEH* this, Observer* o)
{
  observable_add_observer (&this->observable, o);
}

