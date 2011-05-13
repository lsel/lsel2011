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
	
  printf("Visualizacion\n");
	
	

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
