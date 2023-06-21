#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

/* Informacion */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peaky Coders");
MODULE_DESCRIPTION("Character device driver que mide dos señales externas");

/* Variables para el dispositivo y la clase del dispositivo */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_gpio_driver"
#define DRIVER_CLASS "MyModuleClass"

/**
 * @brief Lee los datos del buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	char tmp[22];
	int value,adjusted_value;

	/* Obtiene la cantidad de datos a copiar */
	to_copy = min(count, sizeof(tmp)-1);

	/* Lee el valor del boton */
	if(gpio_get_value(4)){
		printk("Valor del boton: %d\n", gpio_get_value(22));
		value = gpio_get_value(22);
		adjusted_value = value ? 1 : 0;  // Ajustar el valor a 0 o 1

		snprintf(tmp, sizeof(tmp), "Sensor con valor: %d\n",adjusted_value );
		not_copied = copy_to_user(user_buffer, &tmp, to_copy);
	}
	else{
		printk("Valor del boton: %d\n", gpio_get_value(17));
		value = gpio_get_value(17);
		adjusted_value = value ? 1 : 0;  // Ajustar el valor a 0 o 1
		
		snprintf(tmp, sizeof(tmp), "Boton con valor: %d \n",adjusted_value );
		not_copied = copy_to_user(user_buffer, &tmp, to_copy);
	}

	/* Calcular datos */
	delta = to_copy - not_copied;

	msleep(500);

	return delta;
}

/**
 * @brief Escribe los datos al buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	char value;

	/* Obtiene la cantidad de datos a copiar */
	to_copy = min(count, sizeof(value));

	/* Copia los datos al user */
	not_copied = copy_from_user(&value, user_buffer, to_copy);

	/* Establece el LED */
	switch(value) {
		case '0':
			gpio_set_value(4, 0);
			break;
		case '1':
			gpio_set_value(4, 1);
			break;
		default:
			printk("Entrada invalida!\n");
			break;
	}

	/* Calcular datos */
	delta = to_copy - not_copied;

	return delta;
}

/**
 * @brief Llama a la funcion cuando el device file es abierto
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open fue llamda!\n");
	return 0;
}

/**
 * @brief Llama a la funcion cuando el device file es cerrado
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - close fue llamada!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

/**
 * @brief Llama a la funcion cuando se carga el modulo en el kernel
 */
static int __init ModuleInit(void) {
	printk("Hola Kernel!\n");

	/* Aloca un numero de device */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device nr. no pudo ser alocado\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d fue registrado!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Crea la device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class no pudo ser creada\n");
		goto ClassError;
	}

	/* Crea el device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Device file no pudo ser creado\n");
		goto FileError;
	}

	/* Inicializa el device file */
	cdev_init(&my_device, &fops);

	/* Se registra el device en el kernel */
	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Fallo el registro del device en el kernel\n");
		goto AddError;
	}

	/* GPIO 4 init */
	if(gpio_request(4, "rpi-gpio-4")) {
		printk("No se puede alocar GPIO 4\n");
		goto AddError;
	}

	/* Direccion del GPIO */
	if(gpio_direction_output(4, 0)) {
		printk("No se puede setear GPIO 4 como salida\n");
		goto Gpio4Error;
	}

	/* GPIO 17 init */
	if(gpio_request(17, "rpi-gpio-17")) {
		printk("No se puede alocar GPI 17\n");
		goto Gpio17Error;
	}

	/* Direccion del GPIO */
	if(gpio_direction_input(17)) {
		printk("No se puede setear GPIO 17 como entrada!\n");
		goto Gpio17Error;
	}
    	/* PIO 22 init */
	if(gpio_request(22, "rpi-gpio-22")) {
		printk("No se puede alocar GPIO 22\n");
		goto Gpio22Error;
	}

	/* Direccion del GPIO */
	if(gpio_direction_input(22)) {
		printk("No se puede setear GPIO 22 como entrada!\n");
		goto Gpio22Error;
	}


	return 0;
Gpio22Error:
	gpio_free(22);
Gpio17Error:
	gpio_free(17);
Gpio4Error:
	gpio_free(4);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/**
 * @brief La funcion es llamada cuando el modulo es removido del kernel
 */
static void __exit ModuleExit(void) {
	gpio_set_value(4, 0);
	gpio_free(17);
    gpio_free(22);
	gpio_free(4);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Adios Kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

