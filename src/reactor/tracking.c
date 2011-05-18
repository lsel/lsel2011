#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "reactor.h"
#include "tracking.h"

void run_tracking(struct event_handler_t* eh, Train_env* train_env)
{
  TrackingEH* track_eh = (TrackingEH*) eh;
  struct timeval next_activation = eh->next_activation;
	
  DEBUG(printf("Traza tracking\n"));
	//Evento de salida en la estación (entrada hacia el túnel)
  if ((train_env -> cambio_sensores & S0ABAJO) != 0){
		//Si es el diesel, se actualiza su posición y el último tren que ha cambiado.
    if ((train_env -> estado_sensores & S0ARRIBA) != 0){
      train_env -> posTrain1 = ZONA_TUNEL;
			train_env -> ultimoTren = DIESEL;
    }else{
		//Se actualiza el de vapor.
      train_env -> posTrain2 = ZONA_TUNEL;
			train_env -> ultimoTren = VAPOR;
    }
  }
	//Evento de salida del túnel (entrada hacia barrera)
  if ((train_env -> cambio_sensores & S1ABAJO) != 0){;
		//Es el tren diesel
    if ((train_env -> estado_sensores & S1ARRIBA) != 0){
      train_env -> posTrain1 = ZONA_BARRERA;
			train_env -> ultimoTren = DIESEL;
    }else {
		//Es el tren de vapor
      train_env -> posTrain2 = ZONA_BARRERA;
			train_env -> ultimoTren = VAPOR;
    }
  }	
	//Evento de salida de la barrera (entrada hacia agujas)
  if ((train_env -> cambio_sensores & S2ABAJO) != 0){
		//Es el tren diesel
    if ((train_env -> estado_sensores & S2ARRIBA) != 0){
      train_env -> posTrain1 = ZONA_AGUJAS;
			train_env -> ultimoTren = DIESEL;
    }else {
		//Es el tren de vapor
      train_env -> posTrain2 = ZONA_AGUJAS;
			train_env -> ultimoTren = VAPOR;
    }
  }
	//Evento de salida de agujas (entrada hacia estación)
  if ((train_env -> cambio_sensores & S3ABAJO) != 0){
		//Es el tren diesel
    if ((train_env -> estado_sensores & S3ARRIBA) != 0){
      train_env -> posTrain1 = ZONA_ESTACION;
			train_env -> ultimoTren = DIESEL;
    }else {
		//Es el tren de vapor    
  		train_env -> posTrain2 = ZONA_ESTACION;
			train_env -> ultimoTren = VAPOR;
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

