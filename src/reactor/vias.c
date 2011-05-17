#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "memory.h"
#include "reactor.h"
#include "sensores.h"
#include "vias.h"

#define SALIDA 0x01  // S0abajo
#define ENTRADA 0x40 // S3abajo
#define DELAY2S 2000

//interfaces con drivers
int driver_agujas;
int driver_semA;
int driver_semB;

int abreSensores()
{ 
 // Abrimos el driver de agujas ---> driver_agujas
	driver_agujas = open("/var/agujas", O_RDWR);
	driver_semA = open("/var/viaA", O_RDWR);
	driver_semB = open("/var/viaB", O_RDWR);

	if((driver_agujas < 0)||(driver_semA < 0)||(driver_semB < 0)) 
  {
    		printf("error al abrir algun fichero\n");
    		return -1;
  }
  return 0;
}

int cierraSensores()
{ 
 // cerramos el driver de agujas ---> driver_agujas
	int dagujas = close(driver_agujas);
	int dsemA = close(driver_semA);
	int dsemB = close(driver_semB);

	if((dagujas < 0)||(dsemA < 0)||(dsemB < 0)) 
  {
    		printf("error al abrir algun fichero\n");
    		return -1;
  }
  return 0;
}

void vias_notify(Observer* o, Train_env* train_env)
{
  //abrimos los sensores
	int i = abreSensores();
	if (i == -1){
		printf("error open vias");
		train_env->error = -1;
	}
	DEBUG(printf("Traza vias \n"));
	//Aquí comprobar qué ha cambiado (posición, velocidad, agujas o vías) y 
	//cambiar las vías y semáforos cuando sea pertinente.
	
	if (train_env -> currentTrack == VIA_A)
  {
     if(train_env -> semA==SEM_VERDE)
     {
    		if(train_env -> semB==SEM_VERDE) //via A verde
        { 
            if ((train_env -> cambio_sensores & ENTRADA) != 0)
              { 
                    //actualizamos las variables
                      train_env -> semA=SEM_VERDE;
                    	train_env -> semB=SEM_ROJO;
                     	train_env -> currentTrack = VIA_B;
                    //escribimos en el driver
   	         					write(driver_semA,&train_env -> semA,1); // vía B rojo
   	         					write(driver_semB,&train_env -> semB,1);
   	         					write(driver_agujas,&train_env -> currentTrack,1);
                   		}
        }else 
        if (train_env -> semB == SEM_ROJO) 
        {
				   if ((train_env -> cambio_sensores & ENTRADA) != 0)
           { 
                    //actualizamos las variables
            					train_env -> currentTrack = VIA_B;
            				//escribimos en el driver
					 						write(driver_agujas,&train_env -> currentTrack,1);
					 }
        }
	      if((train_env -> cambio_sensores & SALIDA) != 0)
        {
       						//actualizamos las variables
         						train_env -> semA=SEM_VERDE;
         						train_env -> semB=SEM_VERDE;
       					  //espera 2 seg a cambiar a verde, comprobar el tiempo 
         					  usleep(DELAY2S); 
       					  //escribimos en el driver
       	 					  write(driver_semA,&train_env -> semA,1); // vía A verde
  			 					  write(driver_semB,&train_env -> semB,1);
        }
  	}else if(train_env -> semA==SEM_ROJO)
  	{						
			// parar el tren (aunque no se tiene que dar)
     	if(train_env -> semB==SEM_VERDE)
      { 
      // vía A rojo
        if((train_env -> cambio_sensores & SALIDA) != 0)
        {
             //actualizamos las variables
         			train_env -> semA=SEM_VERDE;
         	  	train_env -> semB=SEM_VERDE;
            //escribimos en el driver
	            usleep(DELAY2S); // espera 2 seg a cambiar a verde, comprobar el tiempo 
     				  write(driver_semA,&train_env -> semA,1); // vía A verde
 						  write(driver_semB,&train_env -> semB,1);
         		  //write(driver_agujas,&VIA_B,1);
        }
    	}else
      {
            //actualizamos las variables
         			train_env -> semA=SEM_VERDE;
       
			      //Los dos semáforos en rojo. Encendemos el semáforo de la vía A
			        write(driver_semA,&train_env -> semA,1);
			      //arrancar tren via A
				 if((train_env -> cambio_sensores & SALIDA) != 0)
         { 
         //de '3' pasa a '1'
	           usleep(DELAY2S); // espera 2 seg a cambiar a verde, comprobar el tiempo 
             //actualizamos las variables
         			train_env -> semB=SEM_VERDE;
             //escribimos en el driver
         	 	 write(driver_semB,&train_env -> semB,1);  // vía B también verde
         		 //write(driver_agujas,&VIA_A,1);
					//Arrancar tren B
         }
      }
  	}

	}else if (train_env -> currentTrack == VIA_B)
  {
     if(train_env -> semB==SEM_VERDE)
     {
       if(train_env -> semA==SEM_VERDE)
       { 
       //via A verde
         if ((train_env -> cambio_sensores & ENTRADA) != 0){ 
               //actualizamos las variables
                  train_env -> semB=SEM_VERDE;
                 	train_env -> semA=SEM_ROJO;
                 	train_env -> currentTrack = VIA_A;
               //escribimos en el driver
                  write(driver_semB,&train_env -> semB,1); // vía B rojo
        	     		write(driver_semA,&train_env -> semA,1);
         	     		write(driver_agujas,&train_env -> currentTrack,1);
         }
       }else if (train_env -> semA == SEM_ROJO) 
       {
				 if ((train_env -> cambio_sensores & ENTRADA) != 0)
         { 
               //actualizamos las variables
                 train_env -> currentTrack = VIA_A;
               //escribimos en el driver
									write(driver_agujas,&train_env -> currentTrack,1);
				 }
				}
	     if((train_env -> cambio_sensores & SALIDA) != 0)
       {
        usleep(DELAY2S); // espera 2 seg a cambiar a verde, comprobar el tiempo 
             //actualizamos las variables
               train_env -> semB=SEM_VERDE;
               train_env -> semA=SEM_VERDE;
             //escribimos en el driver
        	   write(driver_semA,&train_env -> semA,1); // vía A verde
       		   write(driver_semB,&train_env -> semB,1);
     	}
     }else if(train_env -> semB==SEM_ROJO)
     {
			// parar el tren (aunque no se tiene que dar)
               if(train_env -> semA==SEM_VERDE)
               { 
               // vía A rojo
                  if((train_env -> cambio_sensores & SALIDA) != 0)
                  {
	            			  usleep(DELAY2S); // espera 2 seg a cambiar a verde, comprobar el tiempo 
                      //actualizamos las variables
                      	 train_env -> semB=SEM_VERDE;
                      	 train_env -> semA=SEM_VERDE;
                      //escribimos en el driver
         	 	 						write(driver_semA,&train_env -> semA,1); // vía A verde
         		 						write(driver_semB,&train_env -> semB,1);
         		 						//write(driver_agujas,&VIA_A,1);
               	  }
               }else
               {
			           //Los dos semáforos en rojo. Encendemos el semáforo de la vía A
                   //actualizamos las variables
                     train_env -> semA=SEM_VERDE;
                   //escribimos en el driver
         	 	         write(driver_semA,&train_env -> semA,1); // vía A verde
			             //arrancar tren via A
				           if((train_env -> cambio_sensores & SALIDA) != 0){
	            			   usleep(DELAY2S); // espera 2 seg a cambiar a verde, comprobar el tiempo
                       //actualizamos las variables
                         train_env -> semB=SEM_VERDE;
                       //escribimos en el driver 
         				         write(driver_semB,&train_env -> semB,1); // Vía B también en verde.
         				         //write(driver_agujas,&VIA_A,1);
					           //Arrancar tren B
               	   }
               }
		 	}
	}
	cierraSensores();
}

Observer* vias_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, vias_notify);
	return observer;
}
