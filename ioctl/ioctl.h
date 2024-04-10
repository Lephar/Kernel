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

enum {
    CDEV_NOT_IN_USE = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};

#define SUCCESS 0
#define BUFLEN 256
#define DEVICE_NAME "ioctl_dev"

#define MAJOR_NUM 100
#define IOCTL_VALGET _IOR(MAJOR_NUM, 0, unsigned long)
#define IOCTL_VALSET _IOW(MAJOR_NUM, 1, unsigned long)

static int device_open(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int, unsigned long);
static int device_release(struct inode *, struct file *);

#endif //IOCTL_MODULE_H