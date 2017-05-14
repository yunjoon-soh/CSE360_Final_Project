#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	int fd = open("./openSample.c", O_RDONLY);

	printf("Fd: %d\n", fd);

	return 0;
}
