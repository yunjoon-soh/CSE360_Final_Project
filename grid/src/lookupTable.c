#include "lookupTable.h"

long keyArray[MAX_TABLE_SIZE];
void* value[MAX_TABLE_SIZE];

char* fileNames[MAX_TABLE_SIZE];
FILE* keyFile[MAX_TABLE_SIZE];
void* valueFile[MAX_TABLE_SIZE];
int last = 0, lastF = 0;

// return non-zero if not same
int compare(FILE* f1, FILE* f2){
	//if(f1-> != f2->_ptr)
	//	return 1;
	return 0;
}

// return NULL on failure
void* getMappedAddr(long key){
	for(int i = 0; i < last; i++){
		if(keyArray[i] == key)
			return value[i];
	}

	return NULL;
}

// return NULL on failure
void* getMappedAddrFile(FILE* key){
	for(int i = 0; i < lastF; i++){
		if (!compare(keyFile[i], key))
			return valueFile[i];
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

// return 0 on success -1 on failure
void* addKeyValuePairFile(FILE* f){
	if(lastF >= MAX_TABLE_SIZE){
		return (void*)-1;
	}

	keyFile[lastF] = f;
	valueFile[lastF] = malloc(PAGE_SIZE);

	return valueFile[lastF++];
}

FILE* findFile(char* filename){
	for(int i = 0; i < lastF; i++){
		if(strcmp(fileNames[i], filename) == 0){
			return keyFile[i];
		}
	}

	return NULL;
}
