#include "fops.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("File Operations Module");

static int major_number;

static struct class *device_cls;
static struct device *device_info;

static struct file_operations device_fops = {
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_IN_USE);

static int __init chardev_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &device_fops);

    if (major_number < 0) {
        pr_alert("Registering char device failed with error code %d.\n", major_number);
        
        return major_number;
    }

    pr_info("Device is assigned major number %d.\n", major_number);

    device_cls = class_create(DEVICE_NAME);
    device_info = device_create(device_cls, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);

    pr_info("/dev/%s is created.\n", device_info->class->name);

    return 0;
}

static int device_open(struct inode *inode, struct file *file) {
    if(atomic_cmpxchg(&already_open, CDEV_NOT_IN_USE, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;

    try_module_get(THIS_MODULE);

    pr_info("Device is opened by the user.\n");

    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    pr_alert("Reading from this module is not supported.\n");

    return -EINVAL;
}

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset) {
    pr_alert("Writing to this module is not supported.\n");

    return -EINVAL;
}

static int device_release(struct inode *inode, struct file *file) {
    atomic_set(&already_open, CDEV_NOT_IN_USE);
    module_put(THIS_MODULE);

    pr_info("Device is released.\n");

    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    device_destroy(device_cls, MKDEV(major_number, 0));
    class_destroy(device_cls);

    pr_info("/dev/%s is destroyed.\n", DEVICE_NAME);

    unregister_chrdev(major_number, DEVICE_NAME);
    
    pr_info("Major number %d is unregistered.\n", major_number);
}

module_init(chardev_init);
module_exit(chardev_exit);
