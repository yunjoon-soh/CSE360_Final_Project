#include <stdio.h>

int main(){
	char buf[100];
	char* path="./test.txt";

	FILE* f = fopen(path, "r");
	printf("[fopenSample.c] fopen called\n");
	if(f == NULL){
		printf("[fopenSample.c] fopen result was NULL\n");
	}

	fread(buf, sizeof(buf), 1, f);
	printf("[fopenSample.c] fread called: %s\n", buf);

	fclose(f);
	printf("[fopenSample.c] fclose called\n");

	return 0;
}
