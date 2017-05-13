#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "cdata_ioctl.h"

#define IOCTL 1

int main(void)
{
    int fd;
	char buffer[128] = {0};
    pid_t child;
	char *str = "hello";
#if 1
	child = fork();
    fd = open("/dev/cdata", O_RDWR);
#else
	fd = open("/dev/cdata", O_RDWR);
	child = fork();
#endif
#if IOCTL
	struct ioctl_buffer data;
	sprintf(data.buf,"cdata");
	ioctl(fd, IOCTL_EMPTY, 0);
	ioctl(fd, IOCTL_SYNC, 0);
	ioctl(fd, IOCTL_NAME, &data);
#else
	write(fd, str, strlen(str));
	write(fd, "test", 4);
	if (child != 0)//parent process
		write(fd, "123456789012345678901234567890", 30);
	//read(fd, buffer, 128);
	//ioctl(fd, 123, 123);
#endif
    sleep(3);
	close(fd);
}
