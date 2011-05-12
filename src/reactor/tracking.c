#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "reactor.h"
#include "tracking.h"
#define PERIODO 1
#define UPERIODO 500000

void run_tracking(struct event_handler_t* eh, Train_env* train_env)
{
	
	printf("Traza tracking\n");
	train_env->semB = SEM_ROJO;
	
	//next call
	TrackingEH* track_eh = (TrackingEH*) eh;
	struct timeval next_activation = eh->next_activation;
	next_activation.tv_sec += PERIODO;
	next_activation.tv_usec += UPERIODO;
	reactor_delay_until (&next_activation);
	//Si los sensores han cambiado, evaluar cambio de velocidad.	
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

