#include "helper.h"

int getAvailableFd(const char * path){
	int ret = open(path, O_TMPFILE);
	return ret;
}

static char* currentHead = (char*) -1;
int retrievePageOfFile(void* inner_addr, char* rBuf){
	memset(rBuf, 0, PAGE_SIZE);

	if(currentHead == (char*) -1){
		currentHead = (char*) inner_addr;
	}

	int i;
	for(i = 0; i < PAGE_SIZE; i++){
		if(*currentHead == '\0'){
			currentHead = (char*) -1;
			break;
		}

		// copy nbytes
		rBuf[i] = *currentHead++;
	}

	return i;
}