//*****************************************************************************

//  elevator.h  -   header files for the Elevator Control System

//*****************************************************************************

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stdbool.h>
#include <ctype.h>

#define STOP -1
#define REQUEST_POSITION 0
#define MIN_FLOOR 1
#define MAX_FLOOR 4
#define SIZE 100
#define UP -2
#define DOWN -3

//*****************************************************************************

// Queue structure
typedef struct {            // The queue has these properties
    int *calls;             // Dynamically allocated array
    int front;              // Front index
    int rear;               // Rear index
    int capacity;           // Current capacity of the queue
} queue;

//*****************************************************************************

// Function declarations for queue operations
void init_queue(queue* q, int initial_capacity);
void expand_queue(queue* q);
int peek(queue* q);
bool isFull(queue* q);
bool isEmpty(queue* q);
void add_request_to_queue(queue* q, int floor);
void get_request_from_queue(queue* q, int *next_floor);
void free_queue(queue* q);

// Function declarations for elevator operations
void elevator_main_control(queue* q);
int read_user_input(char *buffer, size_t size);
void floor_request_handler(queue* q, int *flag, int *last_call, int *RC);
void process_requests_batch(queue *q, int *array, int count);
void run_the_elevator(queue* q, int *end_simulation, int *last_floor);
void move_the_elevator(int next_floor, int *last_floor);

// Function declarations for display
void printQueue(queue* q);
void display_elevator(int current_floor);
void display_elevator_status(int *last_floor, int next_floor);
void display_elevator_motion(int *last_floor, int next_floor);

// Utility functions
int is_valid_request(int current_floor, int target_floor);
void sort_ascend(int arr[], int n);
void sort_descend(int arr[], int n);

#endif

//*****************************************************************************