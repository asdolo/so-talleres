#include <linux/fs.h>
#include <linux/cdev.h>

//Incluir module_init y module_exit
#include <linux/init.h>
//Incluir MODULE_*mod
#include <linux/module.h>

#include <linux/kernel.h>

static ssize_t device_write(struct file *file,
                            const char *buffer,
                            size_t length,
                            loff_t *offset);
static ssize_t device_read(
    struct file *file,
    char *buffer,  
    size_t length,
    loff_t *offset);


// Structs
static struct cdev ej1_cdev;

static struct file_operations ej1_file_operations = {
		.owner = THIS_MODULE,
		.read = device_read,
		.write = device_write
	};

static dev_t ej1_dev_t;


// Funciones
static ssize_t device_read(
    struct file *file,
    char *buffer, /* The buffer to fill with the data */   
    size_t length,     /* The length of the buffer */
    loff_t *offset) /* offset to the file */
{
	printk(KERN_ALERT "Hiciste read...\n");
	return 0;
}


static ssize_t device_write(struct file *file,
                            const char *buffer,
                            size_t length,
                            loff_t *offset)
{
	printk(KERN_ALERT "Hiciste write...\n");
	return length;
}

static int __init ej1_init(void)
{
	printk(KERN_ALERT "Entrando a init...\n");

	printk(KERN_ALERT "Registrando las funciones...\n");
	cdev_init(&ej1_cdev, &ej1_file_operations);
	printk(KERN_ALERT "Funciones registradas...\n");

	
	int ret_val;
	// tratamos de obtener un MINOR y MAJOR
  	ret_val = alloc_chrdev_region(&ej1_dev_t, 0, 1, "nulo");

  	if (ret_val < 0)
  	{
  		printk(KERN_ALERT "Falla en alloc_chrdev_region!\n");
  		return ret_val;
  	}

  	ret_val = cdev_add(&ej1_cdev, ej1_dev_t, 1);

  	if (ret_val < 0)
  	{
  		printk(KERN_ALERT "Falla en cdev_add!\n");
  		return ret_val;
  	}

  	printk(KERN_ALERT "MAJOR = %d\n", MAJOR(ej1_dev_t));
  	printk(KERN_ALERT "MINOR = %d\n", MINOR(ej1_dev_t));

	printk(KERN_ALERT "Saliendo de init\n");
	return 0;
}

static void __exit ej1_exit(void)
{
	cdev_del(&ej1_cdev);

	unregister_chrdev_region(ej1_dev_t, 1);
}



module_init(ej1_init);
module_exit(ej1_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Nuestro primer modulo");