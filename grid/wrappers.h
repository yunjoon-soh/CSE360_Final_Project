#ifndef __WRAPPERS_H__
#define __WRAPPERS_H__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

long int Strtol(const char *nptr, char **endptr, int base);
char *Strncpy(char *dest, const char *src, size_t n);

pid_t Fork(void);
pid_t Waitpid(pid_t pid, int *status, int options);

#endif
