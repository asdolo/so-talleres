#include <linux/fs.h>
#include <linux/cdev.h>

//Incluir module_init y module_exit
#include <linux/init.h>
//Incluir MODULE_*mod
#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/sched.h>

// header
static int device_open(struct inode *node, struct file *file);
static int device_release(struct inode *node, struct file *file);
static ssize_t device_write(struct file *file, const char *buffer, size_t length, loff_t *offset);
static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset);

// structs

static struct cdev device_cdev;
static struct file_operations device_operations = {
		.owner = THIS_MODULE,
		.open = device_open, // atiendo la función open, como dice el primer puntito del ejercicio
    .release = device_release, // atiendo la función release, como dice el segundo puntito del ejercicio
    .write = device_write, // atiendo la función write, como dice el tercer puntito del ejercicio
    .read = device_read // atiendo la función read, como dice el cuarto puntito del ejercicio

	};

struct device_process_info_t
{
  pid_t process;
  unsigned int free;
  unsigned int written;
  char data;
 };

static struct device_process_info_t processes_info[3];

static dev_t device_dev_t;


// implementaciones

static int device_open(struct inode *node, struct file *file)
{
  printk(KERN_ALERT "(letras123) Hiciste open...\n");

  int i;
  for(i = 0; i < 3; i++)
  {
    if (processes_info[i].free == 1)
    {
      processes_info[i].free = 0;
      processes_info[i].process = current->pid;
      processes_info[i].written = 0;

      printk(KERN_ALERT "(letras123) Asignado el espacio %d al pid %d...\n", i, current->pid);
      return 0;
    }
  }
  
  printk(KERN_ALERT "(letras123) Error: no hay espacio libre para hacer open...\n", current->pid);
  return -EPERM;
}

static int device_release(struct inode *node, struct file *file)
{
  printk(KERN_ALERT "(letras123) Hiciste release...\n");

  int i;
  for(i = 0; i < 3; i++)
  {
    if (processes_info[i].process == current->pid)
    {
      processes_info[i].free = 1;
      processes_info[i].written = 0;

      printk(KERN_ALERT "(letras123) pid %d liberado...\n", current->pid);
      return 0;
    }
  }

  printk(KERN_ALERT "(letras123) Error: el pid %d no habia hecho open...\n", current->pid);
  return -EPERM;
}

static ssize_t device_write(struct file *file, const char *buffer, size_t length, loff_t *offset)
{
  printk(KERN_ALERT "(letras123) Hiciste write...\n");

  int i;
  for(i = 0; i < 3; i++)
  {
    if (processes_info[i].process == current->pid)
    {
      if (processes_info[i].written == 0)
      {
        // primera vez que escribo
        processes_info[i].written = 1;
        processes_info[i].data = (*buffer); // !?

        return length;
      }

      printk(KERN_ALERT "(letras123) Warning: el pid %d ya habia escrito antes. Ignorando...\n", current->pid);
      // no es la primera vez que el proceso escribe, ignoro el write
      return length;
    }
  }

  printk(KERN_ALERT "(letras123) Error: el pid %d no habia hecho open...\n", current->pid);
  return length;
}

static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
  printk(KERN_ALERT "(letras123) Hiciste read...\n");

  int i;
  for(i = 0; i < 3; i++)
  {
    if (processes_info[i].process == current->pid)
    {
      if (processes_info[i].written == 1)
      {
        // hay algo escrito
        int j;
        for(j = 0; j < length; j++)
        {
          buffer[j] = processes_info[i].data;
        }
        return length;
      }

      printk(KERN_ALERT "(letras123) Error: el pid %d no habia escrito nada!!\n", current->pid);
      return -EPERM;
    }
  }

  printk(KERN_ALERT "(letras123) Error: el pid %d no habia hecho open...\n", current->pid);
  return -EPERM;
}

static int __init device_init(void)
{
	printk(KERN_ALERT "(letras123) Entrando a device_init...\n");

  printk(KERN_ALERT "(letras123) Registrando las funciones...\n");
  cdev_init(&device_cdev, &device_operations);
  printk(KERN_ALERT "(letras123) Funciones registradas...\n");

	int ret_val;
  // tratamos de obtener un MINOR y MAJOR
  ret_val = alloc_chrdev_region(&device_dev_t, 0, 1, "letras123");

  if (ret_val < 0)
  {
    printk(KERN_ALERT "(letras123) Falla en alloc_chrdev_region!\n");
    return ret_val;
  }

  ret_val = cdev_add(&device_cdev, device_dev_t, 1);

  if (ret_val < 0)
  {
    printk(KERN_ALERT "(letras123) falla en cdev_add!\n");
    return ret_val;
  }

  printk(KERN_ALERT "(letras123) MAJOR = %d\n", MAJOR(device_dev_t));
  printk(KERN_ALERT "(letras123) MINOR = %d\n", MINOR(device_dev_t));

  // inicializo el flag de libre en 1
  processes_info[0].free = 1;
  processes_info[1].free = 1;
  processes_info[2].free = 1;

  printk(KERN_ALERT "(letras123) Saliendo de device_init\n");
  return 0;
}

static void __exit device_exit(void)
{
	printk(KERN_ALERT "(letras123) Entrando a device_exit...\n");

  printk(KERN_ALERT "(letras123) Saliendo de device_exit\n");
}


module_init(device_init);
module_exit(device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Nuestro primer modulo");