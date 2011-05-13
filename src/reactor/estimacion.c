#include <stdio.h>
#include <stdlib.h>
#include "reactor.h"
#include "sensores.h"
#include "estimacion.h"
#include "memory.h"

#define DISTS0 385
#define DISTS1 235
#define DISTS2 145
#define DIST01 150
#define DIST12 90
#define DIST23 145
#define DIST30 90

int posTrain1_prev=3; //estacion
int posTrain2_prev=1;	//barrera

struct control_tiempo {
  unsigned long t0;
  unsigned long t1;
  unsigned long t2;
  unsigned long t3;
  unsigned long estimacionT0;
  unsigned long estimacionT1;
  unsigned long estimacionT2;
};

struct control_tiempo train1, train2;

void estimacion_notify(Observer* o, Train_env* train_env)
{
  printf("Traza estimación \n");
  //Aquí realizar la estimación
  
  if (train_env->posTrain1 != posTrain1_prev){  // diesel
    posTrain1_prev = train_env->posTrain1;

    if (posTrain1_prev == 0){
      train1.t0 = train_env->hora_evento_ms;
      if (train1.t3 != 0){
				train1.estimacionT0 = (DISTS0*(train1.t0 - train1.t3))/(1000*DIST30);	
      }else{
				train1.estimacionT0 = 13;
      }
      printf("Tren diesel entra en zona 0. Tiempo estimado: %ld segundos. \n", train1.estimacionT0);
    }
    if (posTrain1_prev == 1){
  	    train1.t1 = train_env->hora_evento_ms;
      if (train1.t0 != 0){
				train1.estimacionT1 = (DISTS1*(train1.t1 - train1.t0))/(1000*DIST01);		
      }else{
				train1.estimacionT1 = 7;
      }
      printf("Tren diesel entra en zona 1. Tiempo estimado: %ld segundos. \n", train1.estimacionT1);
    }
    if (posTrain1_prev == 2){
      train1.t2 = train_env->hora_evento_ms;
      if (train1.t1 != 0){
				train1.estimacionT2 = (DISTS2*(train1.t2 - train1.t1))/(1000*DIST12);		
      }else{
				train1.estimacionT2 = 3;
      }
      printf("Tren diesel entra en zona 2. Tiempo estimado: %ld segundos. \n", train1.estimacionT2);
    }
    if (posTrain1_prev == 3){
      train1.t3 = train_env->hora_evento_ms;
      printf("Tren diesel entra en zona 3. Tiempo estimado: 0 segundos. \n");
    }
  }

  if (train_env->posTrain2 != posTrain2_prev){  // vapor
    posTrain2_prev = train_env->posTrain2;
		
    if (posTrain2_prev == 0){
      train2.t0 = train_env->hora_evento_ms;
      if (train2.t3 != 0){
				train2.estimacionT0 = (DISTS0*(train2.t0 - train2.t3))/(1000*DIST30);	
      }else{
				train2.estimacionT0 = 13;
      }
      printf("Tren de vapor entra en zona 0. Tiempo estimado: %ld segundos. \n", train2.estimacionT0);
    }
    if (posTrain2_prev == 1){
      train2.t1 = train_env->hora_evento_ms;
      if (train2.t0 != 0){
				train2.estimacionT1 = (DISTS1*(train2.t1 - train2.t0))/(1000*DIST01);		
      }else{
				train2.estimacionT1 = 8;
      }
      printf("Tren de vapor entra en zona 1. Tiempo estimado: %ld segundos. \n", train2.estimacionT1);
    }
    if (posTrain2_prev == 2){
      train2.t2 = train_env->hora_evento_ms;
      if (train2.t1 != 0){
				train2.estimacionT2 = (DISTS2*(train2.t2 - train2.t1))/(1000*DIST12);		
      }else{
				train2.estimacionT2 = 4;
      }
      printf("Tren de vapor entra en zona 2. Tiempo estimado: %ld segundos. \n", train2.estimacionT2);
    }
    if (posTrain2_prev == 3){
      train2.t3 = train_env->hora_evento_ms;
      printf("Tren de vapor entra en zona 3. Tiempo estimado: 0 segundos. \n");
    }
  }
}

Observer* estimacion_observer_new () 
{
	Observer* observer = (Observer*) malloc(sizeof(Observer));
  observer_init (observer, estimacion_notify);
	return observer;
}
