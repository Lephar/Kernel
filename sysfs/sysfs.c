#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/stringify.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Sysfs Entry Module");

#define SYSFS_NAME "sysent"

static struct kobject *sysfs_module;

static int exported_variable = 0;

static ssize_t exported_variable_show(struct kobject* kobj, struct kobj_attribute* attr, char* buffer) {
    int result = sprintf(buffer, "%d\n", exported_variable);
    pr_info("%s value queried by the user at the value of %d\n", __stringify(exported_variable), exported_variable);

    return result;
}

static ssize_t exported_variable_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buffer, size_t length) {
    sscanf(buffer, "%d", &exported_variable);
    pr_info("%s value changed to %d\n", __stringify(exported_variable), exported_variable);

    return length;
}

static struct kobj_attribute exported_variable_attribute = __ATTR(exported_variable, 0644, exported_variable_show, exported_variable_store);

static int __init sysfs_entry_init(void)
{
    sysfs_module = kobject_create_and_add(SYSFS_NAME, kernel_kobj);

    if(!sysfs_module) {
        pr_alert("Failed to create %s kobject\n", SYSFS_NAME);

        return -ENOMEM;
    }

    pr_info("%s kobject created and added to the system\n", SYSFS_NAME);

    int result = sysfs_create_file(sysfs_module, &exported_variable_attribute.attr);

    if(result) {
        pr_alert("Failed to create sysfs file /sys/kernel/%s/%s\n", SYSFS_NAME, __stringify(exported_variable));
        kobject_put(sysfs_module);

        return result;
    }

    pr_info("sysfs entry for %s created at /sys/kernel/%s/%s\n", __stringify(exported_variable), SYSFS_NAME, __stringify(exported_variable));

    return 0;
}

static void __exit sysfs_entry_exit(void)
{
    sysfs_remove_file(sysfs_module, &exported_variable_attribute.attr);
    pr_info("sysfs entry for %s removed from /sys/kernel/%s/%s\n", __stringify(exported_variable), SYSFS_NAME, __stringify(exported_variable));

    kobject_put(sysfs_module);
    pr_info("%s kobject removed\n", SYSFS_NAME);
}

module_init(sysfs_entry_init);
module_exit(sysfs_entry_exit);
