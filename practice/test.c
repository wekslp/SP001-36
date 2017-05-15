#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//#include "cdata_ioctl.h"

#define IOCTL 0

int main(void)
{
    int fd;
	char buffer[128] = {0};
    pid_t child;
	char *str = "hello";
#if 1
	child = fork();
    printf("child: %d\n", child);
	fd = open("/dev/cdata", O_RDWR);
#else
	fd = open("/dev/cdata", O_RDWR);
	child = fork();
#endif

#if IOCTL
	ioctl(fd, IOCTL_EMPTY, 0);

	write(fd, str, strlen(str));
	//ioctl(fd, IOCTL_NAME, &data);
	ioctl(fd, IOCTL_SYNC, 0);

	ioctl(fd, IOCTL_NAME, "hello");
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
