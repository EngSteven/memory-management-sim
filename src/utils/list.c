/**
 * @file list.c
 * @brief Minimal singly-linked list implementation used by the memory simulator.
 *
 * This module provides a lightweight singly-linked list with basic push-front
 * insertion and full deallocation. It is intentionally minimal because the
 * project only requires simple tracking structures (e.g., allocated blocks,
 * free blocks, variable mappings).  
 *
 * All nodes store a generic `void *` data pointer, allowing flexible reuse for
 * multiple components of the simulator.
 */

#include <stdlib.h>
#include "list.h"

/* ------------------------------------------------------------------------- */
/*  Basic List Operations                                                    */
/* ------------------------------------------------------------------------- */

/**
 * @brief Inserts a new element at the beginning of the list.
 *
 * Allocates a new ::ListNode, assigns the provided data pointer, and updates
 * the list head. If memory allocation fails, the original list head is
 * returned unchanged.
 *
 * @param head Pointer to the current head of the list. May be NULL.
 * @param data Pointer to the data to store in the newly created node.
 *
 * @return Pointer to the new head of the list. If allocation fails, returns
 *         the original head.
 */
ListNode *list_push_front(ListNode *head, void *data) {
    ListNode *node = malloc(sizeof(ListNode));
    if (!node) 
        return head;

    node->data = data;
    node->next = head;
    return node;
}

/**
 * @brief Frees an entire list, node by node.
 *
 * Iterates over the list and frees every node. If a custom free function is
 * provided, it is applied to each node's `data` pointer before releasing the
 * node itself.  
 *
 * This function safely handles NULL pointers.
 *
 * @param head Pointer to the first node of the list. May be NULL.
 * @param free_fn Optional callback used to free the memory pointed to by
 *                `node->data`. If NULL, no attempt is made to free the data.
 */
void list_free(ListNode *head, void (*free_fn)(void *)) {
    ListNode *curr = head;
    while (curr) {
        ListNode *next = curr->next;

        if (free_fn) {
            free_fn(curr->data);
        }

        free(curr);
        curr = next;
    }
}
