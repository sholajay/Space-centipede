//-----------------------------------------
// NAME: Sholaja Yusuf 
// STUDENT NUMBER: 7723362
// COURSE: COMP 3430
// INSTRUCTOR: Jim Young
// ASSIGNMENT: 2
//
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "List.h"

//static Node *traverseNode = NULL;

List *list_create(int item_size)
{
    List *list = NULL;

    if(item_size > 0)
    {
        list = (List *)malloc(sizeof(List));
        list->count = 0;
        list->head = NULL;
        list->tail = NULL;
        list->item_size = item_size;
        list->test = (char *)malloc(sizeof(25));;
    }
    return list;
}

void list_insert( List *list, void *new_item )
{
    if(list != NULL && new_item != NULL)
    {
        Node *newNode = (Node *)malloc (sizeof(Node));
        newNode->item = (void *)malloc(list->item_size);
        newNode->next = NULL;

        memcpy(newNode->item, new_item, list->item_size);

        if(list->count == 0)
        {
        //first item in the list
            list->head = newNode;
            list->tail = newNode;
        }
        else
        {
            list->tail->next = newNode;
            list->tail = newNode;
        }
        list->count++;

        sprintf(list->test, "%d ", list->count);
    }
}

List *list_unlink(List *old_list , int count)
{
    assert(old_list != NULL);
    assert(old_list->head != NULL);

    List *new_list = NULL;
    if(old_list != NULL && count < old_list->count)
    {
        new_list = list_create(old_list->item_size);
        int i = 0;
        Node *curr = old_list->head;
        Node *prev = NULL;

        while(i < count && curr != NULL)
        {
            prev = curr;
            curr = curr->next;
            i ++;
        }
        if(curr != NULL)
        {
            new_list->head = curr;
            prev->next = NULL;
        }
    }
    return new_list;
}

bool list_destroy( List *list )
{
    assert(list != NULL);

    bool destroy = false;
    Node *curr;
    while(list->head != NULL)
    {
        curr = list->head;
        list->head = curr->next;

        free(curr->item);
        free(curr);
    }
    destroy = true;
    return destroy;
}
void* list_firstItem(List *list)
{
    void *item  = NULL;
    if(list != NULL &&  list->head)
    {
        item = list->head->item;
        list->traverseNode = list->head->next;
        //memcpy(item, list->head->item, list->item_size);
    }
    return   item;
}
void* list_nextItem(List *list )
{   
    void *item = NULL;
    if(list != NULL && list->traverseNode != NULL)
    {
        item = list->traverseNode->item;
        list->traverseNode = list->traverseNode->next;
    }
    return item;
}

void* list_lastItem(List *list)
{
    void *item = NULL;
    if(list != NULL && list->tail != NULL)
    {
        item = list->tail->item;
        // memcpy(item, list->tail->item, list->item_size);
    }
  
    return item;
}
//void search( List *list, void *target );
