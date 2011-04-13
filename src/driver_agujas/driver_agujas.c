// LSEL 2011 - Driver para el cambio de agujas.
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <asm/mcfsim.h> // Registros internos del MCF5272
#include <asm/io.h> 

//Variables globales
#define agujas_major  251 	//Número mayor
#define PORT0 0x30000000

int init_module(void);
void cleanup_module(void);
int agujas_open(struct inode *inode, struct file *filp);
int agujas_release(struct inode *inode, struct file *filep);
ssize_t agujas_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t agujas_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);

/*Funciones de acceso a fichero*/
struct file_operations agujas_fops = {
	read: agujas_read,
	write: agujas_write,
	open: agujas_open,
	release: agujas_release
};

char estado_agujas;

int init_module(void)
{
	int result;
	//Registrando dispositivo

	result = register_chrdev(agujas_major, "drv_agujas", &agujas_fops);

	if (result < 0) {
		printk("<1>Driver de agujas: fallo al registrar número major\n");
		return result;
	}

	// Escribe el estado global por defecto
	estado_agujas = 0;
	outb(0x00,PORT0);

	printk("<1>Driver de agujas: Cargando\n");

	return 0;
}

void cleanup_module(void)
{
	//Liberamos el número mayor
	unregister_chrdev(agujas_major, "drv_agujas");

	printk("<1>Driver de agujas: Desmontando\n");
}

int agujas_open(struct inode *inode, struct file *filp)
{
	// Abre el archivo
	MOD_INC_USE_COUNT;
	printk("<1>Driver de agujas: Abriendo\n");
	return 0;
}


int agujas_release(struct inode *inode, struct file *filep)
{
	// Cierra el archivo
	MOD_DEC_USE_COUNT;
	printk("<1>Driver de agujas: Cerrando\n");
	return 0;
}

ssize_t agujas_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	// Devuelve el estado global de las agujas
	// Ojo: NO se accede al puerto para lectura

	buf[0] = estado_agujas;	

	printk("<1>Driver de agujas: leyendo un %x\n",estado_agujas);

	return 1;
}


ssize_t agujas_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	// Escribe en el puerto el estado de las agujas
	outb(buf[0],PORT0);

	printk("<1>Driver de agujas: escribiendo un %x\n",buf[0]);

	// Actualiza el estado global
	estado_agujas = buf[0];
	
	return 1;
}







