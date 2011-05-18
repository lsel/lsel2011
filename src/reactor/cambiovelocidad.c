#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "reactor.h"
#include "tracking.h"
#include "cambiovelocidad.h"
#include "memory.h"

//Para evitar que haga nada system (ahora no es necesario el cambio de velocidad e imprime muchas cosas en pantalla)
#define system(x) 0

//data velocidad
char* tVapor = "/mnt/src/train_speed/ts 0 1";
char* tDiesel = "/mnt/src/train_speed/ts 1 1";
//end data velocidad

	void subirDieselBajarVapor (int mod, Train_env* train_env)
{
		int out;
		char comando[35];
		char comando2[35];
		// bajamos velocidad vapor;		
		train_env -> speedTrain2 = (train_env -> speedTrain2) - mod;
		if ((train_env -> speedTrain2) < 0)
		{
			train_env -> speedTrain2 = 0;
		}		
		sprintf(comando, "%s %d", tVapor, train_env -> speedTrain2);
		out = system (comando);
		if (out == 1)
		{
			printf("Salvense quien pueda!");
			train_env->error = -1;
		}
		// subimos velocidad diesel
		train_env -> speedTrain1 = (train_env -> speedTrain1) +  mod;
		if ((train_env -> speedTrain1) > 30)
		{
			train_env -> speedTrain1 = 30;
		}
		sprintf(comando2, "%s %d", tDiesel, train_env -> speedTrain1);
		out = system (comando2);
		if (out == 1)
		{
			printf("Salvense quien pueda!");
			train_env->error = -1;
		}
}

void subirVaporBajarDiesel (int mod, Train_env* train_env)
{		
		int out;
		char comando[35];
		char comando2[35];
	// subimos velocidad vapor;		
		train_env -> speedTrain2 = (train_env -> speedTrain2) + mod;
		if ((train_env -> speedTrain2) > 30)
		{
			train_env -> speedTrain2 = 30;
		}
		sprintf(comando, "%s %d", tVapor, train_env -> speedTrain2);
		out = system (comando);
		if (out == 1)
		{
			printf("Sálvese quien pueda!");
			train_env->error = -1;
		}
		// bajamos velocidad diesel
		train_env -> speedTrain1 = (train_env -> speedTrain1) - mod;
		if ((train_env -> speedTrain1) < 0)
		{
			train_env -> speedTrain1 = 0;
		}
		sprintf(comando2, "%s %d", tDiesel, train_env -> speedTrain1);
		out = system (comando2);
		if (out == 1)
		{
			printf("Sálvese quien pueda!");
			train_env->error = -1;
		}
}

void cambiovelocidad_notify(Observer* o, Train_env* train_env)
{
	DEBUG(printf("Traza cambio de velocidad \n"));
	//Aquí realizar cambios de velocidad

	//El tren que cambia es el diesel
	if(train_env -> ultimoTren ==VAPOR){

		//Misma zona(bajar/subir mas la velocidad)
		if (train_env -> posTrain1 == train_env -> posTrain2) {
			subirVaporBajarDiesel(5, train_env);
		}
	
	//El tren que cambia es el de vapor
	}else{

	//Misma zona(Bajar/subir mas la velocidad)
		if (train_env -> posTrain1 == train_env -> posTrain2) {
		subirDieselBajarVapor(5, train_env);
		}
	}
	//Diesel alcanza una zona a vapor
	if ((train_env -> posTrain1==ZONA_TUNEL) & (train_env -> posTrain2==ZONA_BARRERA)) {
		//train_env -> speedTrain2++;
		//train_env -> speedTrain1--;
		subirVaporBajarDiesel(2, train_env);
	}
	if ((train_env -> posTrain1==ZONA_BARRERA) & (train_env -> posTrain2==ZONA_AGUJAS)) {
		subirVaporBajarDiesel(2, train_env);
	}
	if ((train_env -> posTrain1==ZONA_AGUJAS) & (train_env -> posTrain2==ZONA_ESTACION)) {
		subirVaporBajarDiesel(2, train_env);
	}
	if ((train_env -> posTrain1==ZONA_ESTACION) & (train_env -> posTrain2==ZONA_TUNEL)) {
		subirVaporBajarDiesel(2, train_env);
	}
	
	//Vapor alcanza una zona a diesel
	if ((train_env -> posTrain1==ZONA_BARRERA) & (train_env -> posTrain2==ZONA_TUNEL)) {
		//train_env -> speedTrain2--;
		//train_env -> speedTrain1++;
		subirDieselBajarVapor(2, train_env);
	}
	if ((train_env -> posTrain1==ZONA_AGUJAS) & (train_env -> posTrain2==ZONA_BARRERA)) {
		subirDieselBajarVapor(2, train_env);
	}
	if ((train_env -> posTrain1==ZONA_ESTACION) & (train_env -> posTrain2==ZONA_AGUJAS)) {
		subirDieselBajarVapor(2, train_env);
	}
	if ((train_env -> posTrain1==ZONA_TUNEL) & (train_env -> posTrain2==ZONA_ESTACION)) {
		subirDieselBajarVapor(2, train_env);
	}
}

Observer* cambiovelocidad_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, cambiovelocidad_notify);
	return observer;
}
