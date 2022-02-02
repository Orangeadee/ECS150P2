#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

/* TODO */
enum state {ready,running,blocked,zombie,exited};

struct TCB {
	uthread_t tid;
	enum state s;
	uthread_ctx_t save_ctx;
	void *stack;
	bool pre;
	queue_t ready_q;
	queue_t zombie_q;
	queue_t blocked_q;
};

struct TCB *curr;

int uthread_start(int preempt)
{

	/* TODO */
	struct TCB *thread = (struct TCB*)malloc(sizeof(struct TCB));
	assert(thread);

	thread->ready_q = queue_create();
	thread->zombie_q = queue_create();
	thread->blocked_q = queue_create();
	thread->tid = 0;
	thread->s = ready;
	thread->stack = NULL;

	if(preempt == 1)
	{
		preempt_enable();
		thread->pre = true;	
	}
	else
	{
		preempt_disable();
		thread->pre = false;
	}
	curr = thread;
	return 0;
}

int uthread_stop(void)
{
	/* TODO */
	if(curr->pre == true)
	{
		preempt_stop();
	}
	return -1;
}

int uthread_create(uthread_func_t func)
{
	/* TODO */
	return -1;
}

void uthread_yield(void)
{
	/* TODO */
}

uthread_t uthread_self(void)
{
	/* TODO */
	return -1;
}

void uthread_exit(int retval)
{
	/* TODO */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO */
	return -1;
}

