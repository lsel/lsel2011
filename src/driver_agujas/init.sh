#!/bin/sh

DRIVER=/mnt/driver_agujas.o
MAJORN=251
MINORN=0
DEVFIL=/var/agujas

insmod $DRIVER
mknod $DEVFIL c $MAJORN $MINORN
chmod 755 $DEVFIL
