#ifndef CRASH_MSG_H_
#define CRASH_MSG_H_

#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <signal.h>

struct crash_message_struct;

#define CRASH_MAX_PROCESS_NAME_SIZE (12)
#define CRASH_MAX_BACKTRACE_DEPTH (25)
#define CRASH_ASSERT_BUFFER_SIZE (128)
#define CRASH_MAX_MSG_SIZE (4096)
#define CRASH_MSG_MAGIC (0xdeadbeefUL)
#define CRASH_ANCILLARY_DATA_SIZE (CRASH_MAX_MSG_SIZE - sizeof(struct crash_message_struct))

/* Dead man switch: we wait this many seconds since first being tickled
 * until we continue processing crash, whether full crash information
 * has been received or not 
 * */
#define READ_TIMEOUT (10) 

void crashd_main(char daemonise_flag, const char * progname, int pfd[]);


struct crash_message_struct {
	/* Magic number */
	unsigned long magic;
	/* Process name as passed during registration */
	char process_name[CRASH_MAX_PROCESS_NAME_SIZE];
	/* The process PID */
	pid_t process_id;
	/* The process kernel thread id (struct task -> pid), NOT pthread_self() */
	pid_t thread_id; 
	/* The number of the exception signal */ 
	unsigned int signal_number;
	/* The signal code from siginfo_t. Provides exception reason */
	unsigned int signal_code;
	/* Fault address, if relevant */
	void * fault_address;
	/* The last error as reported via siginfo_t. Seems to be always 0 */
	unsigned int signal_errno;
	/* The last error in errno when the exception handler got called. */
	unsigned int handler_errno;
	/* Number of stack frames we got */
	size_t num_backtrace_frames;
	/* A time stamp */
	struct timespec timestamp;
	/* Buffer for assert data */
	unsigned char assert_buffer[CRASH_ASSERT_BUFFER_SIZE];
	/* Stack backtrace */
	void *backtrace[CRASH_MAX_BACKTRACE_DEPTH];
	/* Place holder for ancillary data, such as symbol trace sent as seperate message */
	char ancillary_data[0];
};

#endif /* CRASH_MSG_H_*/
