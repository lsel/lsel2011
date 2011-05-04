#!/bin/sh

## INICIALIZACION DRIVERS
## HBG 2011-03-22

DRIVER=/mnt/src/driver_sensores/driver_sensores.o
MAJORN=250
MINORN=0
DEVFIL=/var/sensores

insmod $DRIVER
mknod $DEVFIL c $MAJORN $MINORN
chmod 755 $DEVFIL
