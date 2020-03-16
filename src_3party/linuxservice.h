/*
 * linuxservice.h
 *
 *  Created on: Sep 1, 2014
 *      Author: root
 */

#ifndef LINUXSERVICE_H_
#define LINUXSERVICE_H_

void daemonize(const char *pidfile);
void daemonize();
void redirect_stdout(const char* logfile_pathname);

void daemonize2( const char *lockfile );

#endif /* LINUXSERVICE_H_ */
