#################################################################
#								#
#			Makefile				#
#								#
# Para compilar programas para la plataforma del Coldfire de    #
# Motorola, en la placa de desarrollo M5272C3			#
#								#
#################################################################
 

# Directorio donde está instalada la distribución uClinux
UCLINUXDIR = /home/josem/uClinux-dist-LSI
ANTARES_TOOLCHAIN = /opt/old-gnu-toolchain

# Compilador cruzado para el Motorola CF
CC = m68k-elf-gcc -B$(ANTARES_TOOLCHAIN)/lib/gcc-lib/


# Opciones para el compilador a la hora de generar los ficheros objeto
# 
# -fno-builtin
#    Don't  recognize built-in functions that do not begin with two leading underscores.  
#    Currently, the functions affected include _exit, abort, abs, alloca, cos, exit, fabs, 
#    labs, memcmp, memcpy, sin, sqrt, strcmp, strcpy, and strlen.
#
# -nostdinc
#    Do  not  search the standard system directories for header files. 
#    Only the directories you have specified with `-I' options (and the current directory, 
#    if appropriate) are searched.
#
# -Wall
# -Wstrict-prototypes
# -Wno-trigraphs
#    Warnings
#    
# -O2 
#   Optimize code. 
#
# -fno-strict-aliasing
# -pipe
#
#
#   
# -fno-common
#   Allocate even uninitialized global variables in the bss section of the object file, 
#   rather than generating them as common blocks. This  has the effect that if the 
#   same variable is declared (without extern) in two different compilations, 
#   you will get an error when you link them. The only reason this might be useful is if 
#   you wish to verify that the program will work on other systems which always work this way.
#
# -m5307
#   Generate code for the m5307 core.
#
# -Wa,...
#   Passes flags to the assembler
#
#
#
#
CFLAGS = -fno-builtin -nostdinc -I$(UCLINUXDIR)/linux-2.4.x/include -I$(ANTARES_TOOLCHAIN)/lib/gcc-lib/m68k-elf/2.95.3/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -pipe -m5307 -Wa,-S -Wa,-m5307 -D__KERNEL__ -D__linux__ -DMODULE -DEXPORT_SYMTAB -DNO_MM -DNO_FPU -DCONFIG_M5272 -DCONFIG_COLDFIRE

# Diferentes targets del desarrollo

all: driver_sensores_si.o

clean:
	rm -f *.o *.gdb


