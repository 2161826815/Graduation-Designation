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
    list_delete_item(&(task->task_item));
    list_add_head(head,&(task->task_item));
}

void task_dispatch(list_item* head)
{
    list_item *item;
    list_item *n;
    Task_t* m_task;
    list_for_each_next_safe(item,n,head){
        m_task = container_of(Task_t,task_item,item);
        m_task->remain += get_tick()+m_task->Period*1000;
    }
    while(1){
        list_for_each_next_safe(item,n,head){
            m_task = container_of(Task_t,task_item,item);
            if(get_tick() == m_task->remain){
                printf("dao qi\r\n");
                m_task->task();  
                m_task->remain += get_tick()+m_task->Period*1000;
                //task_add(head,m_task);
            }
        }
    }
}
