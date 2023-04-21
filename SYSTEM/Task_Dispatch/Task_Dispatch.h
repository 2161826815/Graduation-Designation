#ifndef _TASK_DISPATCH_H
#define _TASK_DISPATCH_H

#include "stm32f10x.h"
#include "Link_List.h"
#include "init.h"

#define Period_to_Tick(p)  ((((uint32_t)(p) >= TIM_IT_TIME)) ? ((uint32_t)(p)/TIM_IT_TIME) : 1U)

#define HIT_LIST_TICK_MAX   (16)
#define TIM_IT_TIME         (5)

typedef struct Task{
    void (*task)(void);
    uint32_t period;
    uint32_t arrive;
    list_item item;
    uint8_t id;
    uint32_t pri_data;
}Task_t;

#define DISPATCH_TASK_CTOR(_TASK, _PERIOD, _TASK_ID) {   \
        .task = (_TASK),                        \
        .period = (_PERIOD),                    \
        .arrive = 0,                            \
        .item.next = LIST_NULL,                 \
        .item.pre  = LIST_NULL,                 \
        .id = (_TASK_ID),                       \
        .pri_data = 0,                          \
    }

void dispatch_task_add(Task_t *task,uint32_t time);
void task_dispatch(void);
void task_dispatch_looper(void);
#endif
