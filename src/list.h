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


#ifndef LIST_H
#define LIST_H


struct list_node {
    void *data;
    struct list_node *next;
};


typedef struct list_node ListNode;


typedef struct {
    ListNode *head;
    ListNode *tail;
    unsigned long len;
} List;


/*! Compare function, accept two void * arguments, generally referring a node
   and his subsequent */
typedef int (*compare_func)(void *, void *);

/*! Create an empty list */
List *list_init(void);

/*! Release a list, accept a integer flag to control the depth of the free call
 * (e.g. going to free also data field of every node) */
void list_free(List *, int);

/*! Attach a list to another one on tail */
List *list_attach(List *, ListNode *, unsigned long);

/*! Insert data into a node and push it to the front of the list */
List *list_push(List *, void *);

/*! Insert data into a node and push it to the back of the list */
List *list_push_back(List *, void *);

/*! Remove a node from the list based on a compare function that must be
   previously defined and passed in as a function pointer, accept two void *
   args, which generally means a node and his subsequent */
ListNode *list_remove(ListNode *, ListNode *, compare_func);


#endif
