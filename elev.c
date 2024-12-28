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
#define SIZE 100
#define UP -2
#define DOWN -3

// Queue structure
typedef struct {            // The queue has these properties
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
void add_request_to_queue(queue* q, int floor);
void get_request_from_queue(queue* q, int *next_floor);
void printQueue(queue* q);
void free_queue(queue* q);

void display_elevator(int current_floor);
void display_elevator_status(int *last_floor, int next_floor);
void display_elevator_motion(int *last_floor, int next_floor);
void move_the_elevator(queue* q, int next_floor, int *last_floor);
int read_user_input(char *buffer, size_t size);
int is_valid_request(int current_floor, int target_floor);
void sort_ascend(int arr[], int n);
void sort_descend(int arr[], int n);
void process_requests_batch(queue *q, int *array, int count);
void manage_the_requests(queue* q, int *flag, int *last_call, int *RC);
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
void add_request_to_queue(queue* q, int floor) {
    if (q->rear +1 == q->capacity) {       // Expand the queue if full
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
void display_elevator_status(int *last_floor, int next_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
	// Display the status for each floor
    for (int i=3; i>=0; i--) {
        if (i+1 == *last_floor) {
            // Determine elevator movement direction based on target_floor
            if (*last_floor < next_floor) {
                printf("%s [E] - Going up\n", floors[i]);
            } else if (*last_floor > next_floor) {
                printf("%s [E] - Going down\n", floors[i]);
            } else {
                printf("%s [E] - Arrived\n", floors[i]);
            }
        } else if (i +1 == next_floor) {
            printf("%s [ ] - Next stop\n", floors[i]);
        } else {
            printf("%s [ ]\n", floors[i]);
        }
    }
}

// Show the intermediate floors the elevator passes through
void display_elevator_motion(int *last_floor, int next_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};
	// Elevator moving from one floor to another
    if (*last_floor < next_floor) {
        printf("\nElevator is moving up...\n");
        for (int i = *last_floor; i < next_floor-1; i++) {
            printf("Arrived at %s\n", floors[i]);
        }
    } else if (*last_floor > next_floor) {
        printf("\nElevator is moving down...\n");
        for (int i = *last_floor - 1; i > next_floor; i--) {
            printf("Arrived at %s\n", floors[i-1]);
        }
    }
}

// Move the elevator to the requested floor
void move_the_elevator(queue* q, int next_floor, int *last_floor) {
    const char* floors[] = {"Floor 1", "Floor 2", "Floor 3", "Floor 4"};

    if (*last_floor == next_floor) {
       // printf("Elevator at ")
    } else {
        display_elevator_status(last_floor, next_floor);               // Show where the elev is and next stop
        
        display_elevator_motion(last_floor, next_floor);               // Show the arrival of the elev at each floor
        
        printf("Elevator has arrived at %s\n\n", floors[next_floor - 1]); // Show elev arrival at the target floor
    }
    *last_floor = next_floor;             // record the last floor visited - this is where the elev will idle
}

// Runs the elevator according to the requests in the Queue
void run_the_elevator(queue* q, int *end_simulation, int *last_floor) {
    if(*end_simulation) return;     // Simulation terminated

    int next_floor;                 // Next floor in the queue
        
    // Service the request
    while (!isEmpty(q)) {
        get_request_from_queue(q, &next_floor);   // Get the next request from the queue

        move_the_elevator(q, next_floor, last_floor);      // Fulfil the request
    }  
}

void sort_ascend(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sort_descend(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int read_user_input(char *buffer, size_t size) {
    printf("Request current floor and target floor (ex. '1 4, 2 3'): ");
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';       // Remove trailing newline
        return 1;           // Success
    }
    return 0;   // Failed
}

int is_valid_request(int current_floor, int target_floor) {
    return current_floor >= MIN_FLOOR && current_floor <= MAX_FLOOR &&
            target_floor >= MIN_FLOOR && target_floor <= MAX_FLOOR &&
            current_floor != target_floor;
}

// Add processed floor requests to the queue
void process_requests_batch(queue *q, int *array, int count) {
    // Only add the unique floors to the queue - eliminate duplicates
    for (int i = 0; i < count; i++) {
        int alreadyAdded = 0;                       
        // Check if current number already added to the queue
        for (int j = 0; j < i; j++) {
            if (array[j] == array[i]) {
                alreadyAdded = 1;
                break;
            }
        }
        // Add to result if not already added
        if (!alreadyAdded) {
            add_request_to_queue(q, array[i]);
            //printf("Floor %d added to Queue\n", array[i]);
        }    
    }
}

void manage_the_requests(queue* q, int *end_simulation, int *last_floor, int *RC){
    char line[256];
    int position_or_end;                 // position of elevator or end simulation request
    int direction = UP;                      // Direction based on the first request of each batch
    int is_first_request = 1;                // Flag to determine if its first request of each batch

    if (!read_user_input(line, sizeof(line))) {
        printf("Error reading input. Please try again.\n");
        return;
    }

    if (strlen(line) <= 2) {                                             // If only 1 or less input
        if (line[0] == '\n' || line[0] == '\0') {                        // If input is empty
            printf("Input is empty. Enter valid floor requests.\n");
            return;
        }  
    }

    // Array to store pre-processed floor requests
    int going_up[SIZE] = {0}, going_down[SIZE] = {0};       // Arrays to store floor requests
    int going_up_count = 0, going_down_count = 0;

    // Tokenize to get the pairs of requests
    char *token = strtok(line, ",");
    while (token != NULL) {
        int current_floor, target_floor;
        int position_or_end;                                // position of elevator or end simulation request

        while (isspace((unsigned char)*token)) token++;     // Skip spaces

        if (sscanf(token, "%d %d", &current_floor, &target_floor) == 2) {
            if (current_floor == STOP || target_floor == STOP) {                // Check stop condition
                *end_simulation = 1;                                            // Terminate the simulation
                printf("End of Simulation. Exiting...\n");
                return;
            }

            if (is_valid_request(current_floor, target_floor)) {
                if (current_floor < target_floor) {
                    going_up[going_up_count++] = current_floor;
                    going_up[going_up_count++] = target_floor;

                    // Set initial direction based on the first request
                    if (is_first_request) {
                        direction = UP;
                        is_first_request = 0;           // Disable first request flag
                    }
                } else {
                    going_down[going_down_count++] = current_floor;
                    going_down[going_down_count++] = target_floor;

                    if (is_first_request) {
                        direction = DOWN;
                        is_first_request = 0;           // Disable first request flag
                    }
                }
            } else {
                printf("Invalid floor request. Please enter floors 1-4.\n");
            }
            (*RC)++;        // Increment request counter
        } else if (sscanf(token, "%d", &position_or_end) == 1) {        // If token has only 1 integer
            /*
                You can only request elevator position or end simulation when no request in the simulation
            */
            if (position_or_end == REQUEST_POSITION) {
                //printf("Position requested\n");
                display_elevator(*last_floor);
                //add_request_to_queue(q, position_or_end, position_or_end);        // Elev position request added to the queue
            } else if (position_or_end == STOP) {                                 // Check stop condition
                //printf("End simulation requested\n");
                *end_simulation = 1;                                            // Terminate the simulation
                printf("End of Simulation. Exiting...\n");
                return;
                //add_request_to_queue(q, position_or_end, position_or_end);        // End simulation request added to the queue
            }
        } else {
            printf("Invalid input. Please enter pairs of valid numbers.\n");
        }
        token = strtok(NULL, ",");
    }

    // Proces all requests after all tokens are handled
    if (direction == UP) {
        sort_ascend(going_up, going_up_count);                      // Sort all going up requests in ascending order
        sort_descend(going_down, going_down_count);                 // Sort all going up requests in ascending order
        process_requests_batch(q, going_up, going_up_count);
        process_requests_batch(q, going_down, going_down_count);
    } else {
        sort_descend(going_down, going_down_count);
        sort_ascend(going_up, going_up_count);
        process_requests_batch(q, going_down, going_down_count);
        process_requests_batch(q, going_up, going_up_count);
    }
    //printQueue(q);
    printf("RC: %d\n", *RC);
    printQueue(q);
    printf("\n");
}

// The main elevator control
void elevator_main_control(queue* q) {
    int end_simulation = 0;                       // Set to 1 if userInput = -1 to signal termination of the simulation
    int starting_floor = 1;                       // Starting floor at start of simulation
    int last_floor = 1;                           // Last floor the elev visited
    int RC = 0;                                   // Request Counter
    
    // Initial prompt - at the start of the simulaton
    printf("\n\n\t*** Start of Simulation ***\n");
    printf("You may request 1 or more floors separated by comma.\n");
    printf("Select Floor 1-4 (Enter [-1] to stop; [0] for position of Elevator).\n");
    display_elevator(starting_floor);           // Show graphical rep of the elev with starting floor

    while (!end_simulation) {
        manage_the_requests(q, &end_simulation, &last_floor, &RC);             // Process the userInput requests
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