#include "main.h"
#include "init.h"

list_item task_head;
int main(void)
{
  
  peripheral_init(&task_head);

  task_dispatch(&task_head);

  while(1){
    /* ds18b20_task();
    delay_ms(1000); */
  }
}

