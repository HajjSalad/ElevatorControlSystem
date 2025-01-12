//*****************************************************************************

//  utils.c   -   handle sorting and validation functions

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>

//*****************************************************************************

// Check user input is valid current and target floors
int is_valid_request(int current_floor, int target_floor) {
    return current_floor >= MIN_FLOOR && current_floor <= MAX_FLOOR &&
            target_floor >= MIN_FLOOR && target_floor <= MAX_FLOOR &&
            current_floor != target_floor;
}

// Sort the request in ascending order - going up
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

// Sort the request in descending order - going down
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

//*****************************************************************************