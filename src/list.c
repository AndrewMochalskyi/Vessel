/* 
 *
 * The MIT License
*
* Copyright (c) Andrew Mochalskyi
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


/**
 * Create a list, initializing all fields
 */
List *list_init(void) {

    List *l = malloc(sizeof(List));

    if (!l) {
        perror("malloc(3) failed");
        exit(EXIT_FAILURE);
    }

    // set default values to the List structure fields
    l->head = l->tail = NULL;
    l->len = 0L;

    return l;
}


/**
 * Destroy a list, releasing all allocated memory
 */
void list_free(List *l, int deep) {

    if (!l) return;

    ListNode *h = l->head;
    ListNode *tmp;

    // free all nodes
    while (l->len--) {

        tmp = h->next;

        if (h) {
            if (h->data && deep == 1) free(h->data);
            free(h);
        }

        h = tmp;
    }

    // free List structure pointer
    free(l);
}


/**
 * Attach a node to the head of a new list
 */
List *list_attach(List *l, ListNode *head, unsigned long len) {
    // set default values to the List structure fields
    l->head = head;
    l->len = len;
    return l;
}

/*!
 * Insert value at the front of the list
 * Complexity: O(1)
 */
List *list_push(List *l, void *val) {

    ListNode *new_node = malloc(sizeof(ListNode));

    if (!new_node) {
        perror("malloc(3) failed");
        exit(EXIT_FAILURE);
    }

    new_node->data = val;

    if (l->len == 0) {
        l->head = l->tail = new_node;
        new_node->next = NULL;
    } else {
        new_node->next = l->head;
        l->head = new_node;
    }

    l->len++;

    return l;
}


/*!
 * Insert value at the back of the list
 * Complexity: O(1)
 */
List *list_push_back(List *l, void *val) {

    ListNode *new_node = malloc(sizeof(ListNode));

    if (!new_node) {
        perror("malloc(3) failed");
        exit(EXIT_FAILURE);
    }

    new_node->data = val;
    new_node->next = NULL;

    if (l->len == 0) l->head = l->tail = new_node;
    else {
        l->tail->next = new_node;
        l->tail = new_node;
    }

    l->len++;

    return l;
}

/*! remove list
 *
 */
ListNode *list_remove(ListNode *head, ListNode *node, compare_func cmp) {

    if (!head)
        return NULL;

    if (cmp(head, node) == 0) {

        ListNode *tmp_next = head->next;
        free(head);

        return tmp_next;
    }

    head->next = list_remove(head->next, node, cmp);

    return head;
}
