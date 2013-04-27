#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void)
{
	int fd;
	char buf[10] = { 0, 1 };
	char buf2[10];
	fd = open("/dev/my_char", O_RDWR);
	if (fd < 0) {
		printf("Open /dev/my_char");
		return -1;
	}
	write(fd, buf, 2);
	read(fd, buf2, 2);
	close(fd);
	return 0;
}
