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
void add_request_to_queue(queue* q, int current_floor, int target_floor);
void get_request_pair_from_queue(queue* q, int *current_floor, int *target_floor);
void printQueue(queue* q);
void free_queue(queue* q);

void display_elevator(int current_floor);
void display_elevator_status(int current_floor, int target_floor);
void display_elevator_motion(int current_floor, int target_floor);
void move_the_elevator(queue* q, int next_floor, int other_next, int *last_floor);
void manage_the_requests(queue* q, int *flag, int *last_call);
void elevator_main_control(queue* q);
void run_the_elevator(queue* q, int *flag, int *last_call);

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

// Double queue to expand when its full - I think its good for scalability
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
void add_request_to_queue(queue* q, int current_floor, int target_floor) {
    if (q->rear +1 == q->capacity) {       // Expand the queue if full
        expand_queue(q);
    }
    q->calls[q->rear++] = current_floor;        // Store current floor
    q->calls[q->rear++] = target_floor;         // Store target floor
}

// Get element from the queue
void get_request_pair_from_queue(queue* q, int *current_floor, int *target_floor) {
    if (isEmpty(q)) {                   // If no more requests
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
        for (int i = current_floor; i < target_floor-1; i++) {
            printf("Arrived at %s\n", floors[i]);
        }
    } else if (current_floor > target_floor) {
        printf("\nElevator is moving down...\n");
        for (int i = current_floor - 1; i > target_floor; i--) {
            printf("Arrived at %s\n", floors[i-1]);
        }
    }
}
// Move the elevator to the requested floor
void move_the_elevator(queue* q, int current_floor, int target_floor, int *last_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
    printf("\n");

    display_elevator_status(current_floor, target_floor);               // Show where the elev is and next stop
    
    display_elevator_motion(current_floor, target_floor);               // Show the arrival of the elev at each floor
    
    printf("Elevator has arrived at %s\n\n", floors[target_floor - 1]); // Show elev arrival at the target floor

    // Check if another request in the queue then transition to service it
    if (!isEmpty(q)) {
        int next_request_from = peek(q);

        if (next_request_from == REQUEST_POSITION) {        // Elev position requested
            return;                                         // No need to display the transition to next request
        } else if (next_request_from == STOP) {             // End simulation requested
            return;                                         // No need to display the transition to next request
        } else {
             display_elevator_status(target_floor, next_request_from);            // Update where the elev is and the next stop

            display_elevator_motion(target_floor, next_request_from);            // Update the motion while the elev goes to service the next request

            printf("Elevator has arrived at %s for next request\n\n", floors[next_request_from - 1]);  // Elev arrives to service the next request
        }       
    } else {        // If no next request
        *last_floor = target_floor;             // record the last floor visited - this is where the elev will idle
    }
}

// Runs the elevator according to the requests in the Queue
void run_the_elevator(queue* q, int *end_simulation, int *last_floor) {
    if(*end_simulation) return;     // Simulation terminated

    int current_floor;              // Floor you're requesting from
    int target_floor;               // Floor you wish to go
        
    // Service the request
    while (!isEmpty(q)) {
        get_request_pair_from_queue(q, &current_floor, &target_floor);   // Get the request from the queue

        if (current_floor == STOP || target_floor == STOP) {                // Check stop condition
            printf("Simulation Ended.\n\n");
            *end_simulation = 1;
            return;
        } else if (current_floor == REQUEST_POSITION || target_floor == REQUEST_POSITION) {     // Input queued = 0 0
            display_elevator(*last_floor);
            return;
        } else {
            printf("Move is done here");
            move_the_elevator(q, current_floor, target_floor, last_floor);      // Fulfil the request
        }
    }  
}

// Queues the requests in the queue
void manage_the_requests(queue* q, int *end_simulation, int *last_floor) {
    char line[256];
    
    printf("Request current floor and target floor (ex. '1 4, 2 3'): ");
    // Read user input line
    if (fgets(line, sizeof(line), stdin)) {
        
        if (strlen(line) <= 2) {                                // If only 1 or less input
            if (line[0] == '\n' || line[0] == '\0') {           // input is empty
                printf("Input is empty. Enter valid floor requests.\n");
                return;
            }
        }
    
        line[strcspn(line, "\n")] = '\0';               // Remove trailing newline if present

        // Tokenize to get the pairs of requests
        char *token = strtok(line, ",");                // Gets you the first token

        while (token != NULL) {                         // Go through the rest of the tokens
            int current_floor;                          // Floor you are requesting from
            int target_floor;                           // Floor you wish to going
            int position_or_end;                         // Position of the elevator
            while (isspace((unsigned char)*token)) token++;             // Skip the spaces

            // Process current and target floor
            if (sscanf(token, "%d %d", &current_floor, &target_floor) == 2){        // If token has 2 integers
                if (current_floor == STOP || target_floor == STOP) {                // Check stop condition
                    *end_simulation = 1;                                            // Terminate the simulation
                    printf("End of Simulation. Exiting...\n");
                    return;
                }

                if (current_floor >= MIN_FLOOR && current_floor <= MAX_FLOOR &&
                    target_floor >= MIN_FLOOR && target_floor <= MAX_FLOOR &&
                    current_floor != target_floor) {                                // Valid floor range
                    if ((target_floor - current_floor) > 0) {                       // Going up
                        printf("+ve\n");
                    } else if ((target_floor - current_floor) < 0) {                // Going down
                        printf("-ve\n");
                    }
                    add_request_to_queue(q, current_floor, target_floor);            // Queue the request
                } else if (current_floor == target_floor) {
                    printf("You are here");
                } else {
                    printf("Invalid floor request. Please enter floors 1-4.\n");
                }

            } else if(sscanf(token, "%d", &position_or_end) == 1) {        // If token has only 1 integer
                if (position_or_end == REQUEST_POSITION) {
                    add_request_to_queue(q, position_or_end, position_or_end);        // Elev position request added to the queue
                } else if (position_or_end == STOP) {                                 // Check stop condition
                    add_request_to_queue(q, position_or_end, position_or_end);        // End simulation request added to the queue
                }

            } else {
                printf("Invalid input. Please enter pairs of valid numbers.\n");
            }
            token = strtok(NULL, ",");
        }
    } else {
        printf("Error reading input. Please try again.\n");
    }
    //printQueue(q);
}

// The main elevator control
void elevator_main_control(queue* q) {
    int end_simulation = 0;                       // Set to 1 if userInput = -1 to signal termination of the simulation
    int starting_floor = 1;                       // Starting floor at start of simulation
    int last_floor = 1;                           // Last floor the elev visited
    
    // Initial prompt - at the start of the simulaton
    printf("\n\n\t*** Start of Simulation ***\n");
    printf("You may request 1 or more floors separated by comma.\n");
    printf("Select Floor 1-4 (Enter [-1] to stop; [0] for position of Elevator).\n");
    display_elevator(starting_floor);           // Show graphical rep of the elev with starting floor

    while (!end_simulation) {
        manage_the_requests(q, &end_simulation, &last_floor);             // Process the userInput requests
        run_the_elevator(q, &end_simulation, &last_floor);                // Operate the elevator
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

    elevator_main_control(&q);

    free_queue(&q);

    return 0;
}