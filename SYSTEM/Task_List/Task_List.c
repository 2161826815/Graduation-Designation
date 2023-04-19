#include "Task_List.h"
#include "TIM.h"
#include "USART.h"
#include "SysTick.h"
#include "LED.h"

extern volatile uint32_t tim_tick;
uint32_t hit_list_idx(uint32_t time)
{
    return ((tim_tick+time) & (HIT_LIST_TICK_MAX-1));
}

list_item long_task;
list_item hit_task[HIT_LIST_TICK_MAX];

void list_init(list_item* head)
{
    head->next = head;
    head->pre = head;
}

static uint8_t inline is_head_valid(list_item* head)
{
    if(head && head->next && head->pre){
        return 0;
    }else{
        return 1;
    }
}

//空返回1 ,非空返回0
uint8_t is_head_empty(list_item* head)
{
    if(!head || !head->next || !head->pre)
        return 1;
    else
        return 0;
}

static void inline list_insert(list_item* item,list_item *pre,list_item *next)
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
    if(is_head_empty(head))
        return NULL;
    list_item* del = LIST_NULL;
    del = head->next;
    list_delete(head,head->next);
    list_free(del);
    return del;
}

list_item* list_delete_tail(list_item *head)
{
    if(is_head_empty(head))
        return NULL;
    list_item* del = LIST_NULL;
    del = head->next;
    list_delete(head->pre,head);
    list_free(del);
    return del;
}

uint8_t list_add_tail(list_item* head,list_item *item)
{
    if(is_head_valid(head) || !item){
        return 1;
    }
    list_insert(item,head->pre,head);
		return 0;
}

uint8_t list_add_head(list_item* head,list_item *item)
{
    if(is_head_valid(head)){
        return 1;
    }
    list_insert(item,head,head->next);
		return 0;
}

//队列 从pre加
void task_add(Task_t *task,uint32_t time)
{
    list_delete_item(&task->task_item);
    
    if(time >= HIT_LIST_TICK_MAX){
        list_add_tail(&long_task,&(task->task_item));
    }else{
        list_add_tail(&hit_task[hit_list_idx(task->Period)],&(task->task_item));
    }
}

void task_dispatch()
{
    LED_Toggle(1);
    //DMA_Printf("1\r\n");
    list_item *item;
    list_item *n;
    Task_t *m_task;
    uint32_t idx,left;
    idx = hit_list_idx(0);
    if(idx == 0){
        list_for_each_next_safe(item,n,&long_task){
            m_task = container_of(Task_t,task_item,item); 
            m_task->arrive += HIT_LIST_TICK_MAX;
            left = m_task->Period-m_task->arrive;
            if(left == 0){                              //长周期任务到期
                m_task->task();
                m_task->arrive =0;  
            }else if(left < HIT_LIST_TICK_MAX){         //长周期任务即将到期
                task_add(m_task,left);
                DMA_Printf("period:%d\r\n",m_task->Period);
            }
        }
    }
    list_for_each_next_safe(item,n,&hit_task[idx]){    //击中链表
        m_task = container_of(Task_t,task_item,item);
        //printf("Period:%d\r\n",m_task->Period);
        //DMA_Printf("Period:%d\r\n",m_task->Period);
        m_task->task();
        m_task->arrive = 0;
        task_add(m_task,m_task->Period);
    }
}
