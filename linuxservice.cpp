/*
 * linuxservice.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: root
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void startservice()
{
	/* Our process ID and Session ID */
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
			exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	   we can exit the parent process. */
	if (pid > 0) {
			exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Open any logs here */

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
	}



	/* Change the current working directory */
	if ((chdir("/")) < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
	}

//	/* Close out the standard file descriptors */
//	close(STDIN_FILENO);
//	close(STDOUT_FILENO);
//	close(STDERR_FILENO);

	/* Daemon-specific initialization goes here */
}

void redirect_stdout(const char* logfile_pathname)
{
	close(STDIN_FILENO);
	if (open("/dev/null",O_RDONLY) == -1) {
	    printf("failed to reopen stdin while daemonising (errno=%d)",errno);
	}
	int logfile_fileno = open(logfile_pathname,O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR|S_IRGRP);
	if (logfile_fileno == -1) {
		printf("failed to open logfile (errno=%d)",errno);
	}
	//dup2(logfile_fileno,fileno(stdout));
	//dup2(logfile_fileno,fileno(stderr));
	if(freopen(logfile_pathname, "a+", stdout) == NULL) printf("freopen stdout fail");
	if(freopen(logfile_pathname, "a+", stderr) == NULL) printf("freopen stderr fail");

	//close(logfile_fileno);
}
