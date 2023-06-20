#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "temperatura"
#define DATA_FILE "/datos"
#define BUFFER_SIZE 80

static char data_buffer[BUFFER_SIZE];

static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t temperatura_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    int bytes_written = 0;

    if (*off > 0)
        return -EFAULT;

    if (len > BUFFER_SIZE - 1)
        len = BUFFER_SIZE - 1;

    if (copy_from_user(data_buffer, buf, len))
        return -EFAULT;

    data_buffer[len] = '\0';
    *off += len;
    bytes_written = len;

    // Escribir los datos en el archivo /datos
    struct file *filep;
    mm_segment_t old_fs;
    int ret;

    filep = filp_open(DATA_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (IS_ERR(filep)) {
        printk(KERN_ALERT "Error opening file %s\n", DATA_FILE);
        return -EIO;
    }

    old_fs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(filep, data_buffer, len, &filep->f_pos);
    if (ret < 0) {
        printk(KERN_ALERT "Error writing to file %s\n", DATA_FILE);
        ret = -EIO;
    } else {
        printk(KERN_INFO "Data written to file %s: %s\n", DATA_FILE, data_buffer);
        ret = len;
    }

    filp_close(filep, NULL);
    set_fs(old_fs);

    return ret;
}

static struct file_operations fops = {
    .write = temperatura_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    int ret = register_chrdev(0, DEVICE_NAME, &fops);

    if (ret < 0) {
        printk(KERN_ALERT "Failed to register char device: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Char device registered: %s\n", DEVICE_NAME);
    return 0;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(0, DEVICE_NAME);
    printk(KERN_INFO "Char device unregistered: %s\n", DEVICE_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peaky Coders");
MODULE_DESCRIPTION("Módulo CDD para leer datos de /dev/temperatura");
MODULE_VERSION("0.1");

