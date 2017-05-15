#ifndef __GRID_S_H__
#define __GRID_S_H__

#include "builds.h"
#include "wrappers.h"
#include "seccomp-bpf.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ptrace
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/user.h>   // For constants ORIG_EAX etc

char* USAGE="Usage: ./gridDonor -p [PROGRAM_PATH] [-d]";
char* SERVER_VERSION="Beta v0.1";

#define HOOK_LIB_PATH "/home/sekar/CSE360_Final_Project/grid/lib.so"

#define __DEBUG_OUTPUT__ stderr
#define DEBUG(level, format, ...) if(opt_d >= level){ fprintf(__DEBUG_OUTPUT__, "[DEBUG%2d]", level); fprintf(__DEBUG_OUTPUT__, format, ## __VA_ARGS__); }

#endif

