#ifndef __GRID_S_H__
#define __GRID_S_H_

#include "common.h"
#include "builds.h"
#include "wrappers.h"
#include "seccomp-bpf.h"
#include "net.h"
#include "helper.h"
#include "lookupTable.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ptrace
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/user.h>   // For constants ORIG_EAX etc
#include <sys/reg.h>

#ifndef RDI
# define RDI 14
#endif

#ifndef RSP
# define RSP 19
#endif

char* USAGE="Usage: ./gridDonor -p [PROGRAM_PATH] [-d]";
char* SERVER_VERSION="Beta v0.1";

static int install_syscall_filter(void);
static void redirect_file(pid_t child, const char *file);

static int modifyArg0(void* FAILURE_DUMMY_FILE, int childPid);
static void modifyReturnValue(long value, int childPid);
static char* peekData(long value);

#define HOOK_LIB_PATH "/home/sekar/CSE360_Final_Project/grid/lib.so"

#define __DEBUG_OUTPUT__ stderr
#define DEBUG(level, format, ...) if(opt_d >= level){ fprintf(__DEBUG_OUTPUT__, "[DEBUG%2d]", level); fprintf(__DEBUG_OUTPUT__, format, ## __VA_ARGS__); }

char* SUCCESS_DUMMY_FILE="1";
char* FAILURE_DUMMY_FILE="2";

#endif

