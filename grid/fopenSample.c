#include <stdio.h>

int main(){
	char* path="./test.txt";

	FILE *f = fopen(path, "r");

	fclose(f);

	return 0;
}