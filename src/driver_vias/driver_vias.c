// LSEL 2011 - Driver para el cambio de agujas y el control de los semáforos.
// Uso del driver:
// Se cargarán tres ficheros con un minor number correspondiente al bit que se va a modificar
// En EL PRIMER BYTE de dicho fichero se escribe el valor con que se quiere poner el bit

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <asm/mcfsim.h> 	// Registros internos del MCF5272
#include <asm/io.h> 

//Constantes
#define vias_major  251 	//Número mayor
#define PORT0 0x30000000	

// Variables globales
int init_module(void);
void cleanup_module(void);
int vias_open(struct inode *inode, struct file *filp);
int vias_release(struct inode *inode, struct file *filep);
char estado_vias;

// Prototipos de funciones
ssize_t vias_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t vias_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);

//Funciones de acceso a fichero
struct file_operations vias_fops = {
	read: vias_read,
	write: vias_write,
	open: vias_open,
	release: vias_release
};


int init_module(void)
{
	int result;
	//Registrando dispositivo

	result = register_chrdev(vias_major, "drv_vias", &vias_fops);

	if (result < 0) {
		printk("<1>\nDrv. vias: fallo al registrar número major\n");
		return result;
	}

	// Escribe el estado global por defecto	
	estado_vias = 0x00;		//Posición de las vías por defecto y semáforos en rojo.
	//outb(estado_vias,PORT0);
	outb(0x00,PORT0);		

	printk("<1>Drv. vias: Cargando\n");

	return 0;
}

void cleanup_module(void)
{
	//Liberamos el número mayor
	unregister_chrdev(vias_major, "drv_vias");

	printk("<1>Drv. vias: Desmontando\n");
}

int vias_open(struct inode *inode, struct file *filp)
{
	// Abre el archivo
	MOD_INC_USE_COUNT;
	printk("<1>Drv. vias: Abriendo\n");
	return 0;
}


int vias_release(struct inode *inode, struct file *filep)
{
	// Cierra el archivo
	MOD_DEC_USE_COUNT;
	printk("<1>Drv. vias: Cerrando\n");
	return 0;
}

ssize_t vias_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	// Devuelve el estado del bit consultado mediante el minor number

	int minor;
	char valor;
	char mask;

	//Extraemos el minor number para escoger la máscara adecuada.
	minor = MINOR(filep->f_dentry->d_inode->i_rdev);

	if (minor == 0){		//Minor asignado al cambio de agujas.
		mask = 0xFE;
	} else if (minor == 1) {	//Minor asignado al semáforo de la vía A.
		mask = 0xFD;		
	} else if (minor == 2) {	//Minor asignado al semáforo de la vía B.
		mask = 0xFB;
	} else {	
		printk("<1>Drv. vias: llamada a read con minor incorrecto\n");
		return -1;
	}

	valor = estado_vias & ~mask;	// Halla el valor del bit indicado

	if (valor == 0)			// Devuelve el valor
		buf[0] = 0;
	else
		buf[0] = 1;

	printk("<1>Drv. vias: leyendo un %x\n",buf[0]);

	return 1;
}


ssize_t vias_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	// Actualiza el estado del bit correspondiente al fichero indicado

	int minor;
	char valor;
	char mask;

	//Extraemos el minor number para escoger la máscara adecuada.
	minor = MINOR(filep->f_dentry->d_inode->i_rdev); 

	if (minor == 0){		//Minor asignado al cambio de agujas.
		mask = 0xFE;
	} else if (minor == 1) {	//Minor asignado al semáforo de la vía A.
		mask = 0xFD;		
	} else if (minor == 2) {	//Minor asignado al semáforo de la vía B.
		mask = 0xFB;
	} else {			
		printk("<1>Drv. vias: llamada a write con minor incorrecto\n");
		return -1;
	}

	if (buf[0] == 0) {	//Escribir 0: para poner en rojo semáforos o mover vías hacía una posición.
		valor = estado_vias & mask;				//El bit no enmascarado se fuerza a 0.
	}
	else if (buf[0] == 1) {	//Escribir 1: para poner en verde semáforos o mover vías a la otra posición.
		valor = estado_vias | ~mask;	//El bit no enmascarado se fuerza a 1.
	}
	else {
		printk("<1>Drv. vias: error en valor de escritura\n");
		return -1;
	}

	// Escribe en el puerto el estado de las agujas
	outb(valor,PORT0);

	printk("<1>Drv. vias: escribiendo un %x\n",valor);

	// Actualiza el estado global
	estado_vias = valor;
	
	return 1;
}







