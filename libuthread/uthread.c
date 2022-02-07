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
#include "queue.h"
#include "uthread.h"

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
	uthread_ctx_t *save_ctx;
	void *stack;
	bool pre;
	int *ret_val;
	int join_tid;
};


/* Record the current running thread  and current queue status */
struct TCB *curr;
queue_t ready_q;
queue_t zombie_q;
queue_t blocked_q;
int count_thread;

/* 
 * Inspired by example in queue_iterate() testers from prompt  	*
 * Aim is iterating the blocked queue until data is found	*
 * If data is not found, return -1. Otherwise return 1.		*
 */
static int find_data(queue_t q, void *data, void *arg)
{
	//printf("find data\n");
	// TCB instead of int because iterating thru threads
	struct TCB *thread_data = (struct TCB*)data;
	uthread_t match_tid = *((uthread_t*)arg);
	(void)q; //unused

	if(thread_data->tid == match_tid)
		return 1;
	//printf("data not match\n");
	return 0;
}
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
	//printf("start called\n");
	struct TCB *thread = (struct TCB*)malloc(sizeof(struct TCB));
	curr = (struct TCB*)malloc(sizeof(struct TCB));
	assert(thread);
	count_thread = 0;
	ready_q = queue_create();
	zombie_q = queue_create();
	blocked_q = queue_create();
	thread->tid = 0;
	thread->save_ctx = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
	//thread->save_ctx = context;
	thread->s = running;
	thread->stack = NULL;
	thread->join_tid = -1;

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
	count_thread++;
	return 0;
}

/*
 *								*
 * uthread_stop:						*
 * - Stop the multighreading library.				*
 * - Stop the preemption if it has one, then attempt to destroy	*
 *   the ready and blocked queue.				*
 * - Set the current running thread to main thread.		*
 * - Dequeue all data in zombie queue and free all the data	*
 *   including context.						*
 * - Once the zombie queue can be destroyed completely, free	*
 *   the current running thread and reset to NULL, return 0.	*
 * - If ready queue or blocked queue can't be destroyed at the	*
 *   moment, implies that the queue is not empty and not ready	*
 *   to destroy. Thus return -1.				*
 * 								*  
 */
int uthread_stop(void)
{
	/* TODO */
	printf("stop called!\n");
	if(curr->pre == true)
	{
		preempt_stop();
	}
	int des_readyQ = queue_destroy(ready_q);
	int des_blockedQ = queue_destroy(blocked_q);
	if(des_readyQ == 0 && des_blockedQ == 0)
	{
		printf("stop if\n");	
		curr->tid = 0; // main thread
		struct TCB *thread_to_destroy;
		int sta = queue_destroy(zombie_q);
		printf("can I destroy zombie? %d\n",sta);
		while(queue_destroy(zombie_q) == -1 || queue_length(zombie_q) > 0)
		{
			int len = queue_length(zombie_q);
			printf("len %d\n",len);
			thread_to_destroy = (struct TCB*)malloc(sizeof(struct TCB));
			queue_dequeue(zombie_q,(void**)&thread_to_destroy);
			uthread_ctx_destroy_stack(thread_to_destroy->stack);
			free(thread_to_destroy);
		}
		count_thread = 0;
		uthread_ctx_destroy_stack(curr->stack);
		free(curr);
		curr = NULL;
		printf("stop end\n");
		return 0;
	}
	//printf("stop error\n");
	return -1;
}

/*
 *								*
 * uthread_create:						*
 * - Create a new thread with its executing function.		*
 * - Initialize a new thread along with all the infos that a 	*
 *   thread requied to have.					*
 * - Then, pass in to @uthread_ctx_init() for context initiali-	*
 *   zation, return -1 if failed to create a new thread.	*
 * - If success, enqueue the created thread into ready-queue 	*
 *   and increment counts for threads, then return its tid.	*
 *								*
 */
int uthread_create(uthread_func_t func)
{
	/* TODO */
	printf("thread create called\n");
	struct TCB *create_thread = (struct TCB*)malloc(sizeof(struct TCB));
	if(create_thread == NULL || curr->tid >= USHRT_MAX)
	{
		return -1;
	}
	//curr->tid++;
	create_thread->tid = count_thread;
	create_thread->stack = uthread_ctx_alloc_stack();
	create_thread->save_ctx = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	create_thread->s = ready;
	create_thread->ret_val = 0;
	create_thread->join_tid = -1;
	int status = uthread_ctx_init(create_thread->save_ctx,create_thread->stack,func);
	if(status == 0)
	{
		assert(queue_enqueue(ready_q,create_thread) == 0);
		printf("count:%d create_thread tid:%d\n",count_thread,(int)create_thread->tid);
		count_thread++;
		return create_thread->tid;
	}
	count_thread--;
	return -1;
}

/*
 *								*
 * uthread_yield:						*
 * - Stop the current running thread and let other threads	*
 *   to execute.						*
 * - First reset the current thread state to ready instead of	*
 *   running.							*
 * - Then, if the current thread is the main thread, and there	*
 *   is no other threads remaining, no threads to yield.	*
 * - Otherwise, enter the current thread data to the end of the	*
 *   queue and dequeue the next earliest entered thread data	*
 *   and store in @next_t for future usage.			*
 * - Make 2 ctx ptrs that points to the current thread context	*
 *   and the next thread context, switch the context using	*
 *   @uthread_ctx_switch() function.				*
 * - Re-enable the preemption if there is one.			*
 *								*
 */
void uthread_yield(void)
{
	/* TODO */
	//printf("yield called\n");
	struct TCB *next_t = (struct TCB*)malloc(sizeof(struct TCB));
	if(curr->pre == true) preempt_disable();
	curr->s = ready;
	if(queue_length(ready_q) <= 0)
	{
		printf("yield if\n");
		return;
	}
	else
	{
		printf("yield else\n");
		struct TCB *prev_t = (struct TCB*)malloc(sizeof(struct TCB));
		prev_t = curr;
		queue_enqueue(ready_q,curr);
		queue_dequeue(ready_q,(void**)&next_t);
		printf("curr tid:%d\n",(int)curr->tid);	
		uthread_ctx_t *prev_ctx = prev_t->save_ctx;
		uthread_ctx_t *next_ctx = next_t->save_ctx;
		curr = next_t;
		curr->s = running;
		uthread_ctx_switch(prev_ctx,next_ctx);
	}
	if(curr->pre == true) preempt_enable();
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
	//printf("self called\n");
	return curr->tid;
}

/*
 *								*
 * uthread_exit:						*
 * - Exit from currently funning thread and should be turned	*
 *   into zombie state if not yet collected by its parent.	*
 * - Set the current running thread to zombie state and store 	*
 *   its return value. Then enqueue to the zombie queue.	*
 * - Iterate through blocked queue to see if there are any thr-	*
 *   eads left, if that is the case, delete the queue and then	*
 *   enqueue into ready queue waiting for execution.		*
 * - Otherwise, where there is no queue being blocked, we will	*
 *   dequeue from ready queue and perform context switch to it.	*
 *
 */
void uthread_exit(int retval)
{
	/* TODO */
	printf("exit called\n");
	curr->s = zombie;
	curr->ret_val = &retval;
	int join_id = curr->join_tid;
	struct TCB *prev = (struct TCB*)malloc(sizeof(struct TCB));
	prev = curr;
	struct TCB *temp = NULL; // for saving status in zombie_q iteration
	if(curr->pre == true) preempt_disable();
	queue_enqueue(zombie_q,(void*)curr);
	int status = queue_iterate(blocked_q,(queue_func_t)find_data,(void*)&join_id,(void**)&temp);
	if(temp == NULL || status == -1 || queue_length(blocked_q) == 0)
	{
		printf("exit: no block queue\n");
		queue_dequeue(ready_q,(void**)&curr);
		uthread_ctx_switch(prev->save_ctx,curr->save_ctx);
	}
	else
	{
		printf("exit: yes block queue\n");
		queue_enqueue(ready_q,(void*)temp);
		queue_delete(blocked_q,(void*)temp);
		queue_dequeue(ready_q,(void**)&curr);
		//printf("ctx switch exit, curr tid:%d\n",(int)curr->tid);
		uthread_ctx_switch(prev->save_ctx,curr->save_ctx);
		free(temp);
		//free(prev);
	}
	//printf("exit end\n");
	if(curr->pre == true) preempt_enable();
}

/*
 *								*
 * uthread_join:						*
 * - Joining a thread, where each thread can be joined by only	*
 *   one other thread.						*
 * - Given a tid, we are able to find the location of that th-	*
 *   read, wheter it is in zombie, blocked, or ready queue.	*
 * - By iterating through zombie queue first, if the given pid	*
 *   match one of the threads in zombie queue, we first delete	*
 *   the thread in zombie queue, then stop executing the curr-	*
 *   ent thread by using enqueue to ready queue.		*
 *   Once @curr has been enqueued successfully, we extract the	*
 *   oldest thread in ready queue and perform a context switch.	*
 * - If tid matches one of the threads in the blocked or ready	*
 *   queue, block the current running thread and perform a 	*
 *   context switch from current thread to the next oldest 	*
 *   thread in ready queue.					*
 *								*
 */
int uthread_join(uthread_t tid, int *retval)
{
	/* TODO */
	printf("join caleed\n");
	//printf("curr tid:%d\n",(int)curr->tid);
	if(tid == 0 || tid == curr->tid)
	{
		printf("join tid == 0 called\n");
		return -1;
	}

	/* check T2 is dead or not */
	curr->s = blocked;
	curr->join_tid = tid;

	struct TCB *prev = (struct TCB*)malloc(sizeof(struct TCB));
	struct TCB *next = (struct TCB*)malloc(sizeof(struct TCB));
	struct TCB *prev_t = (struct TCB*)malloc(sizeof(struct TCB));
	struct TCB *next_t = (struct TCB*)malloc(sizeof(struct TCB));
	struct TCB *temp = NULL; // for searching existence of the thread
	int inBlock;
	//printf("zombie\n");
	int inZombie = queue_iterate(zombie_q,(queue_func_t)find_data,(void*)&tid,(void**)&temp);
	if(curr->pre == true) preempt_disable();
	if(inZombie != -1 && temp != NULL)
	{
		printf("in zombie exit\n");
		/* T1 collect T2 right away */
		if(retval) curr->ret_val = retval;
		printf("retval %d\n",*(curr->ret_val));
		queue_delete(zombie_q,temp);
		free(temp->save_ctx);
		uthread_ctx_destroy_stack(temp->stack);
		free(temp);
		curr->s = ready;
		curr->join_tid = -1;
		queue_enqueue(ready_q,curr);
		printf("enqueue success\n");
		queue_dequeue(ready_q,(void**)&next_t);
		printf("dequeue success\n");
		prev_t = curr;
		curr = next_t;
		curr->s = running;
		uthread_ctx_switch(prev_t->save_ctx,next_t->save_ctx);
		if(curr->pre == true) preempt_enable();
		return 0;
	}
	//printf("block\n");
	inBlock = queue_iterate(blocked_q,(queue_func_t)find_data,(void*)&tid,(void**)&temp);
	//printf("ready\n");
	int inReady = queue_iterate(ready_q,(queue_func_t)find_data,(void*)&tid,(void**)&temp);
	//printf("inReady %d inB %d\n",inReady,inBlock);
	//printf("temp:%d\n",(int)temp->tid);
	if((inReady == -1 && inBlock == -1) || temp == NULL)
	{
		printf("join if\n");
		return -1;
	}
	else if(temp->join_tid != -1) // prevent from joining multiple threads
	{
	//	printf("join else if\n");
		return -1;
	}
	else
	{
		printf("join else\n");
		queue_enqueue(blocked_q,(void*)curr);
		curr->join_tid = temp->tid;
		curr->ret_val = retval;
		prev = curr;
		queue_dequeue(ready_q,(void**)&next);
		curr = next;
		curr->s = running;
		uthread_ctx_switch(prev->save_ctx,next->save_ctx);
		if(curr->pre == true) preempt_enable();
		free(temp);
		printf("join end\n");
		return 0;
	}
	//printf("join return -1\n");
	return -1;
}

