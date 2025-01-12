//*****************************************************************************

//  floor_request_handler.c   -   process the floor request user inputs

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//*****************************************************************************

int read_user_input(char *buffer, size_t size) {
    printf("Request current floor and target floor (ex. '1 4, 2 3'): ");
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';       // Remove trailing newline
        return 1;           // Success
    }
    return 0;   // Failed
}

// Eliminate duplicate requests - multiple requests to the same floor
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

void floor_request_handler(queue* q, int *end_simulation, int *last_floor, int *RC){
    char line[256];
    //int position_or_end;                 // position of elevator or end simulation request
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
    printf("Request Counter: %d\n", *RC);
    printQueue(q);
    printf("\n");
}
