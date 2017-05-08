#include "gridD.h"

/*Grid Donor*/
int main(int argc, char** argv){
	char* end_ptr; // used for strtol
	int opt_i=0, opt_k=0, opt_m=0, opt_c=0, opt_p = 0, opt_d = 0, opt, val=-1;
	long base = 10, val_m=-1, val_c=-1; // base for input number for m or c

	char exePath[1024];
	char** exeArgs = NULL;

	// unset the environment variable
	DEBUG(0, "%s\n", "Unloading previous LD_PRELOAD") // Note. no ; intended

	unsetenv("LD_PRELOAD");

	while( (opt = getopt(argc, argv, "ikm:c:dp:")) != -1 ) {
		switch(opt){
		case 'i':
			opt_i=1;
			break;
		case 'k':
			opt_k=1; break;
		case 'm':
			// convert next option argument to long
			val = Strtol(optarg, &end_ptr, base);

			val_m = val;
			opt_m = 1;
			break;
		case 'c':
			// convert next option argument to long
			val = Strtol(optarg, &end_ptr, base);

			val_c = val;
			opt_c = 1;
			break;
		case 'd':
			opt_d++;
			break;
		case 'p':
			Strncpy(exePath, optarg, 1024);
			opt_p=1;
			break;
		default:
			fprintf(stderr, "%s\n", USAGE);
			exit(EXIT_FAILURE);
		}
	}

	DEBUG(1, "Option i: %d, Option k: %d, Option m: %d %ld, Option c: %d %ld, Option d: %d\n", opt_i, opt_k, opt_m, val_m, opt_c, val_c, opt_d)

	fprintf(stdout, "Starting Grid Donor... Version: %s (Build: %d)\n", SERVER_VERSION, BUILDS);

	fprintf(stdout, "Validating parameters...\n");

	if(opt_p != 1){
		fprintf(stderr, "Option p must be given! Abort execution.\n");
		exit(EXIT_FAILURE);
	} else if(opt_d >= 1){
		fprintf(stderr, "[DEBUG 1]Option p given: %s\n", exePath);
	}

	// Setup libcall intercept, i.e., export LD_PRELOAD and also unsetenv("LD_PRELOAD") so the child will not know 
	// Setup syscall intercept
	// For lib calls, intercept using LD_PRELOAD trick

/*
1. Start by spawning the Snoopy's program

2. Run it inside seccomp

3. Intercept
Whenever Snoopy's program makes a libcall, if it is our interest, intercept it and send to gridServer
Whenever Snoopy's program makes a system call, intercept it and send to gridServer

4. Wait for the server's response and execute according to the server's response inside the interceptor
*/

	if(opt_d >= 1){
		char *ldPath = getenv("LD_PRELOAD");
		fprintf(stderr, "LD_PRELOAD=%s\n", ldPath);
	}


	setenv("LD_PRELOAD", HOOK_LIB_PATH, 1);

	int childPid=-1, status;
	if( (childPid = Fork()) == 0){
		DEBUG(1, "%s\n", "Child process starting...\n");

		// Communicate with the server for input data
		// Note. User of gridD decides which program to run, not the Snoopy

		// When input data is passed on to execv()

		//Child process execl
		execv(exePath, exeArgs);
	}

	// wait for all child processes
	Waitpid(-1, &status, 0);

	DEBUG(1, "Child process(pid=%d) ended with status=%d\n", childPid, status);
	
	exit(EXIT_SUCCESS);	
}

