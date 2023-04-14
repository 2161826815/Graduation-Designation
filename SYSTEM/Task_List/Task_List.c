#include "Task_List.h"
#include "SysTick.h"
#include "USART.h"

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

//队列 next循环
uint8_t list_add_head(list_item* head,list_item *item)
{
    if(is_head_valid(head)){
        return 1;
    }
    list_insert(item,head,head->next);
		return 0;
}

//栈 next循环
uint8_t list_add_tail(list_item* head,list_item *item)
{
    if(is_head_valid(head)){
        return 1;
    }
    list_insert(item,head->pre,head);
		return 0;
}

//队列从next加
void task_add(list_item *head,Task_t *task)
{
    list_add_head(head,&(task->task_item));
}

void task_dispatch(list_item* head)
{
    uint32_t task_period;
    list_item *item;
    list_item *n;
    Task_t* m_task;
    list_for_each_next_safe(item,n,head){
        m_task = container_of(Task_t,task_item,item);
        
        m_task->task();
    }
}
