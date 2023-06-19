#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#define DEVICE_NAME "signal_device"
#define GPIO_PIN_HUMIDITY 17
#define GPIO_PIN_TEMPERATURE 18

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu nombre");
MODULE_DESCRIPTION("Character Device Driver para leer señales externas");
MODULE_VERSION("0.1");

static int major_number;
static char command;

static int device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
    char signal[256];

    // Leer la señal correspondiente según el comando recibido
    if (command == '1') {
        // Leer señal de humedad (simulado con un GPIO)
        gpio_set_value(GPIO_PIN_TEMPERATURE, 0);
        gpio_set_value(GPIO_PIN_HUMIDITY, 1);
        msleep(1000);  // Simular periodo de 1 segundo
        sprintf(signal, "Humedad: %.2f", 0.0);  // Reemplazar con lectura real del sensor de humedad
    } else if (command == '2') {
        // Leer señal de temperatura (simulado con un GPIO)
        gpio_set_value(GPIO_PIN_HUMIDITY, 0);
        gpio_set_value(GPIO_PIN_TEMPERATURE, 1);
        msleep(1000);  // Simular periodo de 1 segundo
        sprintf(signal, "Temperatura: %.2f", 0.0);  // Reemplazar con lectura real del sensor de temperatura
    } else {
        return -EINVAL;  // Comando no válido
    }

    // Copiar la señal al espacio de usuario
    if (copy_to_user(buffer, signal, strlen(signal)) != 0) {
        return -EFAULT;  // Error al copiar los datos al espacio de usuario
    }

    return strlen(signal);
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset) {
    if (copy_from_user(&command, buffer, sizeof(char)) != 0) {
        return -EFAULT;  // Error al copiar los datos desde el espacio de usuario
    }

    return sizeof(char);
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init cdd_init(void) {
    // Registrar el CDD y obtener el número mayor
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Error al registrar el CDD: %d\n", major_number);
        return major_number;
    }

    // Configurar los pines GPIO para simular las señales de humedad y temperatura
    gpio_request_one(GPIO_PIN_HUMIDITY, GPIOF_OUT_INIT_LOW, "humidity");
    gpio_request_one(GPIO_PIN_TEMPERATURE, GPIOF_OUT_INIT_LOW, "temperature");

    printk(KERN_INFO "CDD registrado con éxito con número mayor %d\n", major_number);
    return 0;
}

static void __exit cdd_exit(void) {
    // Liberar los pines GPIO
    gpio_free(GPIO_PIN_HUMIDITY);
    gpio_free(GPIO_PIN_TEMPERATURE);

    // Desregistrar el CDD
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "CDD desregistrado\n");
}

module_init(cdd_init);
module_exit(cdd_exit);
