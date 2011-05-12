#include <stdio.h>
#include <stdlib.h>
#include "reactor.h"
#include "tracking.h"
#include "cambiovelocidad.h"

void cambiovelocidad_notify(Observer* o, Train_env* train_env)
{
	printf("Traza cambio de velocidad \n");
	//Aquí realizar cambios de velocidad
}

Observer* cambiovelocidad_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, cambiovelocidad_notify);
	return observer;
}
