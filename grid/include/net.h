#ifndef __NET_H__
#define __NET_H__
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

void connection(char* sentMessage);
int fopenConnection(char* path, char* addr);
void server();

#define INVALID_FILE_NAME "Invalid file name\n"
#define INVALID_COMMAND "Invalid command\n"
#endif
