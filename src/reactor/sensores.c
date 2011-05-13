#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "reactor.h"
#include "sensores.h"
#define PERIODO 0
#define UPERIODO 500000

void run_sensores(struct event_handler_t* eh, Train_env* train_env)
{
	
	printf("Traza sensores\n");
	train_env->semA = SEM_ROJO;
	

	//next call
	SensoresEH* seh = (SensoresEH*) eh;
	struct timeval next_activation = eh->next_activation;
	next_activation.tv_sec += PERIODO;
	next_activation.tv_usec += UPERIODO;
	reactor_delay_until (&next_activation);
	//Si los sensores han cambiado, evaluar vías y estimación.	
	observable_notify_all (&seh->observable, train_env);
}

EventHandler* sensores_eh_new (const char* dev, int prio) 
{
	SensoresEH* seh = (SensoresEH*) malloc(sizeof(SensoresEH));
	EventHandler* eh = &seh->eh;
	Observable* observable = &seh->observable;
	event_handler_init(eh, prio, run_sensores);
	observable_init (observable);
	//eh->fd = -1;
	eh->events = EVENTS_TIMEOUT;
	return eh;
}

void sensores_eh_add_observer (SensoresEH* this, Observer* o)
{
  observable_add_observer (&this->observable, o);
}

