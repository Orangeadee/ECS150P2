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

	que->begin = NULL;
	que->end = NULL;
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
	if(status == 0)
	{
		free(queue);
	}
	return status;
}

/*
 *
 * Enqueue:				*
 * - Allocate spaces for storing data	*
 *   into a new node pointer		*
 * - Use assert() to check validity of	*
 *   creating spaces for enter_node	*
 * - Check the validity of queue and 	*
 *   data, return -1 if empty		*
 * - If it's a new queue, set it points *
 *   to the begin of queue		*
 * - Otherwise set it points to the end *
 *   of queue				*
 * - Increment size of queue		*
 *					*
 */
int queue_enqueue(queue_t queue, void *data)
{
	/* TODO */
	struct node* enter_node = (struct node*)malloc(sizeof(struct node));
	assert(enter_node);
	if(queue == NULL || data == NULL)
	{
		return -1;
	}
	enter_node->data = data;
	enter_node->next = NULL;

	if(queue->size == 0)
	{
		queue->begin = enter_node;
		queue->end = enter_node;
	}
	else
	{
		queue->end->next = enter_node;
		queue->end = enter_node;
	}
	queue->size++;
	return 0;
}

/*
 * Dequeue:				*
 * - First check if @queue or @data are	*
 *   NULL, or queue is empty		*
 * - Then create a node and store the	*
 *   oldest data of queue		*
 * - Make the begin pointer points to	*
 *   the second oldest item in queue	*
 * - Oldest item successfully removed	*
 * - Decrement queue size and return 0	*
 * 					*
 */
int queue_dequeue(queue_t queue, void **data)
{
	/* TODO */
	if(queue == NULL || queue->size == 0)
	{
		return -1;
	}
	struct node* old_node = queue->begin;
	*data = old_node->data;
	if(queue->begin != queue->end)
	{
		queue->begin = queue->begin->next;
	}
	else
	{
		queue->begin = NULL;
		queue->end = NULL;
	}
	free(old_node);
	queue->size--;
	return 0;
}

/*
 *
 * Delete queue:			*
 * - First do error some error checks	*
 * - Loop through the queue to find the	*
 *   match data				*
 * - If data is found, save the next	*
 *   data in temp and make the curr ptr *
 *   points to the next data		*
 * - Done using temp node, delete it	*
 * - If data is found at the end of 	*
 *   queue, set the end ptr in queue to *
 *   curr ptr				*
 * - Decrement queue size, return 0	*
 * - If data is not found, make temp 	*
 *   points to current node and make 	*
 *   curr points to next node		*
 * - Return -1 if not found in queue	*
 * 					*   
 */
int queue_delete(queue_t queue, void *data)
{
	/* TODO */
	if(queue == NULL || queue->size == 0)
	{
		return -1;
	}
	struct node* curr = queue->begin;
	struct node* temp = NULL;
	while(curr != NULL)
	{
		if(curr->data == data)
		{
			temp = curr->next;
			if(curr == NULL)
			{
				return -1;
			}
			else
			{
				curr->data = temp->data;
				curr->next = temp->next;
				free(temp);
				if(curr->next == NULL)
				{
					queue->end = curr;
				}
				queue->size--;
				return 0;
			}
		}
		temp = curr;
		curr = curr->next;
	}
	return -1;
}

/*
 *					*
 * Iterate queue:			*
 * - Return -1 if the queue or the	*
 *   funtion is empty			*
 * - Create a new node for iteration	*
 *   temp is to check whether the queue	*
 *   has reached to the end		*
 * - While @itr still has next value,	*
 *   pass in queue, data in itr and its	*
 *   corresponding @arg into function	*
 * - If function success and return 1,	*
 *   save the returned data if it has	*
 *   one, then iteration success and 	*
 *   will return 0			*
 * - If function fail, increment itr	*
 *   and continue passing in datas into	*
 *   funtion				*
 * - Will return -1 if all data in itr	*
 *   fail in function			*
 *   					*
 */
int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	int status = -1;
	/* TODO */
	if(queue == NULL || func == NULL)
	{
		return status;
	}
	struct node* itr = queue->begin;
	struct node* temp = queue->begin->next;
	while(itr->next != NULL)
	{
		int val = (*func)(queue,itr->data,arg);
		if(val == 1)
		{
			if(data == NULL)
			{
				status = 0;
				break;
			}
			/* if data is not NULL, pass in the data */
			else
			{
				status = 0;
				*data = itr->data;
				break;
			}
		}
		else
		{
			if(temp->next == NULL)
			{
				itr = itr->next;
			}
			else
			{
				itr = itr->next;;
				temp = temp->next;
			}
		}

	}
	return status;
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

