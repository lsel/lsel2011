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

char sensorActivo;

struct datos_sensores {
  char estado;
  unsigned long hora_evento_ms;
};

	FILE *driver_llegada;
  struct datos_sensores sensores, sensores_prev;

int actualizarSensores(void){
		sensores_prev=sensores;
		if (fread(&sensores,1,sizeof(sensores),driver_llegada) != sizeof(sensores)) {
      			printf("(tracking) Error fread\n");
     			 	return -1;
		}

		
		return 0;
}


int main(void){
  
  
  int pos_tren_vapor=0, pos_tren_diesel=0; // de 0 a 3

  sensores_prev.estado = 0;

  driver_llegada = fopen("/var/sensores", "r");
  if(driver_llegada == NULL) {
    printf("error open\n");
    return -1;
  }

 
  while(1){
		usleep(50000);

    if (actualizarSensores()== 0){ 
			if (sensores.estado!=sensores_prev.estado) {
				printf("Los sensores han cambiado\n");
				sensorActivo = ((~sensores_prev.estado) & (sensores.estado));							
				if ((sensorActivo & S0ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S0ARRIBA) != 0){
							pos_tren_vapor = 0;
							printf("(tracking) Tren de vapor entra en la zona 0\n");
						}else {
							pos_tren_diesel = 0;
							printf("(tracking) Tren diesel entra en la zona 0\n");
						}
					}
				}
			
				if ((sensorActivo & S1ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S1ARRIBA) != 0){
							pos_tren_vapor = 1;
							printf("(tracking) Tren de vapor entra en la zona 1\n");
						}else {
							pos_tren_diesel = 1;
							printf("(tracking) Tren diesel entra en la zona 1\n");
						}
					}
				}

				if ((sensorActivo & S2ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S2ARRIBA) != 0){
							pos_tren_vapor = 2;
							printf("(tracking) Tren de vapor entra en la zona 2\n");
						}else {
							pos_tren_diesel = 2;
							printf("(tracking) Tren diesel entra en la zona 2\n");
						}
					}
				}
			
				if ((sensorActivo & S3ABAJO) != 0){
					usleep (DELAY);
					if (actualizarSensores() ==0){
						if ((sensores.estado & S3ARRIBA) != 0){
							pos_tren_vapor = 3;
							printf("(tracking) Tren de vapor entra en la zona 3\n");
						}else {
							pos_tren_diesel = 3;
							printf("(tracking) Tren diesel entra en la zona 3\n");
						}
					}
				}
 		 }
	 }


}
}
