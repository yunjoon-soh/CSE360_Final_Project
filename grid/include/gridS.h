#ifndef __GRID_S_H__
#define __GRID_S_H__

#include "builds.h"
#include "wrappers.h"
#include "net.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* USAGE="Usage: Don't use it";
char* SERVER_VERSION="Beta v0.1";

#define __DEBUG_OUTPUT__ stderr
#define DEBUG(level, format, ...) if(opt_d >= level){ fprintf(__DEBUG_OUTPUT__, "[DEBUG%2d]", level); fprintf(__DEBUG_OUTPUT__, format, ## __VA_ARGS__); }

#endif

