#include "test_agujas.h"

int main (int argc, char *argv[])
{
	int driver;

	int error;
	char valor[10];

	driver = open("/var/agujas", O_RDWR);

	if (driver < 0){
		printf("Agujas: Error al abrir el archivo\n");
		return 1; }
	else
		printf("Agujas: Abriendo el archivo para lectura y escritura\n");

	valor[0] = 0xAA;

	error = write(driver,valor,1);
	
	if (error < 0)
		printf("Agujas: Fallo escribiendo\n");
	else
		printf("Agujas: Escribiendo %d bytes - Valor: %x\n", error, valor[0]);

	error = read(driver,valor,1);

	if (error < 0)
		printf("Agujas: Fallo leyendo\n");
	else
		printf("Agujas: Leídos %d bytes - Valor: %x\n",error,valor[0]);	


	error = close(driver);

	if (error == 0)
		printf("Agujas: Archivo cerrado con éxito\n");
	else
		printf("Agujas: Error al cerrar el archivo\n");

	return 0;
}
