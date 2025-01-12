//*****************************************************************************

// test_queue.c  -   unit test for queue.c

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

//*****************************************************************************

void test_init_queue() {
    queue q;
    init_queue(&q, 5);
    assert(q.calls != NULL);                // Check memory for calls is allocated (Not NULL)
    assert(q.front == 0);
    assert(q.rear == 0);
    assert(q.capacity == 5);
    free_queue(&q);
    printf("\ntest_init_queue passed!\n");
}

void test_expand_queue() {
    queue q;
    init_queue(&q, 5);
    expand_queue(&q);
    assert(q.calls != NULL);
    assert(q.capacity == 5*2);
    free_queue(&q);
    printf("test_expand_queue passed!\n");
}

void test_isFull() {
    queue q;
    init_queue(&q, 5);
    assert(isFull(&q) == false);

    for (int i=0; i<5; i++) {
        q.calls[q.rear++] = i;      // Add elements to the queue
    }
    assert(isFull(&q) == true);
    free_queue(&q);
    printf("test_isFull passed!\n");
}

void test_isEmpty() {
    queue q;
    init_queue(&q, 5);
    assert(isEmpty(&q) == true);

    for (int i=0; i<5; i++) {
        q.calls[q.rear++] = i;      // Add elements to the queue
    }
    assert(isEmpty(&q) == false);

    for (int i=0; i<5; i++) {
        q.front++;                  // Remove elements from the queue
    }
    assert(isEmpty(&q) == true);
    free_queue(&q);
    printf("test_isEmpty passed!\n");
}

void test_add_request_to_queue() {
    queue q;
    init_queue(&q, 5);

    add_request_to_queue(&q, 1);        // Add only one request to the queue
    assert(q.calls[0] == 1);
    assert(q.rear == 1);

    for (int i=2; i<=5; i++) {          // Fill queue to capacity
        add_request_to_queue(&q, i);
    }
    assert(q.rear == 5);

    add_request_to_queue(&q, 6);        // Queue full, expansion occurs
    assert(q.capacity == 10);           // Capacity doubled
    assert(q.calls[5] == 6);            // 6th request added
    assert(q.rear == 6);
    free_queue(&q);
    printf("test_add_request_to_queue passed!\n");
}

void test_get_request_from_queue() {
    queue q;
    int next_floor;
    init_queue(&q, 5);

    get_request_from_queue(&q, &next_floor);
    assert(next_floor == STOP);                 // queue is empty

    add_request_to_queue(&q, 3);
    add_request_to_queue(&q, 5);
    add_request_to_queue(&q, 7);

    get_request_from_queue(&q, &next_floor);
    assert(next_floor == 3);
    assert(q.front == 1);

    get_request_from_queue(&q, &next_floor);
    assert(next_floor == 5);
    assert(q.front == 2);

    get_request_from_queue(&q, &next_floor);
    assert(next_floor == 7);
    assert(q.front == 3);

    get_request_from_queue(&q, &next_floor);    // All requests retrieved
    assert(next_floor == STOP);                 // queue is empty again

    free_queue(&q);
    printf("test_get_request_from_queue passed!\n");
}

void test_free_queue() {
    queue q;
    init_queue(&q, 5);

    add_request_to_queue(&q, 1);
    add_request_to_queue(&q, 3);

    free_queue(&q);

    assert(q.calls == NULL);
    assert(q.front == 0);
    assert(q.rear == 0);
    assert(q.capacity == 0);

    printf("test_free_queue passed!\n"); 
}
