/* Cabecera de driver_llegada */
#include <linux/time.h> 

/* Parametros que pasan los drivers a la aplicación */
struct datos_sensores {
  char estado_sensores;
  unsigned long hora_evento_ms;        
};




