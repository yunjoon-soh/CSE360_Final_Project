#!/bin/sh
num=`git log | egrep ^commit | wc -l`
echo "#define BUILDS $num" > ./include/builds.h

