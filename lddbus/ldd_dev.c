#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sched.h> 
#include <asm/uaccess.h>
#include "lddbus.h"

static dev_t devid;

static struct ldd_device ldd_dev = {
    .name = "myldd",	
    .dev = {		
        .init_name = "myldd",
    },
};

static int ldd_dev_init(void) {

    alloc_chrdev_region(&devid, 0, 1, "mylddtest");
    ldd_dev.dev.devt = devid;   //设备号
    register_ldd_device(&ldd_dev);
    return 0;
}

static void ldd_dev_exit(void) {
    unregister_ldd_device(&ldd_dev);
}

module_init(ldd_dev_init);
module_exit(ldd_dev_exit);
MODULE_LICENSE("GPL");
