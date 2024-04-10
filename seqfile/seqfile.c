#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Sequence File Module");

#define PROCFS_NAME "seqfile"

static void *seq_start(struct seq_file *seq_file, loff_t *offset) {
    static unsigned long counter = 0;

    if(!*offset)
        return &counter;

    *offset = 0;
    return NULL;
}

static void *seq_next(struct seq_file *seq_file, void *data, loff_t *offset) {
    (*(unsigned long *)data)++;
    (*offset)++;

    return NULL;
}

static int seq_show(struct seq_file *seq_file, void *data) {
    seq_printf(seq_file, "Counter: %lu\n", *(unsigned long *)data);

    return 0;
}

static void seq_stop(struct seq_file *seq_file, void *data) {
    return;
}

static struct seq_operations seq_ops = {
    .start = seq_start,
    .next = seq_next,
    .show = seq_show,
    .stop = seq_stop
};

static int proc_open(struct inode *inode, struct file *filp) {
    return seq_open(filp, &seq_ops);
}

static struct proc_dir_entry *proc_entry;

static const struct proc_ops proc_ops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = seq_release
};

static int __init procfs_init(void)
{
    proc_entry = proc_create(PROCFS_NAME, 0644, NULL, &proc_ops);

    if(!proc_entry) {
        pr_alert("Failed to create proc entry\n");
        return -ENOMEM;
    }

    pr_info("Created proc entry: /proc/%s\n", PROCFS_NAME);

    return 0;
}

static void __exit procfs_exit(void)
{
    proc_remove(proc_entry);
    pr_info("Proc entry removed: /proc/%s\n", PROCFS_NAME);
}

module_init(procfs_init);
module_exit(procfs_exit);
