#include "main.h"
#include "init.h"


int main(void)
{
  list_item task_head;
  peripheral_init(&task_head);
  task_dispatch(&task_head);
  while(1){
    
  }
}

