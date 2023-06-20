#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUF_LEN 80

static int Major;
//static char msg[BUF_LEN];
static char *msg_Ptr;

static int device_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;

    if (*msg_Ptr == 0)
        return 0;

    while (length && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static struct file_operations fops = {
    .read = device_read,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

    return 0;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peaky Coders");
MODULE_DESCRIPTION("Módulo CDD Para leer datos de /dev/ttyUSB0");
MODULE_VERSION("0.1");
