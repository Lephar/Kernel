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

#define DEVICE_NAME "chardev"

static int major_number;
static struct class *device_cls;
static struct device *device_info;
static struct file_operations empty_fops;

static int __init chardev_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &empty_fops);

    if (major_number < 0) {
        pr_alert("Registering char device failed with error code %d\n", major_number);
        
        return major_number;
    }

    pr_info("Device is assigned major number %d\n", major_number);

    device_cls = class_create(DEVICE_NAME);
    device_info = device_create(device_cls, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);

    pr_info("/dev/%s is created\n", device_info->class->name);

    return 0;
}

static void __exit chardev_exit(void)
{
    device_destroy(device_cls, MKDEV(major_number, 0));
    class_destroy(device_cls);

    pr_info("/dev/%s is destroyed\n", DEVICE_NAME);

    unregister_chrdev(major_number, DEVICE_NAME);
    
    pr_info("Major number %d is unregistered\n", major_number);
}

module_init(chardev_init);
module_exit(chardev_exit);
