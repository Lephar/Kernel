#ifndef PROCFS_MODULE_H
#define PROCFS_MODULE_H

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/atomic.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/proc_fs.h> 

#define SUCCESS 0
#define BUFLEN 256
#define PROCFS_NAME "procent"

static ssize_t proc_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t proc_write(struct file*, const char __user *, size_t, loff_t*);

#endif //PROCFS_MODULE_H