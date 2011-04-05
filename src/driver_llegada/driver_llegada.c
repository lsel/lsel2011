/* Driver sensores */


/* #ifndef __KERNEL__ */
/* #define __KERNEL__ */
/* #endif */

/* #ifndef MODULE */
/* #define MODULE */
/* #endif */
#include "driver_llegada.h"
#include "sensores.h"

// TO DO:
// llegada_open: que pasa con las interrupciones, conseguimos q no sde bloque, pero no esta probado aun
// gettimemillis: estaria bien definir y programar esta funcion en una libreria aparte


  

struct datos_sensores sensores;
/* Rutina de servicio de interrupción */
/* Más info: Introd. a los sistemas digitales con */
/* el microcontrolador MCF5272 pag. 54 y siguientes.*/
void 
manejador_external(int id, void *p, struct pt_regs *regs)
{
    unsigned int reg32_aux;
  /* Cargamos el momento en que se ha producido la interrupción, con el reloj del kernel */

  reg32_aux=inl(MCF_BAR + MCFSIM_ICR1); 
  outl(reg32_aux|0xF0000000,(MCF_BAR + MCFSIM_ICR1)); // ICR1: pag. 178
  //Rellenamos el Struct
  sensores.estado_sensores=inb(MCF_BAR + MCFSIM_PADAT); // se meten los datos en esa registro de datos
  sensores.hora_evento_ms = gettimemillis();
  nuevos_datos_sensores = 1; 
  //Trazas
  printk(KERN_INFO "driver_llegada: Interrupcion\n");
  printk(KERN_INFO "driver_llegada: (INT) Valores de los sensores (1): 0x%X.\n", sensores.estado_sensores&0xFF);
  printk(KERN_INFO "driver_llegada: (INT) Valores de los sensores (2): 0x%X.\n", sensores.estado_sensores&0xFF);
  
}

/* Permite que la aplicación lea si ha habido cambios */
/* http://tali.admingilde.org/dhwk/vorlesung/ar01s08.html */
/* Linux Device Drivers p. 163 */
static unsigned int 
llegada_poll(struct file *file, struct poll_table_struct *wait)
{
  struct llegada_data *llegada = (struct llegada_data *)file->private_data;

  poll_wait(file, &llegada->wait, wait);
  if (llegada->ready)
    return POLLIN | POLLRDNORM;

  return 0;
}


/* Abriendo el dispositivo como fichero*/
int llegada_open(struct inode *inode, struct file *file) 
{
  unsigned int reg32_aux;
  struct llegada_data datos_fichero;
  printk("driver_llegada: Puerto abierto\n");
  
  // Puertos de entrada (p406 manual)
  outw(0x0000,(MCF_BAR + MCFSIM_PADDR)); 
  
  MOD_INC_USE_COUNT;
  file->private_data = (void *) &datos_fichero;
  sensores.estado_sensores=0;
  printk(KERN_INFO "driver_llegada: (open) lectura de sensores.estado_sensores=0x%x\n",sensores.estado_sensores&0xFF);
  return 0; 
}

/* Cerrando el dispositivo como fichero*/
int llegada_release(struct inode *inode, struct file *file)
{
  
  MOD_DEC_USE_COUNT;
  printk(KERN_INFO "driver_llegada: Puerto cerrado\n");
  return 0;

}

/* Leer datos de los sensores que entran en el puerto */
ssize_t llegada_read(struct file *filep, char *buf, size_t count, loff_t *f_pos) 
{
  
  struct llegada_data *llegada = (struct llegada_data *)filep->private_data;
  DECLARE_WAITQUEUE(wait, current);
  if (count < NUMERO_SENSORES)
    return -EINVAL;
//cerrojo, para que no se bloque en la lectura
  spin_lock_irq(&llegada->lock);

  if (!llegada->ready) {
#ifdef BROKEN_LLEGADA
    spin_unlock_irq(&llegada->lock);
    return -EAGAIN;
#else
    if (filep->f_flags & O_NONBLOCK) {
      spin_unlock_irq(&llegada->lock);
      return -EAGAIN;
    }

    add_wait_queue(&llegada->wait, &wait);
  repeat:
    set_current_state(TASK_INTERRUPTIBLE);
    if (!llegada->ready && !signal_pending(current)) {
      spin_unlock_irq(&llegada->lock);
      schedule();
      spin_lock_irq(&llegada->lock);
      goto repeat;
    }

    current->state = TASK_RUNNING;
    remove_wait_queue(&llegada->wait, &wait);

    if (signal_pending(current)) {
      spin_unlock_irq(&llegada->lock);
      return -ERESTARTSYS;
    }
#endif
  }

  // ESTO NO DEBE ESTAR AQUI; SOLO COMO PRUEBA
  //sensores.estado_sensores=inb(MCF_BAR + MCFSIM_PADAT); 
  ////////////////////////////////////////////

  //Trazas
  printk(KERN_INFO "driver_llegada: (read) lectura de sensores.estado_sensores=0x%x\n",sensores.estado_sensores&0xFF);
  printk("%lu milliseconds\n",gettimemillis());

 //Instrucción para pasar el struct por el buf
 memcpy(buf,(const char*) &sensores, sizeof(sensores));
 
 return NUMERO_SENSORES;
}

/*Inicio de módulo*/
int init_llegada(void)
{
  int result;
  result = register_chrdev(IO_N_MAJOR, "driver_llegada", &llegada_fops);
  /* Configuracion de interrupciones y puertos */ // Ver pag. 182 del manual
  reg32_aux=inl(MCF_BAR + MCFSIM_ICR1); 
  outl(reg32_aux|0xF0000000,(MCF_BAR + MCFSIM_ICR1)); // ICR1: pag. 178
  reg32_aux=inl(MCF_BAR + MCFSIM_PITR);
  outl(reg32_aux|0x80000000,(MCF_BAR + MCFSIM_PITR)); // PITR: interrupcción a flanco de subida.
  
  //Petición de la interrupción
  request_irq(VECTOR_INT, manejador_external,SA_INTERRUPT, "INT1", NULL);
  
  if (result < 0){
    printk("driver_llegada: <1>Fallo número mayor\n");
    return result;
  }
  printk("driver_llegada: Iniciado\n");
  return 0;
}


void cleanup_llegada(void) {

  if (unregister_chrdev(IO_N_MAJOR, "driver_llegada") < 0)
    {
      printk("driver_llegada: Error unregistering module\n");
      return;
    }
    unsigned int reg32_aux;
    reg32_aux=inl(MCF_BAR + MCFSIM_ICR1); 
    outl(reg32_aux&0x0FFFFFFF,(MCF_BAR + MCFSIM_ICR1)); // ICR1: pag. 178
    reg32_aux=inb(MCF_BAR + MCFSIM_PITR);
    outl(reg32_aux&0x7FFFFFFF,(MCF_BAR + MCFSIM_PITR)); // PITR: interrupcción a flanco de subida.
    free_irq(VECTOR_INT, NULL);
  printk(KERN_INFO "driver_llegada: Finalizado\n");
  return;
}

module_init(init_llegada);
module_exit(cleanup_llegada);
