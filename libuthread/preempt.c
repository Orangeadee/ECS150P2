#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

void preempt_start(void)
{
	/* TODO */
	struct itimerval new;
  	new.it_interval.tv_usec = 10;
  	new.it_interval.tv_sec = 0;
  	new.it_value.tv_usec = 10;
  	new.it_value.tv_sec = 0;

	void termination_handler (int signum);
	signal (SIGVTALRM, termination_handler);
	setitimer (ITIMER_VIRTUAL, &new, NULL);
}

void preempt_stop(void)
{
	/* TODO */
	struct itimerval new;  
    new.it_interval.tv_usec = 0;
  	new.it_interval.tv_sec = 0;
  	new.it_value.tv_usec = 0;
  	new.it_value.tv_sec = 0;
    setitimer(ITIMER_VIRTUAL, &new, NULL);  
}

void preempt_enable(void)
{
	/* TODO */
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGVTALRM);
	sigprocmask (SIG_BLOCK, &set, NULL);
}

void preempt_disable(void)
{
	/* TODO */
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGVTALRM);
	sigprocmask (SIG_BLOCK, &set, NULL);
}

