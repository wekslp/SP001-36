#ifndef CDATA_IOCTL_H
#define CDATA_IOCTL_H

#include <linux/ioctl.h>

#define MAGIC_NUMBER		0x1C
#define IOCTL_EMPTY			_IO(MAGIC_NUMBER, 0)
#define IOCTL_SYNC			_IO(MAGIC_NUMBER, 1)
#define IOCTL_NAME			_IOW(MAGIC_NUMBER, 2, char *)

#endif
