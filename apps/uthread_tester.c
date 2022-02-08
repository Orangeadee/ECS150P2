#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

int thread5(void)
{
	printf("Reached thread5\n");
	return 55;
}

int thread4(void)
{
	uthread_create(thread5);
	printf("Rached thread4 and created thread5\n");
	uthread_yield();
	printf("Come back to thread%d\n",uthread_self());
	return 44;
}

int thread3(void)
{
	printf("Reached thread%d\n",uthread_self());
	uthread_yield();
	printf("Come back to thread%d\n",uthread_self());
	return 33;
}

int thread2(void)
{
	printf("Reached thread%d\n",uthread_self());
	uthread_yield();
	printf("Come back to thread%d\n",uthread_self());
	uthread_yield();
	uthread_exit(22);
	return 2;
}

int thread1(void)
{
	uthread_create(thread2);
	printf("Reached thread1 and created thread2\n");
	uthread_yield();
	printf("Come back to thread%d\n",uthread_self());
	return 111;
}

int main(void)
{
	printf("**** TEST uthread ****\n");
	uthread_t tid1, tid2;

	int retval1, retval2;
	uthread_start(0);
	tid1 = uthread_create(thread1);
	uthread_yield();
	tid2 = uthread_create(thread2);
	uthread_create(thread3);
	uthread_join(tid1,&retval1);
	uthread_yield();
	uthread_join(tid2,&retval2);
	uthread_stop();
	printf("retval1: %d\nretval2: %d\n",retval1,retval2);

	uthread_join(uthread_create(thread4),NULL);
	uthread_stop();
}
