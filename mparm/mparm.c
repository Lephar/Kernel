#include <linux/init.h>
#include <linux/stat.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lephar");
MODULE_DESCRIPTION("Parameter Passing Module");

static int hello_data = 3;
module_param(hello_data, int, 0);
MODULE_PARM_DESC(hello_data, "An example variable");

static int hello_length = 5;
static int hello_array[5] = {0, 1, 2, 3, 4};
module_param_array(hello_array, int, &hello_length, 0);
MODULE_PARM_DESC(hello_array, "An example array");

static char *hello_string = "World";
module_param(hello_string, charp, 0);
MODULE_PARM_DESC(hello_string, "An example char array");

#define BUF_LEN 1024
static char goodbye_string[BUF_LEN] = "See you tomorrow.";
module_param_string(goodbye_string, goodbye_string, BUF_LEN, 0);
MODULE_PARM_DESC(goodbye_string, "An example string");

static int __init hello_init(void)
{
    pr_info("Hello %s.\n", hello_string);
    pr_info("hello_data value: %d\n", hello_data);
    pr_info("hello_array length: %d\n", hello_length);
    for(int i = 0; i < hello_length; i++) {
        pr_info("hello_array[%d]\t=\t%d\n", i, hello_array[i]);
    }

    return 0;
}

static void __exit hello_cleanup(void)
{
    pr_info("%s\n", goodbye_string);
    pr_info("Goodbye %s.\n", hello_string);
}

module_init(hello_init);
module_exit(hello_cleanup);
