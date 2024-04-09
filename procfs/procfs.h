#ifndef PROCFS_MODULE_H
#define PROCFS_MODULE_H

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/minmax.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#define SUCCESS 0
#define BUFLEN 256
#define PROCFS_NAME "procent"

static ssize_t proc_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t proc_write(struct file*, const char __user *, size_t, loff_t*);

#endif //PROCFS_MODULE_H