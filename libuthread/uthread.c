#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

/* TODO */

/* 
 *								*
 * Initialization:						*
 * - Following the prompt, create a type of data structure	*
 *   called @TCB to store all informations that a thread must	*
 *   contain.							*
 * - The state @s stores the current status of a thread		*
 *								*
 */
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

/* Record the current running thread */
struct TCB *curr;

/*
 *								*
 * uthread_start:						*
 * - Allocating spaces for a new thread and initialize all the	*
 *   components inside a TCB.					*
 * - If preempt is true, enable the preempt function and record	*
 *   @preemptive as true, otherwise disable the preempt		*
 * - Since we just started a new thread, its information will	*
 *   be sotred in @curr as current running thread		*
 *								*
 */
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
	struct TCB *create_thread = (struct TCB*)malloc(sizeof(struct TCB));
	if(create_thread == NULL || curr->tid >= USHRT_MAX)
	{
		return -1;
	}
	curr->tid++;
	create_thread->tid = curr->tid;
	create_thread->stack = uthread_ctx_alloc_stack();
	create_thread->s = ready;
	int status = uthread_ctx_init(&create_thread->save_ctx,create_thread->stack,func);
	if(status == 0)
	{
		assert(queue_enqueue(curr->ready_q,create_thread));
		return create_thread->tid;
	}
	return -1;
}

void uthread_yield(void)
{
	/* TODO */
}

/*
 * 						*
 * uthread_self:				*
 * - return the tid of current running thread 	*
 *						*
 */
uthread_t uthread_self(void)
{
	/* TODO */
	return curr->tid;
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

