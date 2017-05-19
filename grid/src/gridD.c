#include "gridD.h"

static int install_syscall_filter(void);
static void runGrep(void);
static void runCp(void);
static void runStat(void);
static void runEcho();

/*Grid Donor*/
int main(int argc, char** argv) {
	char* end_ptr; // used for strtol
	int opt_i = 0, opt_k = 0, opt_m = 0, opt_c = 0, opt_p = 0, opt_d = 0, opt, val = -1;
	long base = 10, val_m = -1, val_c = -1; // base for input number for m or c

	char exePath[1024];
	char** exeArgs = NULL;

	// unset the environment variable
	DEBUG(0, "%s\n", "Unloading previous LD_PRELOAD")
	// Note. no ; and this is intended
	// check gridD.h for declaration and detail

	unsetenv("LD_PRELOAD");

	while ( (opt = getopt(argc, argv, "ikm:c:dp:")) != -1 ) {
		switch (opt) {
		case 'i':
			opt_i = 1;
			break;
		case 'k':
			opt_k = 1;
			break;
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
			Strncpy(exePath + 2, optarg, 1022);
			exePath[0] = '.';
			exePath[1] = '/';
			opt_p = 1;
			break;
		default:
			fprintf(stderr, "%s\n", USAGE);
			exit(EXIT_FAILURE);
		}
	}

	DEBUG(1, "Option i: %d, Option k: %d, Option m: %d %ld, Option c: %d %ld, Option d: %d\n", opt_i, opt_k, opt_m, val_m, opt_c, val_c, opt_d)

	fprintf(stdout, "Starting Grid Donor... Version: %s (Build: %d)\n", SERVER_VERSION, BUILDS);

	fprintf(stdout, "Validating parameters...\n");

	// check if opt_p was used
	if (opt_p != 1) {
		fprintf(stderr, "Option p must be given! Abort execution.\n");
		exit(EXIT_FAILURE);
	} else if (opt_d >= 1) {
		fprintf(stderr, "[DEBUG 1]Option p given: %s\n", exePath);
	}

	// Setup libcall intercept, i.e., export LD_PRELOAD and also unsetenv("LD_PRELOAD") so the child will not know
	// Setup syscall intercept
	// For lib calls, intercept using LD_PRELOAD trick

	/*
	1. Start by spawning the Snoopy's program

	2. Run it inside seccomp
	z
	3. Intercept
	Whenever Snoopy's program makes a libcall, if it is our interest, intercept it and send to helper thread
	Whenever Snoopy's program makes a system call, intercept it and send to helper thread

	Helper thread will then either perform correct actions or communicate with the server on behalf of the Snoopy's program

	4. If necessayr, helperPid waits for the server's response and execute according to the server's response inside the interceptor
	*/

	if (opt_d >= 1) {
		char *ldPath = getenv("LD_PRELOAD");
		fprintf(stderr, "LD_PRELOAD=%s\n", ldPath);
	}

	// setenv("LD_PRELOAD", HOOK_LIB_PATH, 1);

	int childPid = -1, status = -1;

	/*Snoopy Program*/
	if ( (childPid = Fork()) == 0) {
		DEBUG(1, "%s\n", "Child process starting...\n");

		int ret = setenv("LD_PRELOAD", HOOK_LIB_PATH, 1);
		if (ret == -1)
			perror("setenv");

		// When input data is passed on to execv()
		// Trace for syscalls
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);

		//Child process execl
		//execv(exePath, exeArgs);
		//runGrep();
		//runCp();
		//runStat();
		runEcho();
	}
	else {

		// Communicate with the server for input data
		// Note. User of gridD decides which program to run, not the Snoopy
		long orig_eax, eax;
		int insyscall = 0;
		long params[3];

		while (1) {
			Waitpid(childPid, &status, 0);

			DEBUG(2, "Child process(pid=%d): status: %d\n", childPid, status);
			if (WIFEXITED(status))
				break;
			orig_eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * ORIG_EAX, NULL);
			DEBUG(2, "Intercepted %ld\n", orig_eax);

			if (orig_eax == SYS_write) {
				if (insyscall == 0) {
					/* Syscall entry */
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EBX, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, childPid, 4 * ECX, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EDX, NULL);
					DEBUG(1, "Write called with %ld, %ld, %ld\n", params[0], params[1], params[2]);
				}
				else { /* Syscall exit */
					eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * EAX, NULL);
					DEBUG(2, "Write returned with %ld\n", eax);
					insyscall = 0;
				}
			}
			else if (orig_eax == SYS_read) {
				if (insyscall == 0) {
					/* Syscall entry */
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EBX, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, childPid, 4 * ECX, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EDX, NULL);
					DEBUG(1, "Read called with %ld, %ld, %ld\n", params[0], params[1], params[2]);
				}
				else { /* Syscall exit */
					eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * EAX, NULL);
					DEBUG(2, "Read returned with %ld\n", eax);
					insyscall = 0;
				}
			}
			else if (orig_eax == SYS_open) {
				if (insyscall == 0) {
					/* Syscall entry */
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EBX, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, childPid, 4 * ECX, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EDX, NULL);
					DEBUG(1, "Open called with %ld, %ld, %ld\n", params[0], params[1], params[2]);
				}
				else { /* Syscall exit */
					eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * EAX, NULL);
					DEBUG(2, "Open returned with %ld\n", eax);
					insyscall = 0;
				}
			}
			else if (orig_eax == SYS_read) {
				if (insyscall == 0) {
					/* Syscall entry */
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EBX, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, childPid, 4 * ECX, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EDX, NULL);
					DEBUG(1, "Close called with %ld, %ld, %ld\n", params[0], params[1], params[2]);
				}
				else { /* Syscall exit */
					eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * EAX, NULL);
					DEBUG(2, "Close returned with %ld\n", eax);
					insyscall = 0;
				}
			}
			else {
				DEBUG(1, "Child made syscall #%ld\n", orig_eax);
			}
			ptrace(PTRACE_SYSCALL, childPid, NULL, NULL);

			// 0. Intercept Syscall and libcalls
			// 1. Communicates with the Snoopy's program
			// 2. Communicates with the gridServer
			// So this needs two bidirectional connections
		}
	}

	// wait for all child processes
	Waitpid(childPid, &status, 0);

	DEBUG(1, "Child process(pid=%d) ended with status=%d\n", childPid, status);


	exit(EXIT_SUCCESS);
}

static void runGrep(void) {
	char* argList[6] = {"/bin/grep", "--include=\\*.{c,h}", "-rnw", "-e", "void", NULL};
	printf("Calling: %s %s %s %s %s\n\n", argList[0], argList[1], argList[2], argList[3], argList[4]);
	execv(argList[0], argList);
}

static void runCp(void) {
	char* argList[4] = {"/bin/cp", "rwSample.out", "~/Documents", NULL};
	printf("Calling: %s %s %s\n\n", argList[0], argList[1], argList[2]);
	printf("Error %d", execv(argList[0], argList));
}

static void runStat(void) {
	char* argList[3] = {"/usr/bin/stat", "~/Documents/", NULL};
	printf("Calling: %s %s\n\n", argList[0], argList[1]);
	printf("Error %d", execv(argList[0], argList));
}

static void runEcho() {
	char* argList[5] = {"/bin/echo", "hello", ">", "tmp.txt", NULL};
	printf("Calling: %s %s %s %s\n\n", argList[0], argList[1], argList[3], argList[4]);
	printf("Error %d", execv(argList[0], argList));
}

static int install_syscall_filter(void) {
	struct sock_filter filter[] = {
		/* Validate architecture. */
		VALIDATE_ARCHITECTURE,
		/* Grab the system call number. */
		EXAMINE_SYSCALL,
		/* List allowed syscalls. */
		ALLOW_SYSCALL(rt_sigreturn),
#ifdef __NR_sigreturn
		ALLOW_SYSCALL(sigreturn),
#endif
		ALLOW_SYSCALL(exit_group),
		ALLOW_SYSCALL(exit),
		// ALLOW_SYSCALL(read),
		// ALLOW_SYSCALL(write),
		// ALLOW_SYSCALL(open),
		//KILL_PROCESS, @ Daniel I think this was a major problem.  It was killing any process who used a syscall that wasn't declared allowed.  Now it is done where it allows everything UNLESS Blacklisted.
		// use BLACKLIST(name_of_syscall) to blacklist a syscall
		BLACKLIST(exit),
		RETURN_ALLOW,
	};
	struct sock_fprog prog = {
		.len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
		.filter = filter,
	};

	if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("prctl(NO_NEW_PRIVS)");
		goto failed;
	}
	if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
		perror("prctl(SECCOMP)");
		goto failed;
	}
	return 0;

failed:
	if (errno == EINVAL)
		fprintf(stderr, "SECCOMP_FILTER is not available. :(\n");
	return 1;
}
