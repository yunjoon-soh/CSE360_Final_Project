#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int ret = setuid(1);

	if(ret != 0)
		perror("setuid");

	return ret;
}