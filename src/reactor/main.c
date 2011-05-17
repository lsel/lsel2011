#include "memory.h"
#include "reactor.h"
#include "sensores.h"
#include "tracking.h"
#include "vias.h"
#include "estimacion.h"
#include "cambiovelocidad.h"
#include "visualizacion.h"

Train_env train_env;

void init (void){
	train_env.posTrain1 = ZONA_ESTACION;//Posición inicial para el tren Diesel
	train_env.posTrain2 = ZONA_BARRERA;	//Posición inicial para el tren de Vapor
	train_env.speedTrain1 = DEF_SPEED;	//Velocidad por defecto
	train_env.speedTrain2 = DEF_SPEED;
	train_env.currentTrack = VIA_A;			//Vía A por defecto
	train_env.semA = SEM_VERDE;					//Semáforos inicialmente en verde
	train_env.semB = SEM_VERDE;
	train_env.hora_evento_ms = CERO;
	train_env.cambio_sensores = CERO;
  train_env.estado_sensores = CERO;
	train_env.ultimoTren = CERO; //Ultimo tren que ha cambiado el diesel
	train_env.error = CERO;
}

int main()
{
	//Declaración de event handlers y observers
	EventHandler* sensores_eh;
	Observer* vias;
	Observer* estimacion;
	EventHandler* track_eh;
  Observer* cambiovelocidad;
	EventHandler* visual_eh;
	init();
  reactor_init();
	//EH de sensores, cuyos observadores son vías y estimación
  sensores_eh = sensores_eh_new("nada", 5);
  vias = vias_observer_new();
  estimacion = estimacion_observer_new();
  sensores_eh_add_observer ((SensoresEH*)sensores_eh, vias);
  sensores_eh_add_observer ((SensoresEH*)sensores_eh, estimacion);
  reactor_add_handler(sensores_eh);
	//EH de tracking, cuyo observador es cambio de velocidad
  track_eh = tracking_eh_new("nada", 4);
  cambiovelocidad = cambiovelocidad_observer_new();
  tracking_eh_add_observer ((TrackingEH*)track_eh, cambiovelocidad);
  reactor_add_handler(track_eh);

	//EH de visualizacion, no tiene observadores
	visual_eh = visualizacion_eh_new("nada", 3);
  reactor_add_handler(visual_eh);

  while(1) {
    reactor_handle_events(&train_env);
  }
  return 1;
}
