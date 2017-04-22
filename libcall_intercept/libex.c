#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
int puts(const char *message)
{
	int (*new_puts)(const char *message);
	int result;
	new_puts = dlsym(RTLD_NEXT, "puts");
	printf("Hooked!");
	if(strcmp(message, "Hello world!\n") == 0)
	{
		result = new_puts("Goodbye, cruel world!n");
	}
	else
	{
		result = new_puts(message);
	}
	return result;
}
