#include <linux/init.h>
#include <linux/stat.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Hello World Module");

int hello_data = 3;
module_param(hello_data, int, 0);
MODULE_PARM_DESC(hello_data, "An example data");

static int __init hello_init(void)
{
    pr_info("Hello World %d.\n", hello_data);

    return 0;
}

static void __exit hello_cleanup(void)
{
    pr_info("Goodbye World %d.\n", hello_data);
}

module_init(hello_init);
module_exit(hello_cleanup);
