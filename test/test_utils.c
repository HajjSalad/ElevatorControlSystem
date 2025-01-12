//*****************************************************************************

//  test_utils.c   -   unit test for utils.c

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>
#include <assert.h>

//*****************************************************************************

void test_is_valid_request() {
    assert(is_valid_request(1,3) == 1);
    assert(is_valid_request(2,4) == 1);
    assert(is_valid_request(4,3) == 1);
    assert(is_valid_request(2,1) == 1);
    assert(is_valid_request(MIN_FLOOR, MAX_FLOOR) == 1);

    assert(is_valid_request(MIN_FLOOR - 1, 5) == 0);
    assert(is_valid_request(3, MAX_FLOOR + 1) == 0);
    assert(is_valid_request(3, 3) == 0);
    printf("\ntest_is_valid_request passed!\n");
}

void test_sort_ascend() {
    int arr[] = {2,4,1,3,2,4,2,4,3,2};
    int n = sizeof(arr)/sizeof(int);
    sort_ascend(arr, n);
    
    for (int i=0; i<n-1; i++) {
        assert(arr[i] <= arr[i+1]);
    }
    printf("test_sort_ascend passed!\n");
}

void test_sort_descend() {
    int arr[] = {2,4,1,3,2,4,2,4,3,2};
    int n = sizeof(arr)/sizeof(int);
    sort_descend(arr, n);
    
    for (int i=0; i<n-1; i++) {
        assert(arr[i] >= arr[i+1]);
    }
    printf("test_sort_ascend passed!\n");
}
