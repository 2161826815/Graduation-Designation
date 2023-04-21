#include "Link_List.h"

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
