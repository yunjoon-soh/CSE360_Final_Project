#ifndef __NET_H__
#define __NET_H__

#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

void connection(char* sentMessage);
int fopenConnection(char* path, char* addr, char* recvBuf);
void server();
#define SNOOPY_ADDR "127.0.0.1"

#define INVALID_FILE_NAME "Invalid file name\n"
#define INVALID_COMMAND "Invalid command\n"
#define FILE_NOT_OPEN "File not open\n"
#endif
