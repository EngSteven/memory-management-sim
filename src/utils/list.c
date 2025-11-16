#include <stdlib.h>
#include "list.h"

/*
 * Implementación mínima temporal.
 */

ListNode *list_push_front(ListNode *head, void *data) {
    ListNode *node = malloc(sizeof(ListNode));
    if (!node) return head;
    node->data = data;
    node->next = head;
    return node;
}

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
