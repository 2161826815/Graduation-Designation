#include "Task_List.h"
#include "TIM.h"
#include "USART.h"
#include "SysTick.h"
#include "LED.h"

list_item long_task = {
    .next = &long_task,
    .pre = &long_task,
};

list_item hit_task = {
    .next = &hit_task,
    .pre = &hit_task,
};

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

void list_priority_sort(list_item* head)
{
    if(!head || !head->pre || !head->next){
        return;
    }
    Task_t *a,*b;
    list_item *i,*j,*k,*x,*y;

    for(i = head->next->next; i != head; i = i->next){
        for(j = i; j != head; j = j->pre){
            a = container_of(Task_t,task_item,j);
            b = container_of(Task_t,task_item,j->pre);
            if((a->priority) < (b->priority)){
                k = j;
                x = j->pre->pre;
                y = j->pre;
                list_delete_item(j);
                list_insert(k,x,y);
            }
        }
    }

    /* printf("priority ");
    for(i = head->next; i != head; i = i->next){
        a = container_of(Task_t,task_item,i);
        printf("%d ",a->priority);
    }
    printf("\r\n"); */
}

//栈 从pre加
void task_add(Task_t *task)
{
    list_delete_item(&task->task_item);
    if(task->remain > HIT_TIME){
        list_add_tail(&long_task,&(task->task_item));
    }else{
        list_add_tail(&hit_task,&(task->task_item));
    }
}



#if 1
void task_dispatch()
{
    list_item *item;
    list_item *n;
    Task_t *m_task;
    list_for_each_next_safe(item,n,&long_task){
        m_task = container_of(Task_t,task_item,item);
            if(m_task->remain <= 0){    //任务到期
                m_task->atomic = 1;             //原子执行
                m_task->task();
                m_task->remain = m_task->Period;
                task_add(m_task);
                LED_Toggle(4);      //正在执行长周期任务
                m_task->atomic = 0;
            }else{
                if(m_task->remain < HIT_TIME)     //即将到期
                    task_add(m_task);
                    continue;
            }
    }

    list_priority_sort(&hit_task);   //击中链表优先级排序
    list_for_each_next_safe(item,n,&hit_task){    //击中链表
        m_task = container_of(Task_t,task_item,item);
        if(m_task->priority != 1 && m_task->priority != 9 && m_task->priority != 10)
            printf("task priority:%d\r\n",m_task->priority);
        m_task->task();
        LED_Toggle(5);  //击中任务完成
        m_task->remain = m_task->Period;
        task_add(m_task);
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

