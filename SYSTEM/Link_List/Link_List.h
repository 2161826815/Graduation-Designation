#ifndef _LINK_LIST_H
#define _LINK_LIST_H

#include <stdint.h>
#include <stdio.h>

#define LIST_NULL   ((void*)0)

typedef struct list_item{
    struct list_item *pre;
    struct list_item *next;
}list_item;

#define container_of(type,mem,ptr) \
    ((type*)((char*)ptr - (char*)(&((type*)0)->mem)))

#define list_for_each_next_safe(item,n,head) \
    for(item = (head)->next,n = item->next;item != (head) ;item = n,n = item->next)

#define list_for_each_pre_safe(item,head) \
    for(item = (head)->pre,n = item->pre;item != (head) ;item = n,n = item->pre)

#define cir_each_safe(item,n,head) \
    for(item = (head)->pre,n = item->pre;head != LIST_NULL ;item = n,n = item->pre)

void list_init(list_item* head);
uint8_t list_delete_item(list_item *item);
list_item* list_delete_head(list_item *head);
list_item* list_delete_tail(list_item *head);
uint8_t list_add_tail(list_item* head,list_item *item);
uint8_t list_add_head(list_item* head,list_item *item);

#endif
