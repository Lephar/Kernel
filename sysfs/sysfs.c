#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/printk.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Sysfs Entry Module");

#define SYSFS_NAME "sysent"

static struct kobject *sysfs_module;

static int exported_variable = 0;

static ssize_t exported_variable_show(struct kobject* kobj, struct kobj_attribute* attr, char* buffer) {
    return sprintf(buffer, "%d\n", exported_variable);
}

static ssize_t exported_variable_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buffer, size_t count) {
    sscanf(buffer, "%d", &exported_variable);

    return count;
}

static struct kobj_attribute exported_variable_attribute = __ATTR(exported_variable, 0664, exported_variable_show, exported_variable_store);

static int __init sysfs_entry_init(void)
{
    sysfs_module = kobject_create_and_add(SYSFS_NAME, kernel_kobj);

    if(!sysfs_module) {
        pr_alert("Failed to create kobject\n");

        return -ENOMEM;
    }

    pr_info("Kernel object created and added to the system\n");

    int result = sysfs_create_file(sysfs_module, &exported_variable_attribute.attr);

    if(result) {
        pr_alert("Failed to create sysfs file\n");
        kobject_put(sysfs_module);

        return result;
    }

    pr_info("Sysfs file created\n");

    return 0;
}

static void __exit sysfs_entry_exit(void)
{
    sysfs_remove_file(sysfs_module, &exported_variable_attribute.attr);
    pr_info("Sysfs file removed\n");

    kobject_put(sysfs_module);
    pr_info("Kernel object removed\n");
}

module_init(sysfs_entry_init);
module_exit(sysfs_entry_exit);
