/*
 * Practice for linux driver
 *
*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/input.h>
#include <linux/slab.h> //kmalloc
#include <asm/io.h>
#include <asm/uaccess.h>

#define MAJOR_NUMBER	121
#define DEVICE_NAME		"cdata"

static size_t cdata_read(struct file *filp, char __user *user, size_t size, loff_t *off)
{
	printk(KERN_INFO "<%p> cdata_read\n", filp);
	return 0;
}

static size_t cdata_write(struct file *filp, const char __user *user, size_t size, loff_t *off)
{
	printk(KERN_INFO "<%p> cdata_write\n", filp);
	return 0;
}

static long cdata_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
}

static int cdata_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "<%p> cdata open\n", filp);
	return 0;
}

static int cdata_close(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "<%p> cdata_close\n", filp);
	return 0;
}

static struct file_operations cdata_fops = {
	owner:				THIS_MODULE,
	open:			 	cdata_open,
	read:			 	cdata_read,
	write:			 	cdata_write,
	unlocked_ioctl:		cdata_ioctl,
	release:		 	cdata_close
};

int cdata_init_module(void)
{
	if (register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &cdata_fops) < 0) {
		printk(KERN_ALERT "cdata register failed\n");
		return -1;
	}
	printk(KERN_INFO "cdata register\n");
	return 0 ;
}

void cdata_cleanup_module(void)
{
	unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME);
	printk(KERN_INFO "cdata cleanup module\n");
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_DESCRIPTION("Practice device driver");
MODULE_LICENSE("GPL");
