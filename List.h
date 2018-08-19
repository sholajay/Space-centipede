#ifndef MY_LIST_H
#define MY_LIST_H
#include <stdbool.h>
//typedef enum BOOL { false, true } Boolean;

// Linked list node definition
typedef struct NODE Node;
struct NODE
{
    void *item;
    Node *next;
};

typedef struct LIST List;
struct LIST
{
    Node *head;         //first element in the list
    Node *tail;         //last element in the list
    Node *traverseNode; //traverse through the list
    int item_size;      //size of each element
    int count;          //number of elemen int the list
    char *test;
};

List *list_create(int item_size);

//appends item to the end of the list
void list_insert( List *list, void *new_item );

//given a list and count , it disconnect the list at count and
//returns a new list starting at count
List *list_unlink(List *old_list , int count);

//deletes the whole list
bool list_destroy( List *list );

void* list_firstItem(List *list);

void* list_nextItem(List *list);

void* list_lastItem(List *list);

#endif
