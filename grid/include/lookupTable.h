#ifndef __LOOKUPTABLE_H__
#define __LOOKUPTABLE_H__

#include "common.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TABLE_SIZE 100

void* getMappedAddr(long key);
void* addKeyValuePair(int fd);

void* getMappedAddrFile(FILE* key);
void* addKeyValuePairFile(FILE* f);

#endif