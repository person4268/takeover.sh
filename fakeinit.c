#define _XOPEN_SOURCE 700
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	sigset_t set;
	int status, i;

       if (getpid() != 1) {
		printf("i yearn to be PID1\n");
		return 1;
       }

	printf("closing all file handles we got\n");
	for (i = 3; i < 1024; i++)
		close(i);
        printf("so that's done\n");


	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, 0);


	printf("time to kill all processes!\n");
	kill(-1, SIGKILL);
	printf("now giving you a bash");
	int pid = fork();
	if(pid == 0) {
		char* newargv[] = { "/bin/bash", NULL };
		char* newenviron[] = { NULL };
		execve("/bin/bash", newargv, newenviron);
		perror("execve :( sorry, goodbye\n");
	}

	for (;;) wait(&status);
}

