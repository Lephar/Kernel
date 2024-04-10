#include "procfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Proc Entry Module");

static struct proc_dir_entry *proc_entry;

static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write
};

static char in_msg[BUFLEN];
static char out_msg[BUFLEN];

static size_t out_msg_len;

static int __init procfs_init(void)
{
    proc_entry = proc_create(PROCFS_NAME, 0644, NULL, &proc_ops);

    if(!proc_entry) {
        pr_alert("Failed to create proc entry\n");
        return -ENOMEM;
    }

    pr_info("Created proc entry: /proc/%s\n", PROCFS_NAME);

    snprintf(out_msg, BUFLEN, "Message from the kernel module!\n");
    out_msg_len = strnlen(out_msg, BUFLEN);

    return SUCCESS;
}

static ssize_t proc_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    size_t bytes_remained = out_msg_len - *offset;

    if(!bytes_remained) {
        pr_info("Message successfully read by the user:\n\t%s\n", out_msg);
        *offset = 0;
        return 0;
    }

    size_t bytes_to_be_copied = min(bytes_remained, length);
    size_t bytes_not_copied = copy_to_user(buffer, out_msg + *offset, bytes_to_be_copied);
    size_t bytes_copied = bytes_to_be_copied - bytes_not_copied;

    (*offset) += bytes_copied;
    return bytes_copied;
}

static ssize_t proc_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset) {
    size_t bytes_to_be_copied = min(length, BUFLEN - 1);
    size_t bytes_not_copied = copy_from_user(in_msg + *offset, buffer, bytes_to_be_copied);
    size_t bytes_copied = bytes_to_be_copied - bytes_not_copied;

    if(!bytes_not_copied) {
        in_msg[bytes_copied] = '\0';
        pr_info("Message written by the user:\n\t%s\n", in_msg);
    }

    (*offset) += bytes_copied;
    return bytes_copied;
}

static void __exit procfs_exit(void)
{
    proc_remove(proc_entry);
    pr_info("Proc entry removed: /proc/%s\n", PROCFS_NAME);
}

module_init(procfs_init);
module_exit(procfs_exit);
