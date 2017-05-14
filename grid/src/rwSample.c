#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 4
int main(){
	int fd = open("./openSample.c", O_RDONLY);
	printf("Fd: %d\n", fd);

	char buf[SIZE];
	char pBuf[SIZE+2];
	memset(buf, 0, SIZE);
	memset(pBuf, 0, SIZE+2);
	int n = 0;
	while( (n = read(fd, &buf, SIZE)) == SIZE){
		strncpy((char*) &pBuf, (char*) &buf, n);
		pBuf[SIZE]='|';

		printf("%s\n", pBuf);
	}

	return 0;
}
