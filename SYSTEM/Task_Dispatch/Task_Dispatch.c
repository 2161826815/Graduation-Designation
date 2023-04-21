#include "Task_Dispatch.h"
#include "TIM.h"
#include "USART.h"
#include "SysTick.h"
#include "LED.h"
#include "Link_List.h"
#include "init.h"
uint32_t hit_list_idx(uint32_t time)
{
    return ((tim_get_tick()+time) & (HIT_LIST_TICK_MAX-1));
}

list_item long_task;
list_item hit_task[HIT_LIST_TICK_MAX];


void dispatch_task_add(Task_t *task,uint32_t time)
{
    list_delete_item(&task->item);
    
    if(time >= HIT_LIST_TICK_MAX){
        list_add_tail(&long_task,&(task->item));
    }else{
        list_add_tail(&hit_task[hit_list_idx(task->period)],&(task->item));
    }
}

void task_dispatch(void)
{
    list_item *cur_item;
    list_item *next_item;
    Task_t *m_task;
    uint32_t idx,left;
    idx = hit_list_idx(0);
    if(idx == 0){
        list_for_each_next_safe(cur_item,next_item,&long_task){
            m_task = container_of(Task_t,item,cur_item);
            m_task->arrive += HIT_LIST_TICK_MAX;
            left = m_task->period-m_task->arrive;
            if(left == 0){
                m_task->task();
                m_task->arrive =0;  
            }else if(left < HIT_LIST_TICK_MAX){
                dispatch_task_add(m_task,left);
            }
        }
    }
    list_for_each_next_safe(cur_item,next_item,&hit_task[idx]){
        m_task = container_of(Task_t,item,cur_item);
        m_task->task();
        m_task->arrive = 0;
        dispatch_task_add(m_task,m_task->period);
    }
}

void task_dispatch_looper()
{
    static uint32_t pre_tick;
    uint32_t cur_tick;

    tim3_init((TIM_IT_TIME*10-1),7199);

    for(;;){
        cur_tick = tim_get_tick();
        if(cur_tick != pre_tick){  
            pre_tick++;
            task_dispatch();
        }else{
            continue;
        }
    } 
}
