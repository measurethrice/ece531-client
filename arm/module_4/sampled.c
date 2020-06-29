


#include <stdio.h>    //printf(3)
#include <stdlib.h>   //exit(3)
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h> //umask(3)
#include <syslog.h>   //syslog(3), openlog(3), closelog(3)
#include <errno.h>
#include <string.h>




#define OK			0
#define ERR_FORK	1
#define ERR_SETSID	2
#define ERR_CHDIR	3
#define ERR_WTF		4

const char ERROR_FORMAT; 


#define true		1

#define DAEMON_NAME "sampled"


//The signal handler
static void _signal_handler(const int signal) { 
	switch(signal) {
		case SIGHUP: 
			break; 
		case SIGTERM: 
			syslog(LOG_INFO, "recieved SIGTERM, exiting."); 
			closelog(); 
			exit(OK); 
			break; 
		default: 
			syslog(LOG_INFO, "recieved unhandled signal.");
	}
}

//May not always  want it static for testing
//Writing stuff to log file and sleep, then do it again
static void _do_work(void) {
	for (int i = 0; true; i++) {
		syslog(LOG_INFO, "iteration: %d", i); 
		sleep(1); 
	}
}

int main(void) {


	openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON); 
	syslog(LOG_INFO, "starting sampled");

	pid_t pid = fork(); 


	// pit_t pid; 
	// fork(); 
	// pid = getpid(); 

	if (pid < 0) {
		//syslog(LOG_ERR, ERROR_FORMAT, strerror(errno)); 
		syslog(LOG_ERR, "Couldn't fork: %s", strerror(errno)); 
		return ERR_FORK; 
	}

	if (pid > 0) { 
		return OK; 
	}

	if (setsid() < -1) {
		syslog(LOG_ERR, "Couldn't fork: %s", strerror(errno)); 
		//syslog(LOG_ERR, ERROR_FORMAT, strerror(errno)); 
		return ERR_SETSID;
	}

	close(STDIN_FILENO); 
	close(STDOUT_FILENO); 
	close(STDERR_FILENO); 

	//Might need to tighten this up for final release
	umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 

	if (chdir("/") < 0) { 
		syslog(LOG_ERR, "Couldn't change directory %s", strerror(errno)); 
		//syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
		return ERR_CHDIR;
	}

	signal(SIGTERM, _signal_handler); 
	signal(SIGHUP, _signal_handler); 

	_do_work(); 

	return ERR_WTF; 

} 

