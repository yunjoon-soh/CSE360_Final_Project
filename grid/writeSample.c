#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char *testStr = "TestString\n";

	write(STDOUT_FILENO, testStr, 12);

	return 0;
}