#include "ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("IOCTL Device Module");

static struct class *class;
static struct device *device;

static long ioctl_arg = 0;
static char message[BUFLEN];

static struct file_operations fops = {
    .open = device_open,
    .read = device_read,
    .unlocked_ioctl = device_ioctl,
    .release = device_release
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_IN_USE);

static int __init ioctl_init(void)
{
    int result = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);

    if(result) {
        pr_alert("Allocating major number failed with error code %d\n", result);
        goto error_register_chrdev;
    }

    pr_info("Character device registered to the system\n");

    class = class_create(DEVICE_NAME);

    if(IS_ERR(class)) {
        result = PTR_ERR(class);
        pr_alert("Class creation failed with error code %d\n", result);
        goto error_class_create;
    }

    pr_info("Device class structure created for the device\n");

    device = device_create(class, NULL, MKDEV(MAJOR_NUM, 0), NULL, DEVICE_NAME);

    if(IS_ERR(device)) {
        result = PTR_ERR(device);
        pr_alert("Device creation failed with error code %d\n", result);
        goto error_device_create;
    }

    pr_info("Device created and registered at /dev/%s\n", DEVICE_NAME);

    return SUCCESS;

    error_device_create:
        class_destroy(class);
    error_class_create:
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    error_register_chrdev:
        pr_info("Cleaned up\n");

    return result;
}

static int device_open(struct inode *inode, struct file *filp) {
    try_module_get(THIS_MODULE);
    pr_info("Device is opened by the user\n");

    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    if(atomic_cmpxchg(&already_open, CDEV_NOT_IN_USE, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;

    int bytes_read = 0;

    snprintf(message, BUFLEN, "Current %s value: %lu\n", __stringify(ioctl_arg), ioctl_arg);

    while(bytes_read < length && *(message + *offset)) {
        put_user(*(message + (*offset)++), buffer++);
        bytes_read++;
    }

    if(!bytes_read) {
        pr_info("Message successfully read by the user:\n\t%s\n", message);
        *offset = 0;
    }

    atomic_set(&already_open, CDEV_NOT_IN_USE);

    return bytes_read;
}

static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    if(atomic_cmpxchg(&already_open, CDEV_NOT_IN_USE, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;

    unsigned long result = SUCCESS;

    if(cmd == IOCTL_VALSET) {
        ioctl_arg = arg;
        pr_info("Set %s: %lu\n", __stringify(ioctl_arg), ioctl_arg);
    }

    else if(cmd == IOCTL_VALGET) {
        result = put_user(ioctl_arg, (unsigned long __user *)arg);
        pr_info("Get %s: %lu\n", __stringify(ioctl_arg), ioctl_arg);
    }

    atomic_set(&already_open, CDEV_NOT_IN_USE);

    return result;
}

static int device_release(struct inode *inode, struct file *filp) {
    module_put(THIS_MODULE);
    pr_info("Device is released\n");

    return SUCCESS;
}

static void __exit ioctl_exit(void)
{
    device_destroy(class, MKDEV(MAJOR_NUM, 0));
    pr_info("Device destroyed and unregistered from /dev/%s\n", DEVICE_NAME);

    class_destroy(class);
    pr_info("Device class structure destroyed\n");

    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    pr_info("Character device unregistered\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);
