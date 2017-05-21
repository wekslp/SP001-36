#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "cdata_ioctl.h"

#define IOCTL 0

#define FORK	0

#define FRAME_BUFFER_SIZE		1024*768*2

int main(void)
{
    int fd;
	fd = open("/dev/cdata-misc", O_RDWR);
	char *mem = NULL;

	if (fd < 0) {
		printf("open failed\n");
		return 0;
	}
	mem = mmap(NULL, FRAME_BUFFER_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//printf("mem: %p\n", *mem);
	int i = 0;
	while (i< FRAME_BUFFER_SIZE) {
		*mem = 0x23;
		i++;
		mem++;
	}
	//read(fd, buffer, 128);
	//ioctl(fd, 123, 123);
    sleep(3);
	close(fd);
}
