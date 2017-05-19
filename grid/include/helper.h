#ifndef __HELPER_H__
#define __HELPER_H__

#include "common.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int getAvailableFd(const char * path);
int retrievePageOfFile(void* inner_addr, char* rBuf);

#endif