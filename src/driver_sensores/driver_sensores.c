/* Driver sensores */
 

/* #ifndef __KERNEL__ */
/* #define __KERNEL__ */
/* #endif */

/* #ifndef MODULE */
/* #define MODULE */
/* #endif */
#include "driver_sensores.h"
#include "sensores.h"

// TO DO:
// sensores_open: que pasa con las interrupciones, conseguimos q no sde bloque, pero no esta probado aun
// gettimemillis: estaria bien definir y programar esta funcion en una libreria aparte


  

struct datos_sensores sensores;



/* Permite que la aplicación lea si ha habido cambios */
/* http://tali.admingilde.org/dhwk/vorlesung///Rellenamos el Struct
   sensores.estado_sensores=inb(MCF_BAR + MCFSIM_PADAT); // se meten los datos en esa registro de datos
   sensores.hora_evento_ms = gettimemillis();
   nuevos_datos_sensores = 1; ar01s08.html */
/* Linux Device Drivers p. 163 */


/* Abriendo el dispositivo como fichero*/
int sensores_open(struct inode *inode, struct file *file) 
{
  printk("driver_sensores: Driver abierto\n");
  MOD_INC_USE_COUNT;
  return 0; 
}

/* Cerrando el dispositivo como fichero*/
int sensores_release(struct inode *inode, struct file *file)
{
  
  MOD_DEC_USE_COUNT;
  printk(KERN_INFO "driver_sensores: Driver cerrado\n");
  return 0;

}

/* Leer datos de los sensores que entran en el puerto */
ssize_t sensores_read(struct file *filep, char *buf, size_t count, loff_t *f_pos) 
{
  u_int16_t PA;
  u_int8_t PA8;
  //Rellenamos el Struct
  PA = inw(MCF_BAR + MCFSIM_PADAT);


  PA8 = ((PA>>8)&0x01) << 7;	
  sensores.estado_sensores = (PA & 0x7F) + PA8;
	

  sensores.hora_evento_ms = gettimemillis();
  if (count < sizeof(sensores))
    return -EINVAL;
  memcpy(buf,(const char*) &sensores, sizeof(sensores));
  
  //Trazas
  printk(KERN_INFO "driver_sensores: (read) lectura de sensores.estado_sensores=0x%02X\n",sensores.estado_sensores&0xFF);
  printk("driver_sensores: (read) lectura de sensores.hora=%lu milliseconds\n",gettimemillis());

 
  return sizeof(sensores);
}

/*Inicio de módulo*/
int init_sensores(void)
{
  int result;
  u_int16_t tmp;

  printk("driver_sensores: Iniciando\n");
  
  tmp = inw(MCF_BAR + MCFSIM_PACNT);
  printk(KERN_INFO "driver_sensores: (init) PACNT =0x%04x\n",tmp);

  // Puertos de entrada (p406 manual)
  outw(0x0000,(MCF_BAR + MCFSIM_PADDR)); 

  tmp = inw(MCF_BAR + MCFSIM_PADDR);
  printk(KERN_INFO "driver_sensores: (init) PADDR =0x%04x\n",tmp);

  tmp = inw(MCF_BAR + MCFSIM_PADAT);
  printk(KERN_INFO "driver_sensores: (init) PADAT =0x%04x\n",tmp);

	
  result = register_chrdev(IO_N_MAJOR, "driver_sensores", &sensores_fops);
  if (result < 0){
    printk("driver_sensores: <1>Fallo número mayor\n");
    return result;
  }
  
  printk("driver_sensores: Iniciado con exito.\n");
  
  return 0;
}


void cleanup_sensores(void) {

  if (unregister_chrdev(IO_N_MAJOR, "driver_sensores") < 0)
    {
      printk("driver_sensores: Error unregistering module\n");
      return;
    }
    
  printk(KERN_INFO "driver_sensores: Finalizado\n");
  return;
}

module_init(init_sensores);
module_exit(cleanup_sensores);
