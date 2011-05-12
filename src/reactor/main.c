#include "main.h"
#include "reactor.h"
#include "sensores.h"
#include "tracking.h"
#include "vias.h"
#include "estimacion.h"
#include "cambiovelocidad.h"

Train_env train_env;

void init (void){
	train_env.posTrain1 = 3;	//Tren 1 sale desde la estación.
	train_env.posTrain2 = 1;	//Tren 2 sale desde la barrera.
	train_env.speedTrain1 = DEF_SPEED;	//Velocidad por defecto.
	train_env.speedTrain2 = DEF_SPEED;
	train_env.currentTrack = VIA_A;
	train_env.semA = SEM_VERDE;
	train_env.semB = SEM_VERDE;
}

int main()
{
	init();
  reactor_init();
	//EH de sensores, cuyos observadores son vías y estimación
  EventHandler* sensores_eh = sensores_eh_new("nada", 1);
  Observer* vias = vias_observer_new();
  Observer* estimacion = estimacion_observer_new();
  sensores_eh_add_observer ((SensoresEH*)sensores_eh, vias);
  sensores_eh_add_observer ((SensoresEH*)sensores_eh, estimacion);
  reactor_add_handler(sensores_eh);
	//EH de tracking, cuyo observador es cambio de velocidad
  EventHandler* track_eh = tracking_eh_new("nada", 2);
  Observer* cambiovelocidad = cambiovelocidad_observer_new();
  tracking_eh_add_observer ((TrackingEH*)track_eh, cambiovelocidad);
  reactor_add_handler(track_eh);

  while(1) {
    reactor_handle_events(&train_env);
  }
  return 1;
}
