#include "lookupTable.h"

long keyArray[MAX_TABLE_SIZE];
void* value[MAX_TABLE_SIZE];
int last = 0;

// return NULL on failure
void* getMappedAddr(long key){
	for(int i = 0; i < last; i++){
		if(keyArray[i] == key)
			return value[i];
	}

	return NULL;
}


// return 0 on success -1 on failure
void* addKeyValuePair(int fd){
	if(last >= MAX_TABLE_SIZE){
		return (void*)-1;
	}

	keyArray[last] = fd;
	value[last] = malloc(PAGE_SIZE);

	return value[last++];
}