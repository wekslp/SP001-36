/*
 *  *LDT - Linux Driver Template
 *  
 *  Copyright (C) 2012 Constantine Shulyupin			 http://www.makelinux.net/
 *  
 *  GPL License
 *  
 *  platform_device template driver
 *  
 *  uses
 *  platform_data
 *  resources
 *  
 */

#include <linux/module.h>
#include <linux/platform_device.h>

static struct resource cdata_resource[] = {
};

static void cdata_dev_release(struct device *dev)
{
}

static struct platform_device cdata_platform_device = {
	.name= "cdata_platform_dev",
	.id= -1
};

static int cdata_plat_dev_init(void)
{
	printk(KERN_INFO "cdata_plat_dev_init\n");
	return platform_device_register(&cdata_platform_device);
}

static void cdata_plat_dev_exit(void)
{
	platform_device_unregister(&cdata_platform_device);
}

module_init(cdata_plat_dev_init);
module_exit(cdata_plat_dev_exit);

MODULE_DESCRIPTION("cdata - Linux Driver Template: platform_device");
MODULE_AUTHOR("Constantine Shulyupin <const@makelinux.net>");
MODULE_LICENSE("GPL");
