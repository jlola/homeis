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


int pidFilehandle;

/* Change this to whatever your daemon is called */
#define DAEMON_NAME "mydaemon"

/* Change this to the user under which to run */
#define RUN_AS_USER "daemon"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


static void child_handler(int signum)
{
    switch(signum) {
    case SIGALRM: exit(EXIT_FAILURE); break;
    case SIGUSR1: exit(EXIT_SUCCESS); break;
    case SIGCHLD: exit(EXIT_FAILURE); break;
    }
}

void daemonize2( const char *lockfile )
{
    pid_t pid, sid, parent;
    int lfp = -1;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Create the lock file as the current user */
    if ( lockfile && lockfile[0] ) {
        lfp = open(lockfile,O_RDWR|O_CREAT,0640);
        if ( lfp < 0 ) {
            syslog( LOG_ERR, "unable to create lock file %s, code=%d (%s)",
                    lockfile, errno, strerror(errno) );
            exit(EXIT_FAILURE);
        }
    }

    /* Drop user if there is one, and we were run as root */
//    if ( getuid() == 0 || geteuid() == 0 ) {
//        struct passwd *pw = getpwnam(RUN_AS_USER);
//        if ( pw ) {
//            syslog( LOG_NOTICE, "setting user to " RUN_AS_USER );
//            setuid( pw->pw_uid );
//        }
//    }

    /* Trap signals that we expect to recieve */
    signal(SIGCHLD,child_handler);
    signal(SIGUSR1,child_handler);
    signal(SIGALRM,child_handler);

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        syslog( LOG_ERR, "unable to fork daemon, code=%d (%s)",
                errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {

        /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
           for two seconds to elapse (SIGALRM).  pause() should not return. */
        alarm(2);
        pause();

        exit(EXIT_FAILURE);
    }

    /* At this point we are executing as the child process */
    parent = getppid();

    /* Cancel certain signals */
    signal(SIGCHLD,SIG_DFL); /* A child process dies */
    signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
    signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        syslog( LOG_ERR, "unable to create a new session, code %d (%s)",
                errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        syslog( LOG_ERR, "unable to change directory to %s, code %d (%s)",
                "/", errno, strerror(errno) );
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);

    /* Tell the parent process that we are A-okay */
    kill( parent, SIGUSR1 );
}

 void daemonize(/*char *rundir,*/const char *pidfile)
 {
        int pid, sid, i;
        char str[10];
        //struct sigaction newSigAction;
        //sigset_t newSigSet;

        /* Check if parent process id is set */
        if (getppid() == 1)
        {
            /* PPID exists, therefore we are already a daemon */
            return;
        }

        /* Fork*/
        pid = fork();

        if (pid < 0)
        {
            /* Could not fork */
            exit(EXIT_FAILURE);
        }

        if (pid > 0)
        {
            /* Child created ok, so exit parent process */
            printf("Child process created: %d\n", pid);
            exit(EXIT_SUCCESS);
        }

        /* Child continues */

        umask(027); /* Set file permissions 750 */

        /* Get a new process group */
        sid = setsid();

        if (sid < 0)
        {
            exit(EXIT_FAILURE);
        }

        /* close all descriptors */
        for (i = getdtablesize(); i >= 0; --i)
        {
            close(i);
        }

        /* Route I/O connections */

        /* Open STDIN */
        i = open("/dev/null", O_RDWR);

        /* STDOUT */
        dup(i);

        /* STDERR */
        dup(i);

        //chdir(rundir); /* change running directory */

        /* Ensure only one copy */
        pidFilehandle = open(pidfile, O_RDWR|O_CREAT, 0600);

        if (pidFilehandle == -1 )
        {
            /* Couldn't open lock file */
            syslog(LOG_INFO, "Could not open PID lock file %s, exiting", pidfile);
            exit(EXIT_FAILURE);
        }

        /* Try to lock file */
        if (lockf(pidFilehandle,F_TLOCK,0) == -1)
        {
            /* Couldn't get lock on lock file */
            syslog(LOG_INFO, "Could not lock PID lock file %s, exiting", pidfile);
            exit(EXIT_FAILURE);
        }


        /* Get and format PID */
        sprintf(str,"%d\n",getpid());

        /* write pid to lockfile */
        write(pidFilehandle, str, strlen(str));
    }

void daemonize()
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

	/* Close out the standard file descriptors */
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
