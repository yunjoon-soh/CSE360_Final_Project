#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
int puts(const char *message)
{
	int (*new_puts)(const char *message);
	int result;
	
	// save original puts
	new_puts = dlsym(RTLD_NEXT, "puts");
	printf("Hooked!");

	// If the message is Hello world!
	if(strcmp(message, "Hello world!\n") == 0)
	{
		// Print the following, using original puts
		result = new_puts("Goodbye, cruel world!n");
	}
	else
	{
		// if the string is different, use original puts() to echo
		result = new_puts(message);
	}
	return result;
}
