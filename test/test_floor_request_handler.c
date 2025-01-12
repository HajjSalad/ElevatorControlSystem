//*****************************************************************************

//  test_floor_request_handler.c   -   unit test for floor_request_handler

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

//*****************************************************************************

void test_process_requests_batch() {
    queue q;
    init_queue(&q, 5);
    int array[] = {1,2,3,2,4};
    process_requests_batch(&q, array, 5);
    // Check size of the queue
    int size = (q.rear >= q.front) ? (q.rear - q.front) : (q.capacity - q.front + q.rear);
    assert(size == 4);
    // Check each element in the queue
    assert(q.calls[q.front] == 1);
    assert(q.calls[(q.front + 1) % q.capacity] == 2);
    assert(q.calls[(q.front + 2) % q.capacity] == 3);
    assert(q.calls[(q.front + 3) % q.capacity] == 4);
    free_queue(&q);
    printf("test_process_requests_batch passed!\n");
}



