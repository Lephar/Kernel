#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Character Device Module");

#define DEVICE_NAME "cdev"

static int result;
static long error;

static int major_number = 0;
static int minor_offset = 0;
static int minor_count = 1;

static dev_t dev;
static struct cdev cdev;
static struct class* class;
static struct device* device;
static struct file_operations empty_fops;

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

    cdev_init(&cdev, &empty_fops);
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

    return 0;

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
