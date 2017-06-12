#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = -1;
	char buffer[128] = {0};
	int i = 0, j = 0;

	fd = open("/dev/cdata-ts", O_RDWR);
	if (fd < 0) {
		printf("open device failed\n");
		exit(1);
	}

	close(fd);
}
