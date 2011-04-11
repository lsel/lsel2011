#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <assert.h>
#include <string.h>
#include "funciones_sms.h"
//#include "config.h"




#ifndef NDEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif


/* ************************************************************************************************
 *  Test para comprobar el correcto funcionamiento de la configuracion del sistema
 *  a partir de mensajes SMS.
 *  Pasos que realiza el test:
 *  1) Se envia un mensaje con el texto "TEMPERATURA_A 10" a si mismo
 *  2) Se lee el mensaje del modem
 *  3) Se comprueba el parametro enviado y se escribe el valor en el fichero correspondiente
 *  Version: 1.0.0 alfa
 *  Fecha de Creación: 24/04/2008
 *  Autores: Daniel Gallego, Miguel Ángel Comino
 **************************************************************************************************/

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS1"
#define NUM_TELEFONO "679623600"
#define BUFFER_SIZE 100  //Tamaño de los buffer de lectura
#define NUM_ATRIBUTOS 12 //Numero de atributos que se pueden configurar en el sistema
#define BUFSIZE 2048
	


/***************************************************************************************
 *  Metodo para escribir en el puerto serie de forma no bloqueante
 ***************************************************************************************/

int serial_send (int fd, const char* msg, int len)
{
  DEBUG(puts (msg); fflush(stdout));
  return write (fd, msg, len);
}

/***************************************************************************************
 * Metodo para leer del puerto serie de forma no bloqueante
 **************************************************************************************/

int serial_recv (int fd, char* buf, int max_bytes, int max_ms)
{
  int bytes = 0;
  while (1) {
    struct timeval tv = { max_ms / 1000, (max_ms % 1000) * 1000};
    int res;
    fd_set rdset;
    FD_ZERO (&rdset);
    FD_SET (fd, &rdset);
    if (select (fd + 1, &rdset, NULL, NULL, &tv) != 1)
      break;
    res = read (fd, buf + bytes, max_bytes - bytes);
    if (res <= 0) break;
    bytes += res;
  }
  buf[bytes] = '\0';
  DEBUG(puts(buf));
  return bytes;
}

/* ***************************************************************************************************
 * Metodo encargado de inicializar la comunicacion con el modem GSM
 ****************************************************************************************************/


void inicializaMODEM(int fd,int timeout)
{
  	
	serial_send (fd,"at\r", timeout);
  	sleep(1);
  	serial_send (fd,"ate0\r", timeout);
  	sleep(1);
  	serial_send (fd,"at+cmgf=1 \r", timeout);
  	sleep(1);
}

/* *********************************************************************************************
 *  Metodo encargado de enviar un mensaje SMS al numero de telefono de la tarjeta SIM del modem
 ************************************************************************************************/

int enviarSMS (int fd, const char * mensaje) 
{
  unsigned char CtrZ = 0x1a;  
  int time_out = 1000;       
  //char cmd[128];
  char buf[BUFFER_SIZE] = "\n";
  //sprintf (cmd, "at+cmgs=\"679623600\"\r\n");
  serial_send (fd, "at+cmgs=\"679623600\"\r", time_out);
  DEBUG (printf("Se le ha enviado el numero\n")); 
	

	
  serial_send(fd, mensaje, time_out); //Enviamos el texto del mensaje
  DEBUG (printf("Se le ha enviado el mensaje\n")); 
	

  serial_send(fd,&CtrZ,time_out); //Escribirmos fin del mensaje
 	
  
	
  DEBUG (printf("Se le ha enviado el Crtl+Z\n")); 
	
  serial_recv(fd,buf,10,time_out);
  
  DEBUG (printf("El modem contesta %s, se ha enviado el mensaje\n",buf)); 
	
  close(fd);
  return 1;   
              
}




/*********************************************************************************************
 * Metodo de inicializacion del puerto serie del Antares
 *********************************************************************************************/

int serial_open (const char* dev, 
		 const char* speed_str, 
		 const char* mode_str, int hwf, int swf)
{
  int fd = 0;

  int bit = mode_str[0];
  int par = mode_str[1];
  int stopb = mode_str[2];
  speed_t speed;

  struct termios tty;
  tcgetattr(fd, &tty);

  fd = open (dev, O_RDWR | O_NOCTTY | O_NONBLOCK);

  if (fd < 0) {
    return -1;
  }
  
  switch (atoi(speed_str)) {
  case 300:	speed = B300;	 break;
  case 600:	speed = B600;	 break;
  case 1200:	speed = B1200;	 break;
  case 2400:	speed = B2400;	 break;
  case 4800:	speed = B4800;	 break;
  case 9600:	speed = B9600;	 break;
  case 19200:	speed = B19200;	 break;
  case 38400:	speed = B38400;	 break;
  case 57600:	speed = B57600;	 break;
  case 115200:	speed = B115200; break;
  default:	speed = B0;	 break;
  }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  switch (bit) {
  case '5':
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
    break;
  case '6':
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
    break;
  case '7':
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
    break;
  case '8':
  default:
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    break;
  }
  /* Set into raw, no echo mode */
  tty.c_iflag =  IGNBRK;
  tty.c_lflag = 0;
  tty.c_oflag = 0;
  tty.c_cflag |= CLOCAL | CREAD;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 5;

  if (swf)
    tty.c_iflag |= IXON | IXOFF;
  else
    tty.c_iflag &= ~(IXON|IXOFF|IXANY);

  tty.c_cflag &= ~(PARENB | PARODD);
  if (par == 'E')
    tty.c_cflag |= PARENB;
  else if (par == 'O')
    tty.c_cflag |= (PARENB | PARODD);

  if (stopb == '2')
    tty.c_cflag |= CSTOPB;
  else
    tty.c_cflag &= ~CSTOPB;

  if (hwf)
    tty.c_cflag |= CRTSCTS;
  else
    tty.c_cflag &= ~CRTSCTS;

  tcsetattr(fd, TCSANOW, &tty);
  return fd;
}















