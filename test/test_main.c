//*****************************************************************************

// test_main.c  -   main entry for unit tests

//*****************************************************************************

#include "elevator.h"
#include <stdio.h>

void test_init_queue();
void test_expand_queue();
void test_isFull();
void test_isEmpty();
void test_add_request_to_queue();
void test_get_request_from_queue();
void test_free_queue();
void test_is_valid_request();
void test_sort_ascend();
void test_sort_descend();
void test_process_requests_batch();

//*****************************************************************************

int main() {

    printf("\n\nRunning tests...\n\n");

    test_init_queue();
    test_expand_queue();
    test_isFull();
    test_isEmpty();
    test_add_request_to_queue();
    test_get_request_from_queue();
    test_free_queue();
    test_is_valid_request();
    test_sort_ascend();
    test_sort_descend();
    test_process_requests_batch();

    printf("\nAll tests passed successfully.\n\n");
}