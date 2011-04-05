





#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/mcfsim.h> /* Registros internos del MCF5272 */
#include <asm/io.h>
#include <linux/sched.h>
#include <linux/types.h>
//#include <linux/poll.h>



#define MCF_BAR 0x10000000
#define IO_N_MAJOR 250
#define IO_N_MINOR 0
#define NUMERO_SENSORES 8 
#define VECTOR_INT 65


/* Declaración de funciones */
unsigned long gettimemillis(void);
int sensores_open(struct inode *inode, struct file *file); 
int sensores_release(struct inode *inode, struct file *file); 
//static unsigned int sensores_poll(struct file *file, struct poll_table_struct *wait);
ssize_t sensores_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);  
void manejador_external(int id, void *p, struct pt_regs *regs); 

/* Para manejar el cerrojo */
struct sensores_data {
  struct sensores_data	        *ops;
  spinlock_t	        	lock;
  char		        	ready;
  wait_queue_head_t     	wait;
};

/* Variables globales */
//static unsigned int    nuevos_datos_sensores = 0;

/*Funciones de acceso a fichero*/
struct file_operations sensores_fops = {
 read: sensores_read,
 open: sensores_open,
 //poll: sensores_poll,
 release: sensores_release 
};


