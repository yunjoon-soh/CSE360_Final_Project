# CSE360_Final_Project
Final project for CSE360 Spring 2017.

This README.md file is written for the purpose of explaining details of the project. High-level description/implementation details can be found in the final report.

## File Structure
### Main Program
* *grid/*
 * Makefile

 * *include/*
: Header files

 * *src/*
: Source files

 * *script/*
: Script files

### Building Blocks
* *libcall_intercept/*
: library call intercept sample, it is required to "export LD_PRELOAD=`pwd`/lib.so" to hook
 * Makefile
 * libex.c
 * libex.h
 * hello.c


* *ptrace_example/*
: syscall intercept sample 
 * 

* *seccomp_example/*
: seccomp usage code

* *socket_example/*
: socket communication related code

## Grid Folder


### Source files
* \*Sample.c
: All the sample PROGRAM used during development

* gridD.c, gridS.c
: Main entry source file

* libex.c
: Source code for lib.so, which is used to hook library calls

* net.c
: Socket/Network related functions, it is partially different from *socket_sample/*

* gridDPseudo.c
: Detailed implementation of gridD using pseudo code when related to advanced ptrace topics

* helper.c
: Minor helper functions for gridDPseudo.c

* lookupTable.c
: Simple array of key-value data structure, where key is internal, dummy file descriptor and value is memory address of file content. (See lookupTable section for detail).

* wrapper.c
: Error handled library calls. Note. Not all libcalls are error handled because some developer of this project preferred inline error handling.

### Script Folder
* buildN.sh
: Script to generate builds.h which includes the build number based on the number of git commits. Included as .PHONY in the Makefile



--------------------------------------------------------------------------------------------------------
# Grid
## Usage

* ./gridD -p [PROGRAM] [-d|-dd]
* ./gridS [-d]

--------------------------------------------------------------------------------------------------------
# GridS Internal

## Flow of GridS
1. Parse input with getopt
1. Infinite loop to wait for connection
1. Upon connection read the request packet
1. Parse the packet
1. Execute request
1. Write response packet

## Behaviors Handled (Check net.c::server() for detail)
* fopen
: Upon request for opening a file, action of gridS depends on whether it is a read open or write open. If both are set, return failure. If read open was requested, read the whole file, send the content of the file. If write open was requested, open the file and remember the FILE*. 

* fread
: Ignore. Simulating read will be done on gridD. 

* fwrite
: Write to the open file. If there are multiple of them latest write-opened file will be used. In this case, previous FILE* will just be overwritten without being closed. This is a side-effect. 
Future work: check if requested write-to-file is open based on an additional parameter: filename. Handle the side-effect.

* fclose
: Close the most recent write-open file. 

## Challenge

The reason why multiple write-open request is not handled properly is because there was no easy way to store the FILE* and filename pair. lookupTable.c::compare() is intended to handle this, but wasn't able to figure out within the given time. However, the pseudo code is inside net.c::server() comment. Note that there also should be a way to retrieve the filename and include that in the fwrite request to the server from the libex.c::fwrite. In other words, with just the parameters of fwrite, donor should have a way to find the filename. This may be trivial because I did not go through in detail. The reason that I didn't go through in detail was because even there is an easy solution to the problem it would be meaning less if it is not possible to compare the FILE*.

--------------------------------------------------------------------------------------------------------
# GridD Internal

## Flow of GridD
1. Parse input with getopt
1. Fork a child process that would flag itself to be traced by ptrace.
1. Parent process will be listening to the ptrace and intercept syscalls.
1. Based on the syscall number take appropriate actions.

## Two gridD
Under current version, two different variants of gridD are implemented.

* gridD
: gridD does not have incomplete function. At the same time, all it can do with is log and view the sequence of syscalls made by the untrusted program. This can be later used for various software abnormally detection as discussed in the lecture.


* gridDPseudo
: gridDPseudo includes **incomplete function** call that is involved with peeking the child process for parameters and modifying them before the syscall completion. This will be discussed in Challenge section.

## Challenge

Because the grid platform is targeting arbitrary program and thus it is not guaranteed that user will have the source code, family of execv must be called. By the nature of execv, any seccomp-bpf filter applied to it will be ignored. Thus, monitoring every syscall of interest must be examined at the run time. This affects the performance and raise the difficulty to implement it. 

Once syscall gets initialized it is impossible to halt, abort without terminating the tracee with the kill signal or through mechanisms implemented in ptrace. However, this should not be the case in our software because what we want is for subset of syscalls to act differently and not terminate. From the untrusted program's perspective, every syscall should execute as if it was not hooked. This includes the return value of syscalls after examination of its parameters. This is abstract at this level and it is explained in detail where I talk about the issue with actual implementation.





