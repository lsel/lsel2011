#include <stdio.h>
#include <unistd.h>

#define S0MASK 0x03
#define S1MASK 0x0C
#define S2MASK 0x30
#define S3MASK 0xC0

struct datos_sensores {
  char estado;
  unsigned long hora_evento_ms;        
};

int main(void){
  
  FILE *driver_llegada;
  struct datos_sensores sensores, sensores_prev;
  int pos_tren_vapor=0, pos_tren_diesel=0; // de 0 a 3

  sensores_prev.estado = 0;

  driver_llegada = fopen("/var/sensores", "r");
  if(driver_llegada == NULL) {
    printf("error open\n");
    return -1;
  }

  while(1){
    if (fread(&sensores,1,sizeof(sensores),driver_llegada) != sizeof(sensores)) {
      printf("(tracking) Error fread\n");
      return -1;
    }

    if sensores.estado!=sensores_prev.estado {
	if (sensores.estado&S0MASK)!=(sensores_prev.estado&S0MASK) {
	    if (sensores.estado&S0MASK) == 0x03 pos_tren_vapor=0;
	    else pos_tren_diesel=0;
	  }
	if (sensores.estado&S1MASK)!=(sensores_prev.estado&S1MASK) {
	    if ((sensores.estado&S1MASK)>>2) == 0x03 pos_tren_vapor=1;
	    else pos_tren_diesel=1;
	  }
	if (sensores.estado&S2MASK)!=(sensores_prev.estado&S2MASK) {
	    if ((sensores.estado&S2MASK)>>4) == 0x03 pos_tren_vapor=2;
	    else pos_tren_diesel=2;
	  }
	if (sensores.estado&S3MASK)!=(sensores_prev.estado&S3MASK) {
	    if ((sensores.estado&S3MASK)>>6) == 0x03 pos_tren_vapor=3;
	    else pos_tren_diesel=3;
	  }
      }
    
    sensores_prev=sensores;
    printf("(tracking) Posicion tren vapor:  sector %d\n",pos_tren_vapor);
    printf("(tracking) Posicion tren diesel: sector %d\n",pos_tren_diesel);
  }
}
