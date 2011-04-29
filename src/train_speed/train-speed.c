#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "funciones_sms.h"

uint8_t check_paridad(uint8_t word) 
{
	
  uint8_t num_ones = 0;
  uint8_t mask = 0x01;
	uint8_t i;	
  for (i=0; i< 8;i++) {
    if(word & mask)
      num_ones++;
    mask = mask << 1;
  }
  if (num_ones % 2 == 0) {
    return 0;
  } else {
    return 1;
  }
}

int main(int argc, char* argv[]) 
{
	
  uint8_t number_train = atoi(argv[1]);
  uint8_t number_way = atoi(argv[2]);
  uint8_t number_speed = atoi(argv[3]);
  int ret;	
  
  uint8_t final, paridad;
//  FILE * puertoserie;
    int puertoserie;
	
  uint8_t word = ((number_train & 0x01) << 6) | ((number_way & 0x01) << 5) | (number_speed & 0x1F);
  paridad = check_paridad(word);	

  //printf("tren %d ,sentido %d, velocidad %d \n", number_train, number_way, number_speed);	

  final = ((paridad & 0x01) << 7) | word;
  //printf("0x%X \n", final);
#if 0
  puertoserie = fopen("/dev/ttyS1","rw");
  ret = fwrite(&final, 1, 1, puertoserie);
  printf("fwrite devuelve %i\n", ret);
  fclose(puertoserie);
#endif
#if 1	
  puertoserie = serial_open("/dev/ttyS1","19200","8N1",0,0);
	if (puertoserie >= 0) 
	{
		char* tren;
		char* sentido;		
		if (number_train == 0)
		{
			tren = "vapor";
		} else {
			tren = "diesel";
		} 		
		if (number_way == 0)
		{
			sentido = "hacia delante";
		} else {
			sentido = "marcha atrás";		
		}
		printf("tren %s %s, velocidad %d \n", tren, sentido, number_speed);
		//printf("Puerto serie S1 open \n");
		ret = serial_send(puertoserie, &final , 1);
		//printf("enviado, serial_send devuelve %i \n", ret);
		close(puertoserie);
	} 
	else 
	{
	printf("No se ha podido abrir el puerto serie S1 \n");
	return 1;
	}
#endif
	return 0;

}

