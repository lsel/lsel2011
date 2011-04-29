#include "test_vias.h"

int main (int argc, char *argv[])
{
	int viaA, viaB, agujas;
	int error;
	char valor[10];

	//Apertura de ficheros----------------------------------------------------------------------
	agujas = open("/var/agujas", O_RDWR);
	if (agujas < 0){
		printf("Drv_vias: Error al abrir el archivo 'agujas'\n");
		return 1; 
	} else
		printf("Drv_vias: Abriendo el archivo 'agujas' para lectura/escritura\n");
	
	viaA = open("/var/viaA", O_RDWR);
	if (viaA < 0){
		printf("Drv_vias: Error al abrir el archivo 'viaA'\n");
		return 1; 
	} else
		printf("Drv_vias: Abriendo el archivo 'viaA' para lectura/escritura\n");
	
	viaB = open("/var/viaB", O_RDWR);
	if (viaB < 0){
		printf("Drv_vias: Error al abrir el archivo 'viaB'\n");
		return 1; 
	} else
		printf("Drv_vias: Abriendo el archivo 'viaB' para lectura/escritura\n");
	
	//Pruebas de lectura y escritura en los ficheros con valor 0.-------------------------------
	valor[0] = 0x00;
	
	//Agujas
	error = write(agujas,valor,1);
	
	if (error < 0)
		printf("Drv_vias: Fallo escribiendo un 0 en 'agujas'\n");
	else
		printf("Drv_vias: Escribiendo %d bytes en 'agujas' - Valor: %x\n", error, valor[0]);

	error = read(agujas,valor,1);
	if (error < 0)
		printf("Drv_vias: Fallo leyendo un 0 en 'agujas'\n");
	else
		printf("Drv_vias: Leídos %d bytes en 'agujas' - Valor: %x\n",error,valor[0]);	
	
	//Vía A
	error = write(viaA,valor,1);
	if (error < 0)
		printf("Drv_vias: Fallo escribiendo un 0 en 'viaA'\n");
	else
		printf("Drv_vias: Escribiendo %d bytes en 'viaA' - Valor: %x\n", error, valor[0]);

	error = read(viaA,valor,1);

	if (error < 0)
		printf("Drv_vias: Fallo leyendo un 0 en 'viaA'\n");
	else
		printf("Drv_vias: Leídos %d bytes en 'viaA' - Valor: %x\n",error,valor[0]);	

	//Vía B
	error = write(viaB,valor,1);
	if (error < 0)
		printf("Drv_vias: Fallo escribiendo un 0 en 'viaB'\n");
	else
		printf("Drv_vias: Escribiendo %d bytes en 'viaB' - Valor: %x\n", error, valor[0]);

	error = read(viaB,valor,1);

	if (error < 0)
		printf("Drv_vias: Fallo leyendo un 0 en 'viaB'\n");
	else
		printf("Drv_vias: Leídos %d bytes en 'viaB' - Valor: %x\n",error,valor[0]);

	//Pruebas de lectura y escritura en los ficheros con valor 1.-------------------------------
	valor[0] = 1;
	
	//Agujas
	error = write(agujas,valor,1);
	
	if (error < 0)
		printf("Drv_vias: Fallo escribiendo un 1 en 'agujas'\n");
	else
		printf("Drv_vias: Escribiendo %d bytes en 'agujas' - Valor: %x\n", error, valor[0]);

	error = read(agujas,valor,1);
	if (error < 0)
		printf("Drv_vias: Fallo leyendo un 1 en 'agujas'\n");
	else
		printf("Drv_vias: Leídos %d bytes en 'agujas' - Valor: %x\n",error,valor[0]);	
	
	//Vía A

	valor[0] = 1;

	error = write(viaA,valor,1);
	if (error < 0)
		printf("Drv_vias: Fallo escribiendo un 1 en 'viaA'\n");
	else
		printf("Drv_vias: Escribiendo %d bytes en 'viaA' - Valor: %x\n", error, valor[0]);

	error = read(viaA,valor,1);

	if (error < 0)
		printf("Drv_vias: Fallo leyendo un 1 en 'viaA'\n");
	else
		printf("Drv_vias: Leídos %d bytes en 'viaA' - Valor: %x\n",error,valor[0]);	

	//Vía B

	valor[0] = 1;

	error = write(viaB,valor,1);
	if (error < 0)
		printf("Drv_vias: Fallo escribiendo un 1 en 'viaB'\n");
	else
		printf("Drv_vias: Escribiendo %d bytes en 'viaB' - Valor: %x\n", error, valor[0]);

	error = read(viaB,valor,1);

	if (error < 0)
		printf("Drv_vias: Fallo leyendo un 1 en 'viaB'\n");
	else
		printf("Drv_vias: Leídos %d bytes en 'viaB' - Valor: %x\n",error,valor[0]);	
	
	//Cerrar los ficheros-----------------------------------------------------------------------
	error = close(agujas);
	if (error == 0)
		printf("Drv_vias: Archivo 'agujas' cerrado con éxito\n");
	else
		printf("Drv_vias: Error al cerrar el archivo 'agujas'\n");

	error = close(viaA);
	if (error == 0)
		printf("Drv_vias: Archivo 'viaA' cerrado con éxito\n");
	else
		printf("Drv_vias: Error al cerrar el archivo 'viaA'\n");

	error = close(viaB);
	if (error == 0)
		printf("Drv_vias: Archivo 'viaB' cerrado con éxito\n");
	else
		printf("Drv_vias: Error al cerrar el archivo 'viaB'\n");

	// Finaliza el test correctamente.----------------------------------------------------------
	return 0;
}
