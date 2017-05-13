#ifndef CDATA_IOCTL_H
#define CDATA_IOCTL_H

#include <linux/ioctl.h>

#define MAGIG_NUMBER	0x1C
#define MAX_LEN			32

struct ioctl_buffer {
	char buf[32];
};

#define IOCTL_EMPTY		_IO(MAGIG_NUMBER, 0)
#define IOCTL_SYNC		_IO(MAGIG_NUMBER, 1)
#define IOCTL_NAME		_IOW(MAGIG_NUMBER, 2, struct ioctl_buffer)

#endif
