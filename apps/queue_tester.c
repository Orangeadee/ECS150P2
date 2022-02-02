#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Destroy */
void test_destroy(void)
{
	queue_t q = queue_create();
	fprintf(stderr, "*** TEST destroy ***\n");
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* More Enqueue/Dequeue */
void test_queue_more(void)
{
	int data[] = {10,2,13,34,22,8}, *ptr;
	queue_t q;
	int i = 0;

	fprintf(stderr, "*** TEST queue_more ***\n");

	q = queue_create();
	for(i = 0; i < 6; i++)
	{
		queue_enqueue(q,&data[i]);
	}

	TEST_ASSERT(queue_destroy(q) == -1);

	for(i = 0; i < 6; i++)
	{
		queue_dequeue(q,(void**)&ptr);
	}
	TEST_ASSERT(ptr == &data[i-1]);
	TEST_ASSERT(queue_destroy(q) == 0);

	int *ptr2 = NULL;
	int retval = queue_enqueue(q,ptr2);
	TEST_ASSERT(retval == -1);
}

/* Enqueue/Dequeue complicate */
void test_queue_complicate(void)
{
	char sentence[] = "Hello World!", *ptr;
	queue_t q;
	int i = 0;
	
	fprintf(stderr, "*** TEST queue_complicate ***\n");

	q = queue_create();
	for(i = 0; i < (int)strlen(sentence); i++)
	{
		queue_enqueue(q,&sentence[i]);
	}

	for(i = 0; i < (int)strlen(sentence); i++)
	{
		queue_dequeue(q,(void**)&ptr);
	}
	TEST_ASSERT(ptr == &sentence[i-1]);
}

/* Delete queue */
void test_queue_delete(void)
{
	int data[] = {1,2,3,4,5,6}, *ptr;
	queue_t q;
	int i = 0;

	fprintf(stderr, "*** TEST queue_delete ***\n");

	q = queue_create();
	for(i = 0; i < (int)(sizeof(data)/sizeof(data[0])); i++)
	{
		queue_enqueue(q,&data[i]);
	}

	ptr = NULL;
	TEST_ASSERT(queue_delete(q,&data[1]) == 0);
	TEST_ASSERT(queue_delete(q,&data[1]) == -1);

	TEST_ASSERT(queue_delete(q,ptr) == -1);
	int data2 = 7;
	TEST_ASSERT(queue_delete(q,&data2) == -1);
}

/* ------ Iterate Queue ------ */
static int inc_item(queue_t q, void *data, void *arg)
{
	int *a = (int*)data;
	int inc = (int)(long)arg;

	if(*a == 10)
		queue_delete(q,data);
	else
		*a += inc;
	return 0;
}

static int find_item(queue_t q, void *data, void *arg)
{
	int *a = (int*)data;
	int match = (int)(long)arg;
	(void)q;

	if(*a == match)
		return 1;
	return 0;
}

void test_queue_iter(void)
{
	queue_t q;
	int data[] = {1,2,3,4,5,10,6,7,8,9}, *ptr;
	size_t i;

	q = queue_create();
	for(i = 0; i < sizeof(data)/sizeof(data[0]); i++)
	{
		queue_enqueue(q,&data[i]);
	}

	queue_iterate(q,inc_item,(void*)1,NULL);
	TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(queue_length(q) == 9);

	ptr = NULL;
	queue_iterate(q,find_item,(void*)5,(void**)&ptr);
	TEST_ASSERT(ptr != NULL);
	TEST_ASSERT(*ptr == 5);
	TEST_ASSERT(ptr == &data[3]);
}
int main(void)
{
	test_create();
	test_destroy();
	test_queue_simple();
	test_queue_more();
	test_queue_complicate();
	test_queue_delete();
	test_queue_iter();
	return 0;
}
