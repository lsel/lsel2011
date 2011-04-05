/* Cabecera de driver_llegada */
#include <linux/time.h> 

/* Parametros que pasan los drivers a la aplicación */
struct datos_sensores {
  char estado_sensores;
  unsigned long hora_evento_ms;        
};


/*Para conocer el momento en que se ejecuta la interrupción */
/* Devuelve la hora del sistema en milisegundos */
unsigned long gettimemillis(void){
  
  struct timeval tv;
  
  do_gettimeofday(&tv);
  return (unsigned long) 1000*tv.tv_sec + (unsigned long) tv.tv_usec/1000;
}

