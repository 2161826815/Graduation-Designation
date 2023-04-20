#ifndef _TASK_LIST_H
#define _TASK_LIST_H

#include "stm32f10x.h"
#include <stdio.h>

#define LIST_NULL   ((void*)0)

#define HIT_LIST_TICK_MAX   (16)
#define TIM_IT_TIME         (5)

#define Period_to_Tick(p)  ((((uint32_t)(p) >= TIM_IT_TIME)) ? ((uint32_t)(p)/TIM_IT_TIME) : 1U)

typedef struct list_item{
    struct list_item *pre;
    struct list_item *next;
}list_item;

typedef struct Task{
    void (*task)(void);
    uint32_t period;
    uint32_t arrive;
    list_item item;
    uint8_t id;
    uint32_t pri_data;
}Task_t;

#define TASK_CTOR(_TASK, _PERIOD, _TASK_ID) {   \
        .task = (_TASK),                        \
        .period = (_PERIOD),                    \
        .arrive = 0,                            \
        .item.next = LIST_NULL,                 \
        .item.pre  = LIST_NULL,                 \
        .id = (_TASK_ID),                       \
        .pri_data = 0,                          \
    }

#define container_of(type,mem,ptr) \
    ((type*)((char*)ptr - (char*)(&((type*)0)->mem)))

#define list_for_each_next_safe(item,n,head) \
    for(item = (head)->next,n = item->next;item != (head) ;item = n,n = item->next)

#define list_for_each_pre_safe(item,head) \
    for(item = (head)->pre,n = item->pre;item != (head) ;item = n,n = item->pre)

#define cir_each_safe(item,n,head) \
    for(item = (head)->pre,n = item->pre;head != LIST_NULL ;item = n,n = item->pre)

void list_init(list_item* head);
void task_add(Task_t *task,uint32_t time);
void task_dispatch(void);
uint32_t hit_list_idx(uint32_t time);
#endif
