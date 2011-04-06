/* Driver tiempo llegada trenes */


/* #ifndef __KERNEL__ */
/* #define __KERNEL__ */
/* #endif */

/* #ifndef MODULE */
/* #define MODULE */
/* #endif */


#include <stdio.h>
#include <unistd.h>
#define MCF_BAR 0x10000000
#define IO_N_MAJOR 250
#define IO_N_MINOR1 0
#define NUM_SENSORES 2
#define NUM_EJECUCIONES 100

struct datos_sensores {
  char estado_sensores;
  unsigned long hora_evento_ms;        
};

int main(void){
  
  FILE *driver_llegada;
  struct datos_sensores sensores;
  int j;
  
  driver_llegada = fopen("/var/sensores", "r");
  if(driver_llegada == NULL) {
    printf("error open\n");
    return -1;
  }
  
  for (j=0; j<NUM_EJECUCIONES; j++)
    {
      fread(&sensores,1,sizeof(sensores),driver_llegada);
      printf("Lectura de sensores.estado_sensores=0x%02X\n",sensores.estado_sensores&0xFF);
      printf("Lectura de sensores.hora=%lu milliseconds\n",sensores.hora_evento_ms);
      sleep(1);
      printf("\n");
    }
  fclose(driver_llegada);
  return 0;
}
