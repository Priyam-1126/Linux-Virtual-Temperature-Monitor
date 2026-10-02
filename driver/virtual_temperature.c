// Linux Virtual Temperature  Device Driver

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "virtual_temperature"
#define CLASS_NAME "vtemp"
#define BUFFER_SIZE 64

static dev_t device_number;
static struct cdev vtemp_cdev;
static struct class *vtemp_class;
static struct device *vtemp_device;
static DEFINE_MUTEX(vtemp_mutex);
static int temperature = 28;

module_param(temperature, int, 0644);
MODULE_PARM_DESC(temperature, "Initial temperature in Celsius");

static int vtemp_open(struct inode *inode, struct file *file)
{
    pr_info(DRIVER_NAME ": device opened\n");
    return 0;
}

static int vtemp_release(struct inode *inode, struct file *file)
{
    pr_info(DRIVER_NAME ": device closed\n");
    return 0;
}

static ssize_t vtemp_read(struct file *file, char __user *user_buffer,
                          size_t count, loff_t *offset)
{
    char buffer[BUFFER_SIZE];
    int len;

    mutex_lock(&vtemp_mutex);
    len = scnprintf(buffer, sizeof(buffer), "%d\n", temperature);
    mutex_unlock(&vtemp_mutex);

    if (count < len)
        return -EINVAL;

    if (copy_to_user(user_buffer, buffer, len))
        return -EFAULT;

    *offset = 0;
    return len;
}

static ssize_t vtemp_write(struct file *file, const char __user *user_buffer,
                           size_t count, loff_t *offset)
{
    char buffer[BUFFER_SIZE];
    long value;

    if (count == 0 || count >= sizeof(buffer))
        return -EINVAL;

    if (copy_from_user(buffer, user_buffer, count))
        return -EFAULT;

    buffer[count] = '\0';

    if (kstrtol(buffer, 10, &value) != 0)
        return -EINVAL;

    if (value < -50 || value > 150)
        return -ERANGE;

    mutex_lock(&vtemp_mutex);
    temperature = (int)value;
    mutex_unlock(&vtemp_mutex);

    pr_info(DRIVER_NAME ": temperature updated to %d C\n", temperature);
    return count;
}

static const struct file_operations vtemp_fops = {
    .owner = THIS_MODULE,
    .open = vtemp_open,
    .read = vtemp_read,
    .write = vtemp_write,
    .release = vtemp_release,
};

static int __init vtemp_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME);
    if (ret < 0)
        return ret;

    cdev_init(&vtemp_cdev, &vtemp_fops);
    vtemp_cdev.owner = THIS_MODULE;

    ret = cdev_add(&vtemp_cdev, device_number, 1);
    if (ret < 0)
        goto unregister_region;

    vtemp_class = class_create(CLASS_NAME);
    if (IS_ERR(vtemp_class))
    {
        ret = PTR_ERR(vtemp_class);
        goto delete_cdev;
    }

    vtemp_device = device_create(vtemp_class, NULL, device_number, NULL,
                                 DRIVER_NAME);
    if (IS_ERR(vtemp_device))
    {
        ret = PTR_ERR(vtemp_device);
        goto destroy_class;
    }

    pr_info(DRIVER_NAME ": loaded, initial temperature=%d C\n", temperature);
    return 0;

destroy_class:
    class_destroy(vtemp_class);
delete_cdev:
    cdev_del(&vtemp_cdev);
unregister_region:
    unregister_chrdev_region(device_number, 1);
    return ret;
}

static void __exit vtemp_exit(void)
{
    device_destroy(vtemp_class, device_number);
    class_destroy(vtemp_class);
    cdev_del(&vtemp_cdev);
    unregister_chrdev_region(device_number, 1);
    pr_info(DRIVER_NAME ": unloaded\n");
}

module_init(vtemp_init);
module_exit(vtemp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student Project");
MODULE_DESCRIPTION("Virtual temperature Linux character device driver");
MODULE_VERSION("1.0");
