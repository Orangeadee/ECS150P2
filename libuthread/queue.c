#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node {
	void *data;
	struct node *next;
};

struct queue {
	/* TODO */
	struct node *begin;
	struct node *end;
	int size;
};

/*					
 *					*
 * Create a queue:			*
 * - Allocate memories for queue	*
 * - Use assert to check errors		*
 * - Initialize the beginning and 	*
 *    ending of the queue to NULL	*
 * - Return the created queue		*
 *					*
 */
queue_t queue_create(void)
{
	/* TODO */
	queue_t que = (queue_t)malloc(sizeof(struct queue));
	assert(que);

	que->begin = que->end = NULL;
	que->size = 0;
	return que;
}

/*
 *
 * Destroy a queue:
 * - Set up a variable to return	*
 * - First check if passed in an	*
 *   empty(NULL) queue			*
 * - Then check if the queue is		*
 *   not empty				*
 * - Last check if the size of		*
 *   the queue is 0 or not		*
 * - If any of the cases exist		*
 *   above, set return value to -1	*
 * - If not,set it to be 0		*
 *					*
 */
int queue_destroy(queue_t queue)
{
	/* TODO */
	int status = 0;
	if(queue == NULL)
	{
		status = -1;
	}
	if(queue->begin != NULL || queue->end != NULL)
	{
		status = -1;
	}
	if(queue->size > 0)
	{
		status = -1;
	}
	free(queue);
	return status;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO */
	return -1;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO */
	return -1;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO */
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO */
	return -1;
}

/*
 * 					*
 * Get queue length:			*
 * - First chekc if the queue is empty	*
 * - Then if the begin of queue is NULL	*
 *   return length of queue = 0		*
 * - If the begin of queue is NULL but	*
 *   the end is not empty, report error	*
 * - Finally return current queue size	*
 *					*
 */
int queue_length(queue_t queue)
{
	/* TODO */
	int length = 0;
	if(queue == NULL)
	{
		length = -1;
		return length;
	}
	if(queue->begin == NULL)
	{
		return length;
	}
	if(queue->begin != NULL && queue->end == NULL)
	{
		length = -1;
		return length;
	}
	length = queue->size;
	return length;
}

