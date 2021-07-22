#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include "lddbus.h"

static char *Version = "$Revision: 1.9 $\n";

//--------------------------------- bus ----------------------------------------

static int ldd_match(struct device *dev, struct device_driver *drv)
{
    struct ldd_device *pdev = to_ldd_device(dev);

    return !strncmp(pdev->name, drv->name, strlen(drv->name));
}

struct bus_type ldd_bus_type = {
    .name = "ldd",
    .match = ldd_match,
};

//--------------------------------- device --------------------------------------

static ssize_t show_bus_version(struct bus_type *bus, char *buf)
{
    return snprintf(buf, strlen(Version), "%s\n", Version);
}

static BUS_ATTR(version, S_IRUGO, show_bus_version, NULL);

// parent device
static void ldd_bus_release(struct device *dev)
{
    printk(KERN_DEBUG "lddbus release\n");
}
static void ldd_dev_release(struct device *dev){ }

struct device ldd_bus = {
    .init_name   = "ldd_bus", // ldd_bus 就是总线的名字
    .release  = ldd_bus_release
};

int register_ldd_device(struct ldd_device *ldddev)
{

    ldddev->dev.bus = &ldd_bus_type;
    ldddev->dev.parent = &ldd_bus;
    ldddev->dev.release = ldd_dev_release;
    return device_register(&ldddev->dev);
}
EXPORT_SYMBOL(register_ldd_device);

void unregister_ldd_device(struct ldd_device *ldddev)
{
    device_unregister(&ldddev->dev);
}
EXPORT_SYMBOL(unregister_ldd_device);

//--------------------------------- driver --------------------------------------

static ssize_t show_version(struct device_driver *driver, char *buf)
{
    struct ldd_driver *ldriver = to_ldd_driver(driver);

    sprintf(buf, "%s\n", ldriver->version);
    return strlen(buf);
}

int register_ldd_driver(struct ldd_driver *driver)
{
    int ret;

    driver->driver.bus = &ldd_bus_type;
    ret = driver_register(&driver->driver);
    if (ret)
        return ret;
    driver->version_attr.attr.name = "version";
    //driver->version_attr.attr.owner = driver->module;
    driver->version_attr.attr.mode = S_IRUGO;
    driver->version_attr.show = show_version;
    driver->version_attr.store = NULL;
    return driver_create_file(&driver->driver, &driver->version_attr);
}

void unregister_ldd_driver(struct ldd_driver *driver)
{
    driver_unregister(&driver->driver);
}
EXPORT_SYMBOL(register_ldd_driver);
EXPORT_SYMBOL(unregister_ldd_driver);

/*--------------------------------- bus ----------------------------------------*/

static int __init ldd_bus_init(void)
{
    int ret;
    device_register(&ldd_bus);
    ret = bus_register(&ldd_bus_type);
    if (ret)
        return ret;
    if (bus_create_file(&ldd_bus_type, &bus_attr_version))
        printk(KERN_NOTICE "Unable to create version attribute\n");

    return ret;
}

static void ldd_bus_exit(void)
{
    bus_unregister(&ldd_bus_type);
}

module_init(ldd_bus_init);
module_exit(ldd_bus_exit);
MODULE_AUTHOR("zt");
MODULE_LICENSE("GPL");