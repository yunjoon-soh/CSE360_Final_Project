#include "libex.h"

int puts(const char *message)
{
	int (*old_puts)(const char *message);
	int result;

	// save original puts
	old_puts = dlsym(RTLD_NEXT, "puts");

	// If the message is Hello world!
	if (strcmp(message, "Hello world!\n") == 0)
	{
		// Print the following, using original puts
		result = old_puts("Goodbye, cruel world!\n");
	}
	else
	{
		// if the string is different, use original puts() to echo
		result = old_puts(message);
	}
	return result;
}

