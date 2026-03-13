#define _XOPEN_SOURCE 700
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

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
	// so now we gotta reopen 0, 1, 2 because getty is ded
	close(0); close(1); close(2);
	int fd = open("/dev/console", O_RDWR);
    	if (fd >= 0) {
        	dup2(fd, 0); // stdin
        	dup2(fd, 1); // stdout
        	dup2(fd, 2); // stderr
    	}

	printf("now giving you a bash\n");
	int pid = fork();
	if(pid == 0) {
printf("am child\n");
                sigset_t empty_set;
                sigemptyset(&empty_set);
                sigprocmask(SIG_SETMASK, &empty_set, NULL);

                setsid();
                ioctl(0, TIOCSCTTY, 1);

		char* newargv[] = { "/bin/bash", NULL };
		char* newenviron[] = { NULL };
		execve("/bin/bash", newargv, newenviron);
		perror("execve :( sorry, goodbye\n");
	}

	printf("am parent\n");
	for (;;) wait(&status);
}

