#ifndef IOCTL_MODULE_H
#define IOCTL_MODULE_H

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/atomic.h>
#include <linux/printk.h>
#include <linux/uaccess.h>

#define SUCCESS 0
#define BUFLEN 256
#define DEVICE_NAME "ioctl_dev"

#define IOC_MAGIC '\x66'
#define IOCTL_VALSET _IOR(IOC_MAGIC, 0, struct ioctl_arg)
#define IOCTL_VALGET _IOW(IOC_MAGIC, 1, struct ioctl_arg)
#define IOCTL_VALGET_NUM _IOR(IOC_MAGIC, 2, int)
#define IOCTL_VALSET_NUM _IOW(IOC_MAGIC, 3, int)
#define IOCTL_VAL_MAX_NUMBER 3

enum {
    CDEV_NOT_IN_USE = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};

struct ioctl_arg {
    unsigned int val;
};

struct ioctl_data {
    unsigned char val;
    rwlock_t lock;
};

static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char __user*, size_t, loff_t*);
static long device_ioctl(struct file*, unsigned int, unsigned long);
static ssize_t device_write(struct file*, const char __user *, size_t, loff_t*);

#endif //IOCTL_MODULE_H