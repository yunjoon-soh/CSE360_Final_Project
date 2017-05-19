#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef HOOK_LIB_PATH
# define HOOK_LIB_PATH "/home/sekar/CSE360_Final_Project/libcall_intercept/lib.so"
#endif

int main(int argc, char** argv, char** env)
{
	int pid = -1, status;
	char* ldPath = "";

//	setenv("LD_PRELOAD", HOOK_LIB_PATH, 1);

	printf("This is parent:");
	puts("Hello world!\n");

	// parent
	ldPath = getenv("LD_PRELOAD");
	printf("[Parent]Path: %s\n\n", ldPath);

	// This is to test if child will remain hooked even when changing the setenv
	unsetenv("LD_PRELOAD");

	if( (pid=fork()) == 0){
		//child
		printf("This is child:");
		puts("Hello world!\n");
		ldPath = getenv("LD_PRELOAD");
		printf("[Child]Path: %s\n", ldPath);
		exit(EXIT_SUCCESS);
	}

	// Wait for the child to finish
	waitpid(pid, &status, 0);
	
	printf("Child's pid was %d, with exit status=%d\n", pid, status);
	return 0;
}
