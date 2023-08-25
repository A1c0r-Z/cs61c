#include <stddef.h>
#include "ex10_ll_cycle.h"

int ll_has_cycle(node *head) {
    // Start with two pointers at the head of the list
    node *fast_ptr = head;
    node *slow_ptr = head;

    while (fast_ptr && fast_ptr->next) {
        // Advance fast_ptr by two nodes
        fast_ptr = fast_ptr->next->next;

        // Advance slow_ptr by one node
        slow_ptr = slow_ptr->next;

        // If the fast_ptr and slow_ptr point to the same node, the list is cyclic
        if (fast_ptr == slow_ptr) {
            return 1; // List has a cycle
        }
    }

    return 0; // List is acyclic
}