#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "reactor.h"
#include "sensores.h"
#define PERIODO 0
#define UPERIODO 200000

struct datos_sensores {
  char estado;
  unsigned long hora_evento_ms;
};

struct datos_sensores sensores, sensores_prev;
char cambio_sensores_next = 0;

FILE *driver_llegada;


int actualizarSensores(void){
  sensores_prev=sensores;
  if (fread(&sensores,1,sizeof(sensores),driver_llegada) != sizeof(sensores)) {
    printf("Error fread\n");
    return -1;
  }

  return 0;
}

void run_sensores(struct event_handler_t* eh, Train_env* train_env)
{
  //next call
  SensoresEH* seh = (SensoresEH*) eh;
  struct timeval next_activation = eh->next_activation;
	
  DEBUG(printf("Traza sensores\n"));
  driver_llegada = fopen("/var/sensores", "r");
  if(driver_llegada == NULL) {
    printf("error sensores open\n");
    train_env -> error = -1;
  }

  if (actualizarSensores()== 0){
    if (sensores.estado!=sensores_prev.estado) {
      train_env -> hora_evento_ms = sensores.hora_evento_ms;
      train_env -> cambio_sensores = ((~sensores_prev.estado) & (sensores.estado));
      train_env -> estado_sensores = sensores.estado;
      DEBUG(printf ("Los sensores han cambiado"));
    }
  }

  next_activation.tv_sec += PERIODO;
  next_activation.tv_usec += UPERIODO;
  reactor_delay_until (&next_activation);
  //Si los sensores han cambiado, evaluar vías y estimación.
	if (cambio_sensores_next != train_env -> cambio_sensores) {
  	observable_notify_all (&seh->observable, train_env);
		cambio_sensores_next = train_env -> cambio_sensores;
	}
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

