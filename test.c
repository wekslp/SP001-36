#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;
    pid_t child;

    child = fork();
    fd = open("/dev/cdata", O_RDWR);

    if (child != 0) {
        write(fd, "h", 1);
        write(fd, "e", 1);
	sleep(1);
        write(fd, "l", 1);
        write(fd, "l", 1);
        write(fd, "o", 1);
    } else {
        write(fd, "12345", 5);
    }

    close(fd);
}
