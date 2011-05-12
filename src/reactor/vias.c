#include <stdio.h>
#include <stdlib.h>
#include "reactor.h"
#include "sensores.h"
#include "vias.h"

void vias_notify(Observer* o, Train_env* train_env)
{
	printf("Traza vias \n");
	//Aquí comprobar qué ha cambiado (posición, velocidad, agujas o vías) y 
	//cambiar las vías y semáforos cuando sea pertinente.
}

Observer* vias_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, vias_notify);
	return observer;
}
