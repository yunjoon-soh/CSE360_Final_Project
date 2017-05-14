# CSE360_Final_Project
Final project for CSE360 Spring 2017

running donor and snoopy: 
gcc donor_socket.c -o donor
gcc snoopy_socket.c -o snoopy

# Grid
gridD.\*: Grid Donor program
gridS.\*: Grid Server program
wrappers: Wrapper of libraries with error handling

# List of syscalls to allow or check or disallow

## Allow
It would be terriblly slow if we intercept every read and write
* read
* write

In multi-threaded program, interprocess communication may frequently happen, so we allow very basic ones
...

Time related functionalities, sending over the network would not make sense
...

Process controls, need to allow spawn child processes

## Must send to server
Opening a file must be checked before execution
* open
* close



------------------------------------------------------------------------
* mmap
* munmap
* gettablesize
* dup, dup2

File and IO related control are also sensitive
* fcntl
* ioctl

## Disallow
User/Group Id related??
Resource Controls?
Virtual Memory related

