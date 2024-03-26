#include <linux/init.h>
#include <linux/stat.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Hello World Module");

static int __init hello_init(void)
{
    pr_info("Hello World.\n");
    
    return 0;
}

static void __exit hello_cleanup(void)
{
    pr_info("Goodbye World.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
