#ifndef _TASK_LIST_H
#define _TASK_LIST_H

#include "stm32f10x.h"
#include <stdio.h>

typedef struct list_item{
    struct list_item *pre;
    struct list_item *next;
}list_item;

typedef struct Task{
    void (*task)(void);
    uint32_t Period;
    list_item task_item;
}Task_t;

#define container_of(type,mem,ptr) \
    ((type*)((char*)ptr - (char*)(&((type*)0)->mem)))

//常用
#define list_for_each_next_safe(item,n,head) \
    for(item = (head)->next,n = item->next;item != (head) ;item = n,n = item->next)

#define list_for_each_pre_safe(item,head) \
    for(item = (head)->pre,n = item->pre;item != (head) ;item = n,n = item->pre)


void list_init(list_item* head);
void task_add(list_item *head,Task_t *task);
void task_dispatch(list_item* head);
#endif
