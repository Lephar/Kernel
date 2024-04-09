#ifndef FOPS_MODULE_H
#define FOPS_MODULE_H

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/atomic.h>
#include <linux/printk.h>
#include <linux/uaccess.h>

#define SUCCESS 0
#define BUFLEN 256
#define DEVICE_NAME "fops"

enum {
    CDEV_NOT_IN_USE = 0, 
    CDEV_EXCLUSIVE_OPEN = 1, 
};

static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char __user *, size_t, loff_t*);

#endif //FOPS_MODULE_H