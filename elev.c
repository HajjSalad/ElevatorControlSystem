// Program: 4-Floor Elevator Control System

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define STOP -1
#define REQUEST_POSITION 0
#define MIN_FLOOR 1
#define MAX_FLOOR 4

// Queue structure
typedef struct {
    int *calls;             // Dynamically allocated array
    int front;              // Front index
    int rear;               // Rear index
    int capacity;           // Current capacity of the queue
} queue;

// Function Declarations
void init_queue(queue* q, int initial_capacity);
void expand_queue(queue* q);
int peek(queue* q);
bool isFull(queue* q);
bool isEmpty(queue* q);
void enqueue(queue* q, int current_floor, int target_floor);
void dequeue_pair(queue* q, int *current_floor, int *target_floor);
void printQueue(queue* q);
void free_queue(queue* q);

void display_elevator(int current_floor);
void display_elevator_status(int current_floor, int target_floor);
void display_elevator_motion(int current_floor, int target_floor);
void next_floor_request(int next_floor, int other_next);
void elevator_requests(queue* q, int *flag, int *last_call);
void elevator_control(queue* q);
void run_elevator(queue* q, int *flag, int *last_call);

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

// Expand the queue by doubling when its full - I think its good for scalability
// Will consider circular queue in a constrained environment
void expand_queue(queue* q) {
    int new_capacity = q->capacity*2;
    int *new_calls = (int*)realloc(q->calls, new_capacity*sizeof(int));
    if (new_calls == NULL) {
        fprintf(stderr, "Reallocation failed");
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
void enqueue(queue* q, int current_floor, int target_floor) {
    /*
    if (isFull(q)) {
        printf("The elevator is busy now. Please wait.\n");
        return;
    } */
    if (q->rear +1 == q->capacity) {       // Expand the queue if full
        expand_queue(q);
    }
    q->calls[q->rear++] = current_floor;        // Store current floor
    q->calls[q->rear++] = target_floor;         // Store target floor
}

// Get element from the queue
void dequeue_pair(queue* q, int *current_floor, int *target_floor) {
    if (isEmpty(q)) {
        //printf("No more requests.");
        *current_floor = STOP;
        *target_floor = STOP;
        return;
    }
    *current_floor = q->calls[q->front++];
    *target_floor = q->calls[q->front++];                         
}

// Peek at the element at the front
int peek(queue* q) {
    if (isEmpty(q)) {
        //printf("Queue is empty\n");
        return -1;
    }
    return q->calls[q->front];
}

// Queues the requests in the queue
void elevator_requests(queue* q, int *flag, int *last_call) {
    char line[256];
    
    printf("Request floor on and target floor (ex. '1 4, 2 3'): ");
    // Read user input line
    if (fgets(line, sizeof(line), stdin)) {
        // check if input is empty
        if (strlen(line) == 1 && line[0] == '\n') {
            printf("Input is empty. Enter a valid floor request.\n");
            return;
        }
        line[strcspn(line, "\n")] = '\0';               // Remove trailing newline if present

        // Tokenize to get the pairs of requests
        char *token = strtok(line, ",");                // Gets you the first token

        while (token != NULL) {                         // Go through the rest of the tokens
            int current_floor, target_floor;

            while (isspace((unsigned char)*token)) token++;             // Skip the spaces
            // Process current and target floor
            if (sscanf(token, "%d %d", &current_floor, &target_floor) == 2){        // Formatted input
                if (current_floor == STOP || target_floor == STOP) {              // Check stop condition
                    printf("End of Simulation. Exiting...\n");
                    *flag = 0;
                    return;
                }
                if (current_floor >= MIN_FLOOR && current_floor <= MAX_FLOOR &&
                    target_floor >= MIN_FLOOR && target_floor <= MAX_FLOOR &&
                    current_floor != target_floor) {                    // Valid floor range
                    enqueue(q, current_floor, target_floor);            // Queue the request
                } else {
                    printf("Invalid floor request. Please enter 1-4.\n");
                }
            } else {
                printf("Invalid input. Please enter two valid numbers only.\n");
            }
            token = strtok(NULL, ",");
        }
    } else {
        printf("Error reading input. Please try again.\n");
    }
}

// Runs the elevator according to the requests in the Queue
void run_elevator(queue* q, int *flag, int *last_call) {
    if(!*flag) return;

    int current_floor, target_floor;

    if (current_floor == 0) {
        display_elevator(*last_call);
    }         

    /*// If there's only one request in the queue
    if(isEmpty(q)) {
        next_floor_request(current_floor, -1);
        *last_call = current_floor;
        return;
    }*/

    // If multiple requests in the queue
    while (!isEmpty(q)) {
        dequeue_pair(q, &current_floor, &target_floor);

        if (current_floor == STOP || target_floor == STOP) {
            printf("Simulation Ended.\n");
            *flag = 0;
        }

        next_floor_request(current_floor, target_floor);           // Process the next floor request
    }

    // Handle the last request - while loop wont run because queue is empty
    next_floor_request(current_floor, -1);
    *last_call = current_floor;    
}

// Print the elevator display
void display_elevator(int current_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
    if (current_floor == REQUEST_POSITION) return;

    printf("\n");
    for (int i=3; i>=0; i--) {
        if (i+1 == current_floor) {
            printf("%s [E] <- Elevator is here\n", floors[i]);
        } else {
            printf("%s [ ]\n", floors[i]);
        }
    }
    printf("\n");
}

// Shows the visual status of the elevator
void display_elevator_status(int current_floor, int target_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
	// Display the status for each floor
    for (int i=3; i>=0; i--) {
        if (i+1 == current_floor) {
            // Determine elevator movement direction based on target_floor
            if (current_floor < target_floor) {
                printf("%s [E] - Going up\n", floors[i]);
            } else if (current_floor > target_floor) {
                printf("%s [E] - Going down\n", floors[i]);
            } else {
                printf("%s [E] - Arrived\n", floors[i]);
            }
        } else if (i +1 == target_floor) {
            printf("%s [ ] - Next stop\n", floors[i]);
        } else {
            printf("%s [ ]\n", floors[i]);
        }
    }
}

// Show the intermediate floors the elevator passes through
void display_elevator_motion(int current_floor, int target_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
	// Elevator moving from one floor to another
    if (current_floor < target_floor) {
        printf("\nElevator is moving up...\n");
        for (int i = current_floor; i < target_floor; i++) {
            printf("Arrived at %s\n", floors[i]);
        }
    } else if (current_floor > target_floor) {
        printf("\nElevator is moving down...\n");
        for (int i = current_floor - 1; i >= target_floor; i--) {
            printf("Arrived at %s\n", floors[i]);
        }
    }
}
// Move the elevator to the next requested floor
void next_floor_request(int current_floor, int target_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
    printf("\n");

    // Elevator idle and no target floor is specified
    if (target_floor == -1) {
        return;
    }

    display_elevator_status(current_floor, target_floor);
    
    display_elevator_motion(current_floor, target_floor);
    
    printf("Elevator has arrived at %s\n", floors[target_floor - 1]);
    
    display_elevator(target_floor);         // Update the graphical rep of elevator's current floor
}

void elevator_control(queue* q) {
    int flag = 1;
    int starting_floor = 1;
    int last_call = 0;
    // Initial prompt
    printf("\n\n\t*** Start of Simulation ***\n");
    printf("You may request 1 or more floors separated by space.\n");
    printf("Select Floor 1-4 (Enter -1 to stop; 0 for position of Elevator).\n\n");
    display_elevator(starting_floor);

    while (flag) {
        elevator_requests(q, &flag, &last_call);            // Queue the requests into the queue
        run_elevator(q, &flag, &last_call);                 // Operate the elevator
    }
}

// Print the current queue
void printQueue(queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Current Queue: ");
    for (int i = 0; i < q->rear; i++) {
        printf("%d ", q->calls[i]);
    }
    printf("\n");
}

void free_queue(queue* q) {
    free(q->calls);
    q->calls = NULL;
    q->front = 0;
    q->rear = 0;
    q->capacity = 0;
}

int main() {
    queue q;
    init_queue(&q, 4);              // Starts with initial capacity of 4

    elevator_control(&q);

    free_queue(&q);

    return 0;
}