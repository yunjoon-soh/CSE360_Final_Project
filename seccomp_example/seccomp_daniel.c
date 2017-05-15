#include "seccomp-bpf.h"
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/user.h>   // For constants ORIG_EAX etc

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

static int install_syscall_filter(void)
{
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
		ALLOW_SYSCALL(read),
		// ALLOW_SYSCALL(write),
		ALLOW_SYSCALL(open),
		KILL_PROCESS,
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

int main(int argc, char *argv[])
{
	char buf[1024];
	int status;

	// For seccomp
	// if (install_syscall_filter())
	// return 1;

	int childPid = -1;
	if ( (childPid = fork()) == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);

		execv("./a.out", NULL);

	} else {
		long orig_eax, eax;
		int insyscall = 0;
		long params[3];

		while (1) {
			waitpid(childPid, &status, 0);

			printf("Child process(pid=%d): status: %d\n", childPid, status);
			if (WIFEXITED(status))
				break;
			orig_eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * ORIG_EAX, NULL);
			printf("Intercepted %ld\n", orig_eax);

			if (orig_eax == SYS_write) {
				if (insyscall == 0) {
					/* Syscall entry */
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EBX, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, childPid, 4 * ECX, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, childPid, 4 * EDX, NULL);
					printf("Write called with %ld, %ld, %ld\n", params[0], params[1], params[2]);
				}
				else { /* Syscall exit */
					eax = ptrace(PTRACE_PEEKUSER, childPid, 4 * EAX, NULL);
					printf("Write returned with %ld\n", eax);
					insyscall = 0;
				}
			}
			else {
				printf("Child made syscall #%ld\n", orig_eax);
			}
			ptrace(PTRACE_SYSCALL, childPid, NULL, NULL);
		}
	}


	/*
		printf("Type stuff here: ");
		fflush(NULL);
		buf[0] = '\0';
		fgets(buf, sizeof(buf), stdin);
		printf("You typed: %s", buf);

		printf("And now we fork, which should do quite the opposite ...\n");
		fflush(NULL);
		sleep(1);

		fork();
		//printf("You should not see this because I'm dead.\n");
	*/
	return 0;
}
