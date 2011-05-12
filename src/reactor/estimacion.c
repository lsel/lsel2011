#include <stdio.h>
#include <stdlib.h>
#include "reactor.h"
#include "sensores.h"
#include "estimacion.h"

void estimacion_notify(Observer* o, Train_env* train_env)
{
	printf("Traza estimación \n");
	//Aquí realizar la estimación
}

Observer* estimacion_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, estimacion_notify);
	return observer;
}
