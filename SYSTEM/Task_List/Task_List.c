#include "Task_List.h"
#include "TIM.h"
#include "USART.h"
#include "SysTick.h"
#include "LED.h"
void list_init(list_item* head)
{
    head->next = head;
    head->pre = head;
}

uint8_t is_head_valid(list_item* head)
{
    if(head && head->next && head->pre){
        return 0;
    }else{
        return 1;
    }
}

void list_insert(list_item* item,list_item *pre,list_item *next)
{
    item->next = next;
    item->pre = pre;

    next->pre = item;
    pre->next = item;  
}

void list_delete(list_item *pre,list_item *next)
{
    next->pre = pre;
    pre->next = next;
}

void list_free(list_item *item)
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
    del = head->next;
    list_delete(head,head->next);
    list_free(del);
    return del;
}

list_item* list_delete_tail(list_item *head)
{
    list_item* del = LIST_NULL;
    del = head->next;
    list_delete(head->pre,head);
    list_free(del);
    return del;
}

//栈next循环
uint8_t list_add_tail(list_item* head,list_item *item)
{
    if(is_head_valid(head)){
        return 1;
    }
    list_insert(item,head->pre,head);
		return 0;
}

//队列 next循环
uint8_t list_add_head(list_item* head,list_item *item)
{
    if(is_head_valid(head)){
        return 1;
    }
    list_insert(item,head,head->next);
		return 0;
}

//栈 从pre加
void task_add(list_item *head,Task_t *task)
{
    list_delete_item(&(task->task_item));
    list_add_tail(head,&(task->task_item));
}

#if 1
void task_dispatch(list_item* head)
{
    list_item *item;
    list_item *n;
    Task_t *m_task;
    while(1){
        list_for_each_next_safe(item,n,head){
            LED_Toggle(5);      //判断任务正在切换
            m_task = container_of(Task_t,task_item,item);
            m_task->atomic = 0;
            if(m_task->Period > 0){     //周期任务
                if(m_task->remain <= 0){
                    m_task->atomic = 1;             //原子执行
                    m_task->task();
                    m_task->remain = m_task->Period;
                    task_add(head,m_task);
                    m_task->atomic = 0;
                }else{
                    if(m_task->remain < TIME_SLICE)
                        task_add(head,m_task);
                        continue;
                }
            }else{  //非周期任务
                m_task->task();
            }
        }      
    }
}
#endif
#if 0
void task_dispatch(list_item* head)
{
    list_item *item;
    list_item *n;
    Task_t *m_task;
    list_for_each_next_safe(item,n,head){
        m_task = container_of(Task_t,task_item,item);
        m_task->remain = tim_get_tick()+m_task->Period;
    }
    while(1){
        list_for_each_next_safe(item,n,head){
            LED_Toggle(5);      //判断任务正在切换
            m_task = container_of(Task_t,task_item,item);
            if(m_task->Period > 0){
                if(tim_get_tick() >= m_task->remain){
                    m_task->task();
                    m_task->remain = tim_get_tick()+m_task->Period;
                }else{
                    continue;
                }
            }else{
                m_task->task();
            }  
        }
    }
}
#endif

