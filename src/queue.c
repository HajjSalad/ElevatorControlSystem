//*****************************************************************************

// queue.c  -   handles queue-related operations

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>
#include <stdlib.h>

//*****************************************************************************

// Initialize the queue
void init_queue(queue* q, int initial_capacity) {
    q->calls = (int*)malloc(initial_capacity*sizeof(int));          // Allocate memory dynamically
    if (q->calls == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    q->front = 0;
    q->rear = 0;
    q->capacity = initial_capacity;
}          

// Double queue to expand when its full - good for scalability
// Will consider circular queue in a constrained environment
void expand_queue(queue* q) {
    int new_capacity = q->capacity*2;
    int *new_calls = (int*)realloc(q->calls, new_capacity*sizeof(int));
    if (new_calls == NULL) {
        fprintf(stderr, "Reallocation failed for queue expansion");
        exit(EXIT_FAILURE);
    }
    q->calls = new_calls;
    q->capacity = new_capacity;
}

// Check whether the queue is full
bool isFull(queue* q) {
    return (q->rear == q->capacity);
}

// Check whether the queue is empty
bool isEmpty(queue* q) {
    return (q->front == q->rear);
}

// Insert the call into the queue
void add_request_to_queue(queue* q, int floor) {
    if (q->rear + 1 == q->capacity) {       // Expand the queue if full
        expand_queue(q);
    }
    q->calls[q->rear++] = floor;        // Store floor request
}

// Get element from the queue
void get_request_from_queue(queue* q, int *next_floor) {
    if (isEmpty(q)) {                   // If no more requests
        *next_floor = STOP;
        return;
    }
    *next_floor = q->calls[q->front++];                       
}

// Free the allocated memory
void free_queue(queue* q) {
    free(q->calls);
    q->calls = NULL;
    q->front = 0;
    q->rear = 0;
    q->capacity = 0;
}

//*****************************************************************************