#include "Task_List.h"
#include "TIM.h"
#include "USART.h"
#include "SysTick.h"
#include "LED.h"

uint32_t hit_list_idx(uint32_t time)
{
    return ((tim_get_tick()+time) & (HIT_LIST_TICK_MAX-1));
}

list_item long_task;
list_item hit_task[HIT_LIST_TICK_MAX];

void list_init(list_item* head)
{
    head->next = head;
    head->pre = head;
}

static inline uint8_t is_head_valid(list_item* head)
{
    return (head && head->next && head->pre);
}

static inline uint8_t is_head_empty(list_item* head)
{
    return is_head_valid(head) ? (head->next == head) : 1;
}
static inline void list_insert(list_item* item,list_item *pre,list_item *next)
{
    next->pre = item;
    item->next = next;
    item->pre = pre;
    pre->next = item;  
}

static inline void list_delete(list_item *pre,list_item *next)
{
    next->pre = pre;
    pre->next = next;
}

static inline void list_free(list_item *item)
{
    item->next = LIST_NULL;
    item->pre = LIST_NULL;
}

uint8_t list_delete_item(list_item *item)
{
    if(!item || !item->pre || !item->next){
        return 1;
    }
    list_delete(item->pre,item->next);
    list_free(item);
    return 0;
}

list_item* list_delete_head(list_item *head)
{
    list_item* del = LIST_NULL;

    if(is_head_empty(head))
        return LIST_NULL;

    del = head->next;
    list_delete(head,head->next);
    list_free(del);
    return del;
}

list_item* list_delete_tail(list_item *head)
{
    list_item* del = LIST_NULL;

    if(is_head_empty(head))
        return NULL;

    del = head->next;
    list_delete(head->pre,head);
    list_free(del);
    return del;
}

uint8_t list_add_tail(list_item* head,list_item *item)
{
    if(!is_head_valid(head) || !item){
        return 1;
    }
    list_insert(item,head->pre,head);
		return 0;
}

uint8_t list_add_head(list_item* head,list_item *item)
{
    if(!is_head_valid(head) || !item){
        return 1;
    }
    list_insert(item,head,head->next);
		return 0;
}

void task_add(Task_t *task,uint32_t time)
{
    list_delete_item(&task->item);
    
    if(time >= HIT_LIST_TICK_MAX){
        list_add_tail(&long_task,&(task->item));
    }else{
        list_add_tail(&hit_task[hit_list_idx(task->period)],&(task->item));
    }
}

void task_dispatch()
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
                task_add(m_task,left);
            }
        }
    }
    list_for_each_next_safe(cur_item,next_item,&hit_task[idx]){
        m_task = container_of(Task_t,item,cur_item);
        m_task->task();
        m_task->arrive = 0;
        task_add(m_task,m_task->period);
    }
}
