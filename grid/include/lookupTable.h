#ifndef __LOOKUPTABLE_H__
#define __LOOKUPTABLE_H__

#include "common.h"
#include <unistd.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE 100
void* getMappedAddr(long key);
void* addKeyValuePair(int fd);

#endif