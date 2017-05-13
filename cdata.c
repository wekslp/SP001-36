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
#include "cdata_ioctl.h"

#define MAX_LEN			32

#define CDATA_MAJOR 121

struct cdata {
	char buf[MAX_LEN];
	int	index;
};

static int cdata_open(struct inode *inode, struct file *filp)
{
	struct cdata *data;
	printk(KERN_ALERT "cdata in open: filp = %p\n", filp);
//	data = kmalloc(sizeof(*data), GFP_KERNEL);
	data = kzalloc(sizeof(*data), GFP_KERNEL);
	data->index = 0;
	filp->private_data = (void *)data;

//	while (1) {
//		current->state = TASK_INTERRUPTIBLE; //prcoess can receive signal and set the state to ready. 
//		//current->state = TASK_UNINTERRUPTIBLE; //prcoess cannot receive signal
//		schedule(); //will set the state to non-running.
//	}
	return 0;
}

static int cdata_close(struct inode *inode, struct file *filp)
{
	struct cdata *c = (struct cdata *)filp->private_data;
	printk(KERN_ALERT "<%p> cdata in close: %s(%d)", filp, c->buf, c->index);
	kfree(c);
	return 0;
}

static ssize_t cdata_write(struct file *filp, const char __user *user, size_t size, loff_t *off)
{
	struct cdata *c = (struct cdata *)filp->private_data;
	int idx = 0;
	int i = 0;	

	char *buf = kzalloc(size, GFP_KERNEL);

	idx = c->index;
#if 0
	
	copy_from_user((void *) buf, user, size);
	for (i=0; i < size; i++) {
		if (idx >= MAX_LEN)
			break;
		c->buf[idx] = buf[i];
		idx++;
	}
#else // Teacher  
	for (i = 0; i < size; i++) {
		copy_from_user(&c->buf[idx], &user[i], 1);
		idx++;
	}
#endif
	c->index = idx;
	kfree(buf);
	printk(KERN_ALERT "<%p> cdata in write\n", filp);
	return 0 ;
}

static size_t cdata_read(struct file *filp,  char __user *user, size_t size, loff_t *off)
{
	printk(KERN_ALERT "<%p> cdata in read\n", filp);
	return 0;
}

static long cdata_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_ALERT "<%p> cdata in ioctl\n", filp);
	struct ioctl_buffer data;
	switch (cmd) {
		case IOCTL_EMPTY:
			printk(KERN_INFO "in ioctl: IOCTL_EMPTY\n");
			break;
		case IOCTL_SYNC:
			printk(KERN_INFO "in ioctl: IOCTL_SYNC\n");
			break;
		case IOCTL_NAME:
			printk(KERN_INFO "in ioctl: IOCTL_NAME\n");
			copy_from_user(&data, (struct ioctl_buffer *) arg, sizeof(data));
			printk(KERN_INFO "DEVICE NAME: %s\n", (char *) data.buf);
			break;
		default:
			return -ENOTTY;
	}

	return 0;
}

// device driver capability.
static struct file_operations cdata_fops = {
    owner:      THIS_MODULE,

    // System call implementation
	open:				cdata_open,
    release:    		cdata_close,
	// Class exercise
    write:				cdata_write,
	unlocked_ioctl:		cdata_ioctl,
	read:				cdata_read
};

int cdata_init_module(void)
{
	// regist to VFS
	if (register_chrdev(CDATA_MAJOR, "cdata", &cdata_fops)) {
	    printk(KERN_ALERT "cdata module: can't registered.\n");
    }
	printk(KERN_INFO "cdata module: registered.\n");
	return 0;
}

void cdata_cleanup_module(void)
{
	unregister_chrdev(CDATA_MAJOR, "cdata");
	printk(KERN_ALERT "cdata module: unregisterd.\n");
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
