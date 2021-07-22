#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sched.h> 
#include <asm/uaccess.h>
#include <linux/io.h>
#include "lddbus.h"
 
struct ldd_driver ldd_drv = {
	.version 	= "version 1.0\n",	
	.driver = {
		.name = "myldd",
	},
};
 
static int ldd_drv_init(void){
	register_ldd_driver(&ldd_drv);
	return 0;
}
 
static void ldd_drv_exit(void){
	
	unregister_ldd_driver(&ldd_drv);
}
 
module_init(ldd_drv_init);
module_exit(ldd_drv_exit);
MODULE_LICENSE("GPL");
