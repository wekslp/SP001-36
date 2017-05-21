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
#include <linux/platform_device.h>
#include <linux/mm_types.h>
#include <linux/workqueue.h>
#include <linux/kfifo.h>
#include <asm/spinlock.h>
#include <asm/page.h>
#include "cdata_ioctl.h"

#define MAJOR_NUMBER			121
#define DEVICE_NAME				"cdata"

#define MAX_BUFSIZE				32
#define MAX_FRAMBUFFER			1024*768*2

#define USE_PLATFORM_DRIVER		1

#define FRAMEBUFFER_IO_MEM		0xa0000000

static DEFINE_MUTEX(ioctl_lock);


static void *write_framebuffer_with_work(struct work_struct *);
static void *write_framebuffer(unsigned long);

struct cdata_t {
	char				buf[MAX_BUFSIZE];
	int					idx;
	wait_queue_head_t	writeable;
	struct timer_list	timer;
	unsigned char		*iomem;
	struct work_struct	work;
	struct mutex		write_lock;
	struct kfifo		out_fifo;
	spinlock_t			out_fifo_lock;
};

static void *write_framebuffer_with_work(struct work_struct *pWork)
{
	printk(KERN_INFO "write_framebuffer\n");
	struct cdata_t *cdata = container_of(pWork, struct cdata_t, work);
	unsigned char *iomem;
	int i;
	
	/* write to hardware is done, reset the length */
	cdata->idx = 0;
	
	/* 
	 * framebuffer 
	 * need to add "vga=791" to boot command line
	*/

	iomem = cdata->iomem;
	for (i = 0;i < (MAX_FRAMBUFFER-1); i++) {
		writeb(0x02, iomem+i);
	}

	/* wake up queue process */
	wake_up_interruptible(&cdata->writeable);
}

static size_t cdata_read(struct file *filp, char __user *user, size_t size, loff_t *off)
{
	printk(KERN_INFO "<%p> cdata_read\n", filp);
	return 0;
}

void *write_framebuffer(unsigned long arg)
{
	printk(KERN_INFO "write_framebuffer\n");
	struct cdata_t *cdata = (struct cdata_t *)arg;
	unsigned char *iomem;
	int i;
	/* write to hardware is done, reset the length */
	cdata->idx = 0;
	
	/* 
	 * framebuffer 
	 * need to add "vga=791" to boot command line
	*/

	iomem = cdata->iomem;
	for (i = 0;i < (MAX_FRAMBUFFER-1); i++) {
		writeb(0x02, iomem+i);
	}

	/* wake up queue process */
	wake_up_interruptible(&cdata->writeable);
}

static size_t cdata_write(struct file *filp, const char __user *user, size_t size, loff_t *off)
{
	printk(KERN_INFO "cdata_write\n");
	struct cdata_t *cdata;
	struct timer_list *timer;
	int idx;
	int i;
	int ret;
	char *buf;
	unsigned char 	*iomem;
	unsigned int 	copied;

	DECLARE_WAITQUEUE(wait, current);
	
	cdata = (struct cdata_t *)filp->private_data;

	if (mutex_lock_interruptible(&cdata->write_lock) < 0)
		return -EINTR;

	idx = cdata->idx;
	buf = cdata->buf;
	timer = &cdata->timer;

	for (i = 0;i < size; i++) {
		if (idx > (MAX_BUFSIZE-1)){
			add_wait_queue(&cdata->writeable, &wait);
			current->state = TASK_INTERRUPTIBLE;
			
			/* timer_list */
			//timer->data = (unsigned long)cdata;
			//timer->expires = 1;
			//timer->function = write_framebuffer;
			//add_timer(timer);
			/* end */
			schedule_work(&cdata->work);
			

			mutex_unlock(&cdata->write_lock);

			/* return cpu resource and do process context switch */
			schedule();

			remove_wait_queue(&cdata->writeable, &wait);
			
			/* if data is transfer to hardware, we have to update the index */
			idx = cdata->idx;

			if (mutex_lock_interruptible(&cdata->write_lock) < 0)
				return -EINTR;
		}
		//copy_from_user(&buf[idx], &user[i], 1);
		//kfifo_from_user(&cdata->out_fifo, &user[i], 1, &copied);
		idx++;
	}
	
	cdata->idx = idx;
	
	printk(KERN_INFO "<%p> cdata_write: %s\n", filp, buf);
	ret = 0;
	mutex_unlock(&cdata->write_lock);
fail:
	return ret;
}

static long cdata_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;

	ret = 0;
	
	switch (cmd) {
	case IOCTL_EMPTY:
		printk(KERN_INFO "IOCTL_EMPTY\n");
		break;
	case IOCTL_SYNC:
		printk(KERN_INFO "IOCTL_SYNC\n");
		break;
	case IOCTL_NAME:	
		printk(KERN_INFO "IOCTL_NAME\n");
		break;
	default:
		ret = -1;
	}
	return ret;
}

static int cdata_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "<%p> cdata open\n", filp);
	struct cdata_t *cdata;
	
	/* initial data */
	/* GFP_KERNEL will cause Reentrant 講義 page 65*/
	cdata = kzalloc(sizeof(struct cdata_t), GFP_KERNEL);
	cdata->idx = 0;
	
	/* wait queue */
	init_waitqueue_head(&cdata->writeable);
	/* end */
	
	/* timer_list */
	init_timer(&cdata->timer);
	/* end */
	mutex_init(&cdata->write_lock);
	INIT_WORK(&cdata->work, write_framebuffer_with_work);
	//init_kfifo(
	/* map physical address to virtual address */
	cdata->iomem = ioremap(FRAMEBUFFER_IO_MEM, MAX_FRAMBUFFER);

	filp->private_data = (void *)cdata;
	return 0;
}

static int cdata_close(struct inode *inode, struct file *filp)
{
	struct cdata_t *cdata;
	cdata = (struct cdata_t *)filp->private_data;
	char *buf;
	buf = cdata->buf;
	//kfifo_get(cdata->out_fifo, buf);
	printk(KERN_INFO "<%p> cdata_close: %s\n", filp, cdata->buf);
	
	/* timer_list */
	del_timer(&cdata->timer);
	/* end */

	kfree(filp->private_data);
	return 0;
}

static int cdata_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long start = 0, end = 0;
	int size = 0;
	start = vma->vm_start;
	end = vma->vm_end;
	size = end - start;
	printk(KERN_INFO "MMAP: %d\n", size);
#if 0
	if (remap_pfn_range(vma, vma->vm_start, FRAMEBUFFER_IO_MEM, size, vma->vm_page_prot) < 0)
#else
	/* with my kernel we need use io_remap_pfn_range */
	if (io_remap_pfn_range(vma, vma->vm_start, FRAMEBUFFER_IO_MEM >> PAGE_SHIFT, size, vma->vm_page_prot) < 0)
#endif
		return -EAGAIN;
	printk(KERN_INFO "remap %p to %x\n", start, FRAMEBUFFER_IO_MEM);
	return 0;
}

static struct file_operations cdata_fops = {
	owner:				THIS_MODULE,
	open:			 	cdata_open,
	read:			 	cdata_read,
	write:			 	cdata_write,
	unlocked_ioctl:		cdata_ioctl,
	release:		 	cdata_close,
	mmap:				cdata_mmap
};

static struct miscdevice cdata_miscdev = {
	.minor	= 77,
	.name	= "cdata-misc",
	.fops	= &cdata_fops,
};

static int cdata_plat_probe(struct platform_device *pdev)
{
	int ret;
	ret = misc_register(&cdata_miscdev);
	if (ret < 0) {
		printk(KERN_ALERT "cdata_plat_probe failed\n");
		goto exit;
	}
	printk(KERN_INFO "cdata_plat_probe success\n");
exit:
	return ret;
}

static int cdata_plat_remove(struct platfor_device *pdev)
{
	misc_deregister(&cdata_miscdev);
	printk(KERN_INFO "cdata_plat_remove\n");
}

static struct platform_driver cdata_platform_driver = {
	.probe = cdata_plat_probe,
	.remove = cdata_plat_remove,
	.driver = {
		.name = "cdata",
		.owner = THIS_MODULE,
	}
};

static int cdata_init_module(void)
{
#if USE_PLATFORM_DRIVER
	if (platform_driver_register(&cdata_platform_driver) < 0) {
		printk(KERN_ALERT "platform driver register failed\n");
		return -1;
	}
#else
	if (register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &cdata_fops) < 0) {
		printk(KERN_ALERT "cdata register failed\n");
		return -1;
	}
#endif
	printk(KERN_INFO "cdata register\n");
	return 0 ;
}

static void cdata_cleanup_module(void)
{
#if USE_PLATFORM_DRIVER
	platform_driver_unregister(&cdata_platform_driver);
#else
	unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME);
#endif
	printk(KERN_INFO "cdata cleanup module\n");
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_DESCRIPTION("Practice device driver");
MODULE_LICENSE("GPL");
