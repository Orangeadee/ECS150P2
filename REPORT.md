# ECS150 Project2

## Authors:
Huilin Zhang, He Li

## Table of Contents
- Phase 1
    - Queue API
    - Makefile
    - Testing
- Phase 2
    - Private Data Structure
    - Uthread API 
- Phase 3
    - Uthread_join()
    - Testing
- Phase 4
    - Preemption
    - Testing

### Phase 1 - Queue API
In the first part of the project, a link list is implemented and it contains
the current node's data and a node pointer that points to the next node. Then
in a queue, we use two node pointers to keep track of the top and bottom of 
the heap. Besides that, we will also keep track of the size of the queue. Since
the queue is FIFO, using a linked list tracking the earliest and the oldest 
data will be easy to implement `queue_delete()` function. We can delete a data
by making the previous pointer points to the next node instead of pointing to
the current node. At the same time, it would also be simple for functions 
`queue_dequeue()` and `queue_enqueue()`. For dequeue, we remove the earliest
data in the queue. And we enqueue the data at the end of queue (`@end`). In
order to destroy the queue successfully, checking the queue is empty will be
essential. After the queue passes several conditions, we will `free()` the
queue entirely and it will be destroyed. In `queue_iterate()`, we iterate thru
the queue from top to bottom and extract data to the given function `func`.
Lastly, the size stored in the queue will be used in `queue_length()` to return
the size of the current queue.

###### Phase 1 - Makefile
Since we are suppose to create a static library archive `libuthread.a`, we will
be using the command `ar rcs` for archive creation. And `ojbs` contains all the
essential object files to create `libuthread.a`. We also include `-MMD` for 
dependency tracking and can be used to compose the generic rule for object
generation. Finally, a `clean` option to clear all the files created by `make`.

###### Phase 1 - Testing
In order to test the correctness of our queue API, we create `queue_tester.c`
and add as many usage senarios as possible. To test `destroy()`, we attempt to
destroy an empty queue and a non-empty queue. The result shows our queue passes
all the destroy test cases. For testing `dequeue()` and `enqueue`, we first add
a single data into the queue and successfully dequeue. Then, we enqueue a list
of data into queue and it also dequeue without errors. `iterate()` is tested by
deleting a given data in the queue. And it is also tested by finding a certain
data within the queue. If data is found, return 1, 0 otherwise. Fortunately,
our queue API passes all the test cases we created.

###### Phase 2 - Private Data Structure

