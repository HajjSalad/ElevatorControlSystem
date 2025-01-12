//*****************************************************************************

//  elevator_main_control.c   -   implement elevator logic and control

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//*****************************************************************************

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
        floor_request_handler(q, &end_simulation, &last_floor, &RC);             // Process the userInput requests
        run_the_elevator(q, &end_simulation, &last_floor);                // Operate the elevator
    }
}

// Runs the elevator according to the requests in the Queue
void run_the_elevator(queue* q, int *end_simulation, int *last_floor) {
    if(*end_simulation) return;     // Simulation terminated

    int next_floor;                 // Next floor in the queue
        
    // Service the request
    while (!isEmpty(q)) {
        get_request_from_queue(q, &next_floor);   // Get the next request from the queue

        move_the_elevator(next_floor, last_floor);      // Fulfil the request
    }  
}

// Move the elevator to the requested floor
void move_the_elevator(int next_floor, int *last_floor) {
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
