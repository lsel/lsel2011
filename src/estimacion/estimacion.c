#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include "sensores.h"



#define DIST 1
#define DIST_S1 2.35
#define DIST_S2 1.35

struct datos_sensores *datos;

			
int main (int argc, char** argv) 
{
  const char* devname = "/var/sensores";
  char buf [2];
  int fd;

  if (argc > 1)
    devname = argv[1];
  fd = open (devname, O_RDONLY);
  if (fd < 0) { 
    perror (devname);
    exit (1);
  }
  while (1) {
    while (1) {
      int ret;
      fd_set rdset;
      FD_ZERO (&rdset);
      FD_SET (fd, &rdset);

      ret = select (fd + 1, &rdset, NULL, NULL, NULL);
      if (ret <= 0) {
			break;
			}
      if (FD_ISSET (fd, &rdset)) {
	int cnt = read (fd, buf, 2);
	if (cnt == 0) {
	  exit (0);
  	}
  	if (cnt != 2) { 
	  perror ("read");
	  exit (2);
	}
	datos = (struct datos_sensores *) buf; 
        printf ("%d ", (int) datos->estado_sensores);
      }
    }
  }
  close (fd);
  return 0;
}
