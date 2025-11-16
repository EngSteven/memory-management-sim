#ifndef LIST_H
#define LIST_H

/*
 * Utilidades genéricas de listas (si las necesitamos).
 * Se implementan en src/utils/list.c
 */

typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;

ListNode *list_push_front(ListNode *head, void *data);
void      list_free(ListNode *head, void (*free_fn)(void *));

#endif /* LIST_H */
