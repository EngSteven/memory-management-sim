#ifndef LIST_H
#define LIST_H

/**
 * @file list.h
 * @brief Generic singly linked list utilities.
 *
 * This module defines a minimalistic generic singly linked list structure
 * and the basic operations needed for insertion and destruction.
 * Implementations are provided in src/utils/list.c.
 */

/**
 * @struct ListNode
 * @brief Node of a generic singly linked list.
 *
 * Each node stores a pointer to arbitrary user-provided data and
 * a pointer to the next node in the list.
 */
typedef struct ListNode {
    void *data;              /**< Pointer to the stored element (generic type). */
    struct ListNode *next;   /**< Pointer to the next node in the list. */
} ListNode;

/**
 * @brief Inserts a new node at the front of the list.
 *
 * Allocates a new ListNode, assigns the provided data pointer,
 * and sets it as the new head of the list.
 *
 * @param head Pointer to the current head of the list (may be NULL).
 * @param data Pointer to the data to store in the new node.
 * @return ListNode* Pointer to the new head of the list.
 */
ListNode *list_push_front(ListNode *head, void *data);

/**
 * @brief Frees all nodes in the list.
 *
 * Traverses the list and frees each node. If a custom destructor is provided
 * via `free_fn`, it is applied to each node's data before freeing the node.
 *
 * @param head Pointer to the first node of the list (may be NULL).
 * @param free_fn Function pointer to a destructor for the node data.
 *                If NULL, the stored data is not freed.
 */
void list_free(ListNode *head, void (*free_fn)(void *));

#endif /* LIST_H */
