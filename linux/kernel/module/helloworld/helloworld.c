#include <linux/module.h>
#include <linux/kernel.h>

int init_hello_module(void)
{
    printk("***************Start***************\n");
    printk("Hello World! Start of hello world module!\n");
    return 0;
}

void exit_hello_module(void)
{
    printk("***************End***************\n");
    printk("Hello World! End of hello world module!\n");
}


MODULE_LICENSE("Dual BSD/GPL");
module_init(init_hello_module);
module_exit(exit_hello_module);
