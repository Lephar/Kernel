#include "procfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Proc Entry Module");

static struct proc_dir_entry* proc_entry;

static const struct proc_ops proc_ops = { 
    .proc_read = proc_read, 
    .proc_write = proc_write
};

static char in_msg[BUFLEN];
static char out_msg[BUFLEN];

static int __init procfs_init(void)
{
    proc_entry = proc_create(PROCFS_NAME, 0644, NULL, &proc_ops);

    if(!proc_entry) {
        proc_remove(proc_entry);
        pr_alert("Failed to create proc entry\n");
        return -ENOMEM;
    }

    pr_info("Created proc entry: /proc/%s\n", PROCFS_NAME);
    snprintf(out_msg, BUFLEN, "Message from the kernel module!\n");

    return SUCCESS;
}

static ssize_t proc_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;

    while(bytes_read < length && *(out_msg + *offset)) {
        put_user(*(out_msg + (*offset)++), buffer++);
        bytes_read++;
    }

    if(!bytes_read) {
        pr_info("Message successfuly read by the user:\n\t%s\n", out_msg);
        pr_info("File read:\n\t%s\n", filp->f_path.dentry->d_name.name);
        *offset = 0;
    }

    return bytes_read;
}

static ssize_t proc_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset) {
    int bytes_written = 0;

    while(bytes_written < length && bytes_written < BUFLEN - 1) {
        get_user(in_msg[bytes_written], buffer + bytes_written);
        bytes_written++;
    }

    in_msg[bytes_written] = '\0';
    pr_info("Message written by the user:\n\t%s\n", in_msg);

    return bytes_written;
}

static void __exit procfs_exit(void)
{
    proc_remove(proc_entry);
    pr_info("Proc entry removed: /dev/%s\n", PROCFS_NAME);
}

module_init(procfs_init);
module_exit(procfs_exit);
