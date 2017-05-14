/*
 *  *LDT - Linux Driver Template
 *   *
 *    *Copyright (C) 2012 Copyrightonstantine Shulyupin  http://www.makelinux.net/
 *     *
 *      *Dual BSD/GPL License
 *       *
 *        *platform_driver template driver
 *         *Power Management (dev_pm_ops)
 *          *Device Tree (of_device_id)
 *           *
 *            */

#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/mod_devicetable.h>
#include <linux/of_irq.h>

static int cdata_plat_probe(struct platform_device *pdev)
{
	printk(KERN_INFO "cdata_plat_probe\n");
	return 0;
}

static struct platform_driver cdata_plat_driver = {
	.driver = {
		.name= "cdata_platform_dev",
		.owner= THIS_MODULE,
	},
	.probe =					cdata_plat_probe,

};

int cdata_init_module(void)
{
	return platform_driver_register(&cdata_plat_driver);
}

void cdata_cleanup_module(void)
{
	printk(KERN_INFO "cdata_cleanup\n");
	platform_driver_unregister(&cdata_plat_driver);
}

/*
module_platform_driver(cdata_plat_driver);
*/
module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_DESCRIPTION("LDT - Linux Driver Template: platform_driver template");
MODULE_AUTHOR("Constantine Shulyupin <const@makelinux.net>");
MODULE_LICENSE("Dual BSD/GPL");
