#include <stdio.h>
#include <unistd.h>

#define S0ABAJO 0x01
#define S0ARRIBA 0x02 
#define S1ABAJO 0x04 
#define S1ARRIBA 0x08
#define S2ABAJO 0x10 
#define S2ARRIBA 0x20 
#define S3ABAJO 0x40 
#define S3ARRIBA 0x80

#define DELAY 200000
#define DISTS0 3
#define DISTS1 2
#define DISTS2 1
#define DIST01 1
#define DIST12 1
#define DIST23 1

char sensorActivo;

struct datos_sensores {
  char estado;
  unsigned long hora_evento_ms;
};

struct control_tiempo {
  unsigned long t0;
  unsigned long t1;
  unsigned long t2;
	unsigned long estimacionT0;
	unsigned long estimacionT1;
	unsigned long estimacionT2;
 };



	FILE *driver_llegada;
  struct datos_sensores sensores, sensores_prev;
	struct control_tiempo tiempoVapor, tiempoDiesel;

int actualizarSensores(void){
		sensores_prev=sensores;
		if (fread(&sensores,1,sizeof(sensores),driver_llegada) != sizeof(sensores)) {
      			printf("(tracking) Error fread\n");
     			 	return -1;
		}

		return 0;
}

void estimacion (int tren, int zona){

	//Estimo si es diesel. Valores por defecto para la primera vuelta
		if (tiempoDiesel.t1 != 0){
			tiempoDiesel.estimacionT0 = (DIST01*(tiempoDiesel.t1 - tiempoDiesel.t0))/(1000*DISTS0); //calcula la estimacion desde S0 en s.
			tiempoDiesel.estimacionT1 = (DIST01*(tiempoDiesel.t1 - tiempoDiesel.t0))/(1000*DISTS1); //calcula la estimacion desde S1 en s.
		}else{
			tiempoDiesel.estimacionT0 = 14;
			tiempoDiesel.estimacionT1 = 8;
		}
		if (tiempoDiesel.t2 !=0){
			tiempoDiesel.estimacionT2 = (DIST12*(tiempoDiesel.t2 - tiempoDiesel.t1))/(1000*DISTS2); //calcula la estimacion desde S2 en s.
		}else{
			tiempoDiesel.estimacionT2 = 4;
		}

	//Estimo si es vapor. Valores por defecto para la primera vuelta
		if (tiempoVapor.t1 != 0){
			tiempoVapor.estimacionT0 = (DIST01*(tiempoVapor.t1 - tiempoVapor.t0))/(1000*DISTS0); //calcula la estimacion desde S0 en s.
			tiempoVapor.estimacionT1 = (DIST01*(tiempoVapor.t1 - tiempoVapor.t0))/(1000*DISTS1); //calcula la estimacion desde S1 en s.
		}else{
			tiempoVapor.estimacionT0 = 14;
			tiempoVapor.estimacionT1 = 8;
		}
		if (tiempoVapor.t2 !=0){
			tiempoVapor.estimacionT2 = (DIST12*(tiempoVapor.t2 - tiempoVapor.t1))/(1000*DISTS2); //calcula la estimacion desde S2 en s.
		}else{
			tiempoVapor.estimacionT2 = 4;
		}

	if (tren == 0){  // vapor
		if (zona == 0){
			printf("(estimacion) Tren de vapor saliendo de la estacion\n");
			printf("(estimacion) Tiempo estimado de llegada para el tren de vapor: %ld segundos. \n", tiempoVapor.estimacionT0);
		}
		if (zona == 1){
			printf("(estimacion) Tiempo estimado de llegada para el tren de vapor: %ld segundos. \n", tiempoVapor.estimacionT1);
		}
		if (zona == 2){
			printf("(estimacion) Tiempo estimado de llegada para el tren de vapor: %ld segundos. \n", tiempoVapor.estimacionT2);
		}
		if (zona == 3){
			printf("(estimacion) Tren de vapor entrando en la estacion\n");
			printf("(estimacion) Tiempo estimado de llegada para el tren de vapor: 0 segundos. \n");
		}
	}

	if (tren == 1){  // diesel
		if (zona == 0){
			printf("(estimacion) Tren de vapor saliendo de la estacion\n");
			printf("(estimacion) Tiempo estimado de llegada para el tren diesel: %ld segundos. \n", tiempoDiesel.estimacionT0);
		}
		if (zona == 1){
			printf("(estimacion) Tiempo estimado de llegada para el tren diesel: %ld segundos. \n", tiempoDiesel.estimacionT1);
		}
		if (zona == 2){
			printf("(estimacion) Tiempo estimado de llegada para el tren diesel: %ld segundos. \n", tiempoDiesel.estimacionT2);
		}
		if (zona == 3){
			printf("(estimacion) Tren diesel entrando en la estacion\n");
			printf("(estimacion) Tiempo estimado de llegada para el tren diesel: 0 segundos. \n");
		}
	}
}

int main(void){
    
  int pos_tren_vapor=0, pos_tren_diesel=0; // de 0 a 3

  sensores_prev.estado = 0;

	tiempoVapor.t0 = 0;
	tiempoVapor.t1 = 0;
	tiempoVapor.t2 = 0;
	tiempoDiesel.t0 = 0;
	tiempoDiesel.t1 = 0;
	tiempoDiesel.t2 = 0;

  driver_llegada = fopen("/var/sensores", "r");
  if(driver_llegada == NULL) {
    printf("error open\n");
    return -1;
  }

 
  while(1){
		usleep(50000);

    if (actualizarSensores()== 0){ 
			if (sensores.estado!=sensores_prev.estado) {
				unsigned long hora = sensores.hora_evento_ms;				
				printf("Los sensores han cambiado\n");
				sensorActivo = ((~sensores_prev.estado) & (sensores.estado));	
        					
				if ((sensorActivo & S0ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S0ARRIBA) != 0){
							pos_tren_vapor = 0;
							printf("(tracking) Tren de vapor entra en la zona 0\n");
							tiempoVapor.t0 = hora;
							estimacion(0,0);
						}else {
							pos_tren_diesel = 0;
							printf("(tracking) Tren diesel entra en la zona 0\n");
							tiempoDiesel.t0 = hora;
							estimacion(1,0);
						}
					}
				}
			
				if ((sensorActivo & S1ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S1ARRIBA) != 0){
							pos_tren_vapor = 1;
							printf("(tracking) Tren de vapor entra en la zona 1\n");
							tiempoVapor.t1 = hora;
							estimacion(0,1);
						}else {
							pos_tren_diesel = 1;
							printf("(tracking) Tren diesel entra en la zona 1\n");
							tiempoDiesel.t1 = hora;
							estimacion(1,1);
						}
					}
				}

				if ((sensorActivo & S2ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S2ARRIBA) != 0){
							pos_tren_vapor = 2;
							printf("(tracking) Tren de vapor entra en la zona 2\n");
							tiempoVapor.t2 = hora;
							estimacion(0,2);
						}else {
							pos_tren_diesel = 2;
							printf("(tracking) Tren diesel entra en la zona 2\n");
							tiempoDiesel.t2 = hora;
							estimacion(1,2);
						}
					}
				}
			
				if ((sensorActivo & S3ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S3ARRIBA) != 0){
							pos_tren_vapor = 3;
							printf("(tracking) Tren de vapor entra en la zona 3\n");
							estimacion(0,3);
						}else {
							pos_tren_diesel = 3;
							printf("(tracking) Tren diesel entra en la zona 3\n");
							estimacion(1,3);
						}
					}
				}
 		 }
	 }


	}
  return 0;
}
