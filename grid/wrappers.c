#include "wrappers.h"

long int Strtol(const char *nptr, char **endptr, int base){
	errno = 0;
	long val = strtol(nptr, endptr, base);
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val ==0)){
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	if(*endptr == nptr || **endptr != '\0'){
		fprintf(stderr, "Invalid argument: failed parsing numeric value\n");
		exit(EXIT_FAILURE);
	}

	return val;
}

char *Strncpy(char *dest, const char *src, size_t n){
	dest = strncpy(dest, src, n);
	return dest;
}

pid_t Fork(void){
	pid_t toRet = fork();

	return toRet;
}

pid_t Waitpid(pid_t pid, int *status, int options){
	pid_t toRet = waitpid(pid, status, options);
	return toRet;
}

