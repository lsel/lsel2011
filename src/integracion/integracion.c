#include <stdlib.h>
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

#define DELAY 5000
#define DISTS0 385
#define DISTS1 235
#define DISTS2 145
#define DIST01 150
#define DIST12 90
#define DIST23 145



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
struct control_tiempo tiempodiesel, tiempovapor;
int pos_tren_diesel, pos_tren_vapor; // de 0 a 3


int actualizarSensores(void){
  sensores_prev=sensores;
  if (fread(&sensores,1,sizeof(sensores),driver_llegada) != sizeof(sensores)) {
    printf("(tracking) Error fread\n");
    return -1;
  }

  return 0;
}


//data velocidad
char* tVapor = "/mnt/src/train_speed/ts 0 0";
char* tDiesel = "/mnt/src/train_speed/ts 1 0";
int velocidadVapor = 15;
int velocidadDiesel = 15;
//end data velocidad


void estimacion (int tren, int zona){

  //Estimo si es vapor. Valores por defecto para la primera vuelta
  if ((tiempovapor.t1 != 0) & (tiempovapor.t0 !=0)){
    tiempovapor.estimacionT0 = tiempovapor.estimacionT2*((DISTS0/DISTS2)+1); //calcula la estimacion desde S0 en s.
    tiempovapor.estimacionT1 = (DISTS1*(tiempovapor.t1 - tiempovapor.t0))/(1000*DIST01); //calcula la estimacion desde S1 en s.
  }else{
    tiempovapor.estimacionT0 = 14;
    tiempovapor.estimacionT1 = 8;
  }
  if ((tiempovapor.t2 !=0) & (tiempovapor.t1 !=0)){
    tiempovapor.estimacionT2 = (DISTS2*(tiempovapor.t2 - tiempovapor.t1))/(1000*DIST12); //calcula la estimacion desde S2 en s.
  }else{
    tiempovapor.estimacionT2 = 4;
  }

  //Estimo si es diesel. Valores por defecto para la primera vuelta
  if ((tiempodiesel.t1 != 0) & (tiempodiesel.t0 != 0)){
    tiempodiesel.estimacionT0 = tiempodiesel.estimacionT2*((DISTS0/DISTS2)+1); //calcula la estimacion desde S0 en s.
    tiempodiesel.estimacionT1 = (DISTS1*(tiempodiesel.t1 - tiempodiesel.t0))/(1000*DIST01); //calcula la estimacion desde S1 en s.
  }else{
    tiempodiesel.estimacionT0 = 13;
    tiempodiesel.estimacionT1 = 7;
  }
  if ((tiempodiesel.t2 !=0) & (tiempodiesel.t1 != 0)){
    tiempodiesel.estimacionT2 = (DISTS2*(tiempodiesel.t2 - tiempodiesel.t1))/(1000*DIST12); //calcula la estimacion desde S2 en s.
  }else{
    tiempodiesel.estimacionT2 = 3;
  }

  if (tren == 0){ // diesel
    if (zona == 0){
      printf("Tren diesel entra en zona 0. Tiempo estimado: %ld segundos. \n", tiempodiesel.estimacionT0);
    }
    if (zona == 1){
      printf("Tren diesel entra en zona 1. Tiempo estimado: %ld segundos. \n", tiempodiesel.estimacionT1);
    }
    if (zona == 2){
      printf("Tren diesel entra en zona 2. Tiempo estimado: %ld segundos. \n", tiempodiesel.estimacionT2);
    }
    if (zona == 3){
      printf("Tren diesel entra en zona 3. Tiempo estimado: 0 segundos. \n");
    }
  }

  if (tren == 1){ // vapor
    if (zona == 0){
      printf("Tren de vapor entra en zona 0. Tiempo estimado: %ld segundos. \n", tiempovapor.estimacionT0);
    }
    if (zona == 1){
      printf("Tren de vapor entra en zona 1. Tiempo estimado: %ld segundos. \n", tiempovapor.estimacionT1);
    }
    if (zona == 2){
      printf("Tren de vapor entra en zona 2. Tiempo estimado: %ld segundos. \n", tiempovapor.estimacionT2);
    }
    if (zona == 3){
      printf("Tren de vapor entra en zona 3. Tiempo estimado: 0 segundos. \n");
    }
  }
}

void subirDieselBajarVapor (int mod)
{
		// bajamos velocidad vapor;		
		velocidadVapor = velocidadVapor - mod;
		char comando[35];
		sprintf(comando, "%s %d", tVapor, velocidadVapor);
		out = system (comando);
		if (out == 1)
		{
			printf("Salvense quien pueda!");
		}
		// subimos velocidad diesel
		velocidadDiesel = velocidadDiesel +  mod;
		char comando2[35];
		sprintf(comando2, "%s %d", tDiesel, velocidadDiesel);
		out = system (comando2);
		if (out == 1)
		{
			printf("Salvense quien pueda!");
		}
}

void subirVaporBajarDiesel (int mod)
{
	// subimos velocidad vapor;		
		velocidadVapor = velocidadVapor + mod;
		char comando[35];
		sprintf(comando, "%s %d", tVapor, velocidadVapor);
		out = system (comando);
		if (out == 1)
		{
			printf("Salvense quien pueda!");
		}
		// bajamos velocidad diesel
		velocidadDiesel = velocidadDiesel - mod;
		char comando2[35];
		sprintf(comando2, "%s %d", tDiesel, velocidadDiesel);
		out = system (comando2);
		if (out == 1)
		{
			printf("Salvense quien pueda!");
		}
}
void controlVelocidad(int tren){


int out;

//El tren que cambia es el diesel
	if(tren==0){

//Misma zona(bajar/subir mas la velocidad)
	if (pos_tren_diesel == pos_tren_vapor) {
		subirVaporBajarDiesel(5);
	}
	
//El tren que cambia es el de vapor
}else{

//Misma zona(Bajar/subir mas la velocidad)
	if (pos_tren_diesel == pos_tren_vapor) {
		subirDieselBajarVapor(5);
	}

//Diesel alcanza una zona a vapor
	if ((pos_tren_diesel==0) & (pos_tren_vapor==1)) {
		//velocidadVapor++;
		//velocidadDiesel--;
		subirVaporBajarDiesel(2);
	}
	if ((pos_tren_diesel==1) & (pos_tren_vapor==2)) {
		subirVaporBajarDiesel(2);
	}
	if ((pos_tren_diesel==2) & (pos_tren_vapor==3)) {
		subirVaporBajarDiesel(2);
	}
	if ((pos_tren_diesel==3) & (pos_tren_vapor==0)) {
		subirVaporBajarDiesel(2);
	}

//Vapor alcanza una zona a diesel
	if ((pos_tren_diesel==1) & (pos_tren_vapor==0)) {
		//velocidadVapor--;
		//velocidadDiesel++;
		subirDieselBajarVapor(2);
	}
	if ((pos_tren_diesel==2) & (pos_tren_vapor==1)) {
		subirDieselBajarVapor(2);
	}
	if ((pos_tren_diesel==3) & (pos_tren_vapor==2)) {
		subirDieselBajarVapor(2);
	}
	if ((pos_tren_diesel==0) & (pos_tren_vapor==3)) {
		subirDieselBajarVapor(2);
	}
}


void tracking(char sensorActivo, unsigned long hora){
      
	if ((sensorActivo & S0ABAJO) != 0){
		usleep (DELAY);
		if (actualizarSensores() ==0){
			if ((sensores.estado & S0ARRIBA) != 0){
		        	pos_tren_diesel = 0;
				tiempodiesel.t0 = hora;
				estimacion(0,0);
				controlVelocidad(0);
			}else {
				pos_tren_vapor = 0;
				tiempovapor.t0 = hora;
				estimacion(1,0);
				controlVelocidad(1);
			}
		}
	}
	if ((sensorActivo & S1ABAJO) != 0){
		usleep (DELAY);
		if (actualizarSensores() ==0){
			if ((sensores.estado & S1ARRIBA) != 0){
				pos_tren_diesel = 1;
				tiempodiesel.t1 = hora;
				estimacion(0,1);
				controlVelocidad(0);
			}else {
				pos_tren_vapor = 1;
				tiempovapor.t1 = hora;
				estimacion(1,1);
				controlVelocidad(1);
			}
		}
	}	
	if ((sensorActivo & S2ABAJO) != 0){
		usleep (DELAY);
		if (actualizarSensores() ==0){
			if ((sensores.estado & S2ARRIBA) != 0){
				pos_tren_diesel = 2;
				tiempodiesel.t2 = hora;
				estimacion(0,2);
				controlVelocidad(0);
			}else {
				pos_tren_vapor = 2;
				tiempovapor.t2 = hora;
				estimacion(1,2);
				controlVelocidad(1);
			}
		}
	}
	if ((sensorActivo & S3ABAJO) != 0){
		usleep (DELAY);
		if (actualizarSensores() ==0){
			if ((sensores.estado & S3ARRIBA) != 0){
				pos_tren_diesel = 3;
				estimacion(0,3);
				controlVelocidad(0);
			}else {
				pos_tren_vapor = 3;
				estimacion(1,3);
				controlVelocidad(1);
			}
		}
	}	
}


int main(void){
    
  pos_tren_diesel=0; 
  pos_tren_vapor=0; 

  sensores_prev.estado = 0;

  tiempodiesel.t0 = 0;
  tiempodiesel.t1 = 0;
  tiempodiesel.t2 = 0;
  tiempovapor.t0 = 0;
  tiempovapor.t1 = 0;
  tiempovapor.t2 = 0;

  driver_llegada = fopen("/var/sensores", "r");
  if(driver_llegada == NULL) {
    printf("error open\n");
    return -1;
  }

  while(1){
    usleep(50000);

    if (actualizarSensores()== 0){
			if (sensores.estado!=sensores_prev.estado) {
				unsigned long horaEvento = sensores.hora_evento_ms;
				char sensorAct = ((~sensores_prev.estado) & (sensores.estado));
				tracking (sensorAct, horaEvento);
    	}
    }
  }

  return 0;
}


