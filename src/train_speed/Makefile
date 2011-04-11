#################################################################
#								#
#			   Makefile				#
#								#
# Para compilar programas para la plataforma del Coldfire de    #
# Motorola, en la placa de desarrollo M5272C3			#
#								#
#################################################################


# Directorio donde está instalada la distribución uClinux
UCLINUXDIR = /opt/uClinux-dist-LSI/
TOOLCHAINDIR = /opt/old-gnu-toolchain
INCLUDEDIR = ../include


# Compilador cruzado para el Motorola CF
CC = $(TOOLCHAINDIR)/bin/m68k-elf-gcc -B/opt/old-gnu-toolchain/lib/gcc-lib/


# Opciones para el compilador a la hora de generar los ficheros objeto
CFLAGS = -m5307 -msep-data -Os -fomit-frame-pointer -I$(UCLINUXDIR)/lib/libc/include -I$(UCLINUXDIR)/linux-2.4.x/include -I$(TOOLCHAINDIR)/m68k-elf/include -I$(TOOLCHAINDIR)/lib/gcc-lib/m68k-elf/2.95.3/include -fno-builtin -I$(INCLUDEDIR)


# Opciones para el compilador a la hora de generar los ejecutales
LDFLAGS = -m5307 -Os -fomit-frame-pointer -fno-builtin -msep-data -Wl,-elf2flt 


# Librerias a incluir al generar el ejecutable
LDLIBS = -L$(UCLINUXDIR)/lib/libc -lc


all: train-speed

train-speed: train-speed.o funciones_sms.o

clean:
	rm -rf *.o *.gdb $(EXEC)







