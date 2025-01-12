//*****************************************************************************

//  elevator_display.c   -   manage elevator display and visualization functions

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>

//*****************************************************************************

// Print the current queue
void printQueue(queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Request History: ");
    for (int i = 0; i < q->rear; i++) {
        printf("%d ", q->calls[i]);
    }
    printf("\n");
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


//*****************************************************************************