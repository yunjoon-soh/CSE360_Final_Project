#include <stdio.h>

int main(){
	char* path="./test.txt";

	fopen(path, "r");
	printf("fopen called\n");

	fclose(NULL);
	printf("fclose called\n");

	return 0;
}