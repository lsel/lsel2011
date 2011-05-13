#include <stdio.h>
#include <stdlib.h>
#include "reactor.h"
#include "sensores.h"
#include "vias.h"

#define SALIDA 0x01  // S0abajo
#define ENTRADA 0x40 // S3abajo


//interfaces con drivers
int driver_agujas;
int driver_semA;
int driver_semB;

void abreSensores()
{ 
 // Abrimos el driver de agujas ---> driver_agujas
	driver_agujas = open("/var/agujas", O_RDWR);
	driver_semA = open("/var/viaA", O_RDWR);
	driver_semB = open("/var/viaB", O_RDWR);

	if((driver_agujas < 0)||(driver_semA < 0)||(driver_semB < 0)) {
    		printf("error al abrir algun fichero\n");
    		return -1;
  	}
}

void vias_notify(Observer* o, Train_env* train_env)
{
	printf("Traza vias \n");
	//Aquí comprobar qué ha cambiado (posición, velocidad, agujas o vías) y 
	//cambiar las vías y semáforos cuando sea pertinente.
	
	if (train_env -> currentTrack == VIA_A){
         	if(train_env -> semA==SEM_VERDE){
		
              		if(train_env -> semB==SEM_VERDE){ //via A verde
                   		if ((train_env -> cambio_Sensores & ENTRADA) != 0){ 
         	         		write(driver_semA,&SEM_VERDE,1); // vía B rojo
        	         		write(driver_semB,&SEM_ROJO,1);
         	         		write(driver_agujas,&VIA_B,1);
                   		}
              		}else if (train_env -> semB == SEM_ROJO) {
				if ((train_env -> cambio_Sensores & ENTRADA) != 0){ 
					write(driver_agujas,&VIA_B,1);
				}
			}
	    		if((train_env -> cambio_Sensores & SALIDA) != 0){
            			usleep(DELAY2); // espera 2 seg a cambiar a verde, comprobar el tiempo 
       	 	 		write(driver_semA,&SEM_VERDE,1); // vía A verde
       		 		write(driver_semB,&SEM_VERDE,1);
      	    		}
         	}else if(train_env -> semA==SEM_ROJO){// parar el tren (aunque no se tiene que dar)
                  	if(train_env -> semB==SEM_VERDE){ // vía A rojo
                    		if((train_env -> cambio_Sensores & SALIDA) != 0){
	            			usleep(DELAY2); // espera 2 seg a cambiar a verde, comprobar el tiempo 
         	 	 		write(driver_semA,&SEM_VERDE,1); // vía A verde
         		 		write(driver_semB,&SEM_VERDE,1);
         		 		//write(driver_agujas,&VIA_B,1);

               	    		}
                  	}else{
			//Los dos semáforos en rojo. Encendemos el semáforo de la vía A
			write(driver_semA,&SEM_VERDE,1);
			//arrancar tren via A
				if((train_env -> cambio_Sensores & SALIDA) != 0){ //de '3' pasa a '1'
	            			usleep(DELAY2); // espera 2 seg a cambiar a verde, comprobar el tiempo 
         				write(driver_semB,&SEM_VERDE,1);  // vía B también verde
         				//write(driver_agujas,&VIA_A,1);
					//Arrancar tren B
               	    		}
               		}
		}

     	}else if (train_env -> currentTrack == VIA_B){
         	if(train_env -> semB==SEM_VERDE){
              		if(train_env -> semA==SEM_VERDE){ //via A verde
                   		if ((train_env -> cambio_Sensores & ENTRADA) != 0){ 
         	         		write(driver_semB,&SEM_VERDE,1); // vía B rojo
        	         		write(driver_semA,&SEM_ROJO,1);
         	         		write(driver_agujas,&VIA_A,1);
                   		}
              		}else if (train_env -> semA == SEM_ROJO) {
				if ((train_env -> cambio_Sensores & ENTRADA) != 0){ 
					write(driver_agujas,&VIA_A,1);
				}
			}
	    		if((train_env -> cambio_Sensores & SALIDA) != 0){
            			usleep(DELAY2); // espera 2 seg a cambiar a verde, comprobar el tiempo 
       	 	 		write(driver_semA,&SEM_VERDE,1); // vía A verde
       		 		write(driver_semB,&SEM_VERDE,1);
      	    		}
         	}else if(train_env -> semB==SEM_ROJO){
			// parar el tren (aunque no se tiene que dar)

                  	if(train_env -> semA==SEM_VERDE){ // vía A rojo
                    		if((train_env -> cambio_Sensores & SALIDA) != 0){
	            			usleep(DELAY2); // espera 2 seg a cambiar a verde, comprobar el tiempo 
         	 	 		write(driver_semA,&SEM_VERDE,1); // vía A verde
         		 		write(driver_semB,&SEM_VERDE,1);
         		 		//write(driver_agujas,&VIA_A,1);
               	    		}
                  	}else{
			//Los dos semáforos en rojo. Encendemos el semáforo de la vía A
         	 	write(driver_semA,&SEM_VERDE,1); // vía A verde
			//arrancar tren via A
				if((train_env -> cambio_Sensores & SALIDA) != 0){
	            			usleep(DELAY2); // espera 2 seg a cambiar a verde, comprobar el tiempo 
         				write(driver_semB,&SEM_VERDE,1); // Vía B también en verde.
         				//write(driver_agujas,&VIA_A,1);
					//Arrancar tren B
               	    		}
               		}
		}
     	} 
}

Observer* vias_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, vias_notify);
	return observer;
}
