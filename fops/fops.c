#include "fops.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("File Operations Module");

static int result;
static long error;

static int major_number = 0;
static int minor_offset = 0;
static int minor_count = 1;

static dev_t dev;
static struct cdev cdev;
static struct class* class;
static struct device* device;

static struct file_operations fops = {
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_IN_USE);

static size_t counter = 0;
static char in_msg[BUFLEN];
static char out_msg[BUFLEN];

static int __init chardev_init(void)
{
    result = alloc_chrdev_region(&dev, minor_offset, minor_count, DEVICE_NAME);

    if(result) {
        error = result;
        pr_alert("Allocating major number failed with error code %ld\n", error);
        goto error_alloc_chrdev_region;
    }

    major_number = MAJOR(dev);
    minor_offset = MINOR(dev);

    pr_info("Character device region allocated:\n");
    pr_info("\tMajor number: %d\n", major_number);
    pr_info("\tMinor number offset: %d\n", minor_offset);
    pr_info("\tMinor number count: %d\n", minor_count);

    cdev_init(&cdev, &fops);
    result = cdev_add(&cdev, dev, minor_count);

    if(result) {
        error = result;
        pr_alert("Adding character device failed with error code %ld\n", error);
        goto error_cdev_add;
    }

    pr_info("Character device initialized and added to the system\n");

    class = class_create(DEVICE_NAME);

    if(IS_ERR(class)) {
        error = PTR_ERR(class);
        pr_alert("Class creation failed with error code %ld\n", error);
        goto error_class_create;
    }

    pr_info("Device class structure created for the device\n");

    device = device_create(class, NULL, dev, NULL, DEVICE_NAME);

    if(IS_ERR(device)) {
        error = PTR_ERR(device);
        pr_alert("Device creation failed with error code %ld\n", error);
        goto error_device_create;
    }

    pr_info("Device created and registered at /dev/%s\n", DEVICE_NAME);

    return SUCCESS;

    error_device_create:
        class_destroy(class);
    error_class_create:
        cdev_del(&cdev);
    error_cdev_add:
        unregister_chrdev_region(dev, minor_count);
    error_alloc_chrdev_region:
        pr_info("Cleaned up\n");

    return error;
}

static int device_open(struct inode *inode, struct file *file) {
    if(atomic_cmpxchg(&already_open, CDEV_NOT_IN_USE, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;

    try_module_get(THIS_MODULE);

    pr_info("Device is opened by the user\n");
    snprintf(out_msg, BUFLEN, "Device is accessed %ld times\n", ++counter);

    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;

    while(bytes_read < length && *(out_msg + *offset)) {
        put_user(*(out_msg + (*offset)++), buffer++);
        bytes_read++;
    }

    if(!bytes_read) {
        pr_info("Message successfully read by the user:\n\t%s\n", out_msg);
        *offset = 0;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset) {
    int bytes_written = 0;

    while(bytes_written < length && bytes_written < BUFLEN - 1) {
        get_user(in_msg[bytes_written], buffer + bytes_written);
        bytes_written++;
    }

    in_msg[bytes_written] = '\0';
    pr_info("Message written by the user:\n\t%s\n", in_msg);

    return bytes_written;
}

static int device_release(struct inode *inode, struct file *file) {
    module_put(THIS_MODULE);
    atomic_set(&already_open, CDEV_NOT_IN_USE);

    pr_info("Device is released\n");

    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    device_destroy(class, dev);
    pr_info("Device destroyed and unregistered from /dev/%s\n", DEVICE_NAME);

    class_destroy(class);
    pr_info("Device class structure destroyed\n");

    cdev_del(&cdev);
    pr_info("Character device deleted\n");

    unregister_chrdev_region(dev, minor_count);
    pr_info("Character device region unregistered:\n");
    pr_info("\tMajor number: %d\n", major_number);
    pr_info("\tMinor number offset: %d\n", minor_offset);
    pr_info("\tMinor number count: %d\n", minor_count);
}

module_init(chardev_init);
module_exit(chardev_exit);
